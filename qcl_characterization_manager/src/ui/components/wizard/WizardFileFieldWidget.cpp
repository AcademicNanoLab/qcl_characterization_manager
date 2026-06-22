/**
 * @file WizardFileFieldWidget.cpp
 * @brief Implementation of the WizardFileFieldWidget class for managing file input fields in the wizard.
 * 
 * This widget provides a user interface to add, display, and manage multiple files associated with a wizard step.
 * It supports listing files with corresponding variable names, allows adding new files via a dialog,
 * and manages error states related to file input.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#include "WizardFileFieldWidget.h"
#include "WizardFileFieldWidgetFile.h"

#include <QStandardPaths>
#include "ui/components/text/Text.h"
#include "ui/components/buttons/PushButton.h"

/**
 * @brief Constructs a WizardFileFieldWidget.
 *
 * Initializes the file selection UI elements, including header labels,
 * scroll area for displaying selected files, and the add file button.
 * Connects relevant signals to manage file changes.
 *
 * @param page The wizard page to which this widget belongs.
 * @param name The name of this wizard field.
 * @param fileVariableName The variable name associated with the file field.
 * @param parent The parent QWidget.
 */
WizardFileFieldWidget::WizardFileFieldWidget(WizardPage *page,
                                             const QString &name,
                                             const QString &fileVariableName,
                                             QWidget *parent)
    : WizardFieldWidget(page, name, WizardField::WizardFieldType::FileField, parent),
      fileVariableName(fileVariableName),
      fileDialog(new QFileDialog(this)),
      downloadPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation))
{
    this->setObjectName("fileField");
    page->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    errorArea->layout()->addWidget(errorAreaText);

    layout()->setContentsMargins(0, 0, 0, 0);

    // Header area setup
    Widget *headerArea = new Widget();
    QHBoxLayout *headerAreaLayout = new QHBoxLayout();
    headerArea->setLayout(headerAreaLayout);
    layout()->addWidget(headerArea);

    // File name label in header
    Text *fileName = new Text("Filename", "h3");
    fileName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    headerAreaLayout->addWidget(fileName);

    // File variable label in header
    Text *fileVariable = new Text(fileVariableName, "h3");
    fileVariable->setFixedWidth(150);
    headerAreaLayout->addWidget(fileVariable);

    // Placeholder button in header (disabled)
    PushButton *closeButtonPlaceholder = new PushButton(QIcon(""));
    closeButtonPlaceholder->setFixedSize(30, 30);
    closeButtonPlaceholder->setDisabled(true);
    headerAreaLayout->addWidget(closeButtonPlaceholder);

    // Horizontal line divider
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout()->addWidget(line);

    // Scroll area for file widgets
    QScrollArea *fileFieldScrollArea = new QScrollArea();
    fileFieldScrollContents = new Widget();
    fileFieldScrollLayout = new QVBoxLayout();

    fileFieldScrollArea->setWidget(fileFieldScrollContents);
    fileFieldScrollArea->setWidgetResizable(true);
    fileFieldScrollContents->setLayout(fileFieldScrollLayout);
    fileFieldScrollLayout->setAlignment(Qt::AlignTop);
    fileFieldScrollLayout->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(fileFieldScrollArea);

    // Add file button
    PushButton *addFileButton = new PushButton("+", "contained");
    layout()->addWidget(addFileButton);

    // Connect add button click signal to corresponding slot
    connect(addFileButton, &PushButton::clicked, this, &WizardFileFieldWidget::addButtonClicked);
	field->connectFileField(this, SIGNAL(fileChangeSignal(QVariantMap)), fileVariableName);
}

/**
 * @brief Clears all selected files from the widget.
 *
 * Deletes all individual file widgets currently displayed,
 * effectively resetting the widget's file selection to empty.
 */
void WizardFileFieldWidget::clear()
{
    // Delete all file widgets from the scroll area
    for (WizardFileFieldWidgetFile *individualFileWidget :
         fileFieldScrollContents->findChildren<WizardFileFieldWidgetFile *>())
    {
        individualFileWidget->deleteLater();
    }
}

/**
 * @brief Handles the click event of the "Add File" button.
 *
 * Opens a file dialog allowing the user to select one or more files.
 * For each selected file, it updates the file widget list accordingly.
 */
void WizardFileFieldWidget::addButtonClicked()
{
    // Open file dialog and get the list of selected files
    QStringList files = fileDialog->getOpenFileNames(this,
                                                     "Select one or more files to open",
                                                     downloadPath,
                                                     "Text files (*.*)",
                                                     nullptr,
                                                     QFileDialog::ReadOnly);

    // Add each selected file to the widget
    for (const QString &file : files)
    {
        updateFileWidget(file);
    }
}

/**
 * @brief Slot called when a file is selected.
 *
 * Updates the file widget to include the newly selected file.
 *
 * @param fileName The path of the selected file.
 */
void WizardFileFieldWidget::fileSelected(const QString &fileName)
{
    updateFileWidget(fileName);
}

/**
 * @brief Adds or updates a file widget for a given file.
 *
 * Creates a new file widget for the provided file name, adds it to the scroll layout,
 * and connects its signals to handle text changes and deletion.
 *
 * @param fileName The path of the file to add/update in the widget list.
 */
void WizardFileFieldWidget::updateFileWidget(const QString &fileName)
{
    // Create a new file widget and add it to the layout
    WizardFileFieldWidgetFile *fileWidget = new WizardFileFieldWidgetFile(fileName, fileVariableName);
    fileFieldScrollLayout->addWidget(fileWidget);

    // Connect signals for text change and file deletion
    connect(fileWidget,
            &WizardFileFieldWidgetFile::fileTextChanged,
            this,
            &WizardFileFieldWidget::fileTextChangedSlot);

    connect(fileWidget,
            &WizardFileFieldWidgetFile::fileDeleted,
            this,
            &WizardFileFieldWidget::fileDeletedSlot);

    // Initialize file widget with empty value
    emit fileWidget->fileTextChanged(fileName, "");
}

/**
 * @brief Slot triggered when the text associated with a file widget changes.
 *
 * Updates the internal file map with the new value and emits a signal indicating the change.
 * Also resets any error state in the widget.
 *
 * @param fileName The name/path of the file whose text changed.
 * @param fileVariableValue The new text/value associated with the file.
 */
void WizardFileFieldWidget::fileTextChangedSlot(const QString &fileName, const QString &fileVariableValue)
{
    // Update the file map with the new value
    fileMap[fileName] = fileVariableValue;

    // Emit the signal to notify about the file change
    emit fileChangeSignal(fileMap);

    // Reset the error state
    setErrorState(ErrorState::NoError);
}

/**
 * @brief Slot triggered when a file widget signals deletion.
 *
 * Removes the file from the internal map and emits a signal to notify about the deletion.
 *
 * @param fileName The name/path of the file to remove.
 */
void WizardFileFieldWidget::fileDeletedSlot(const QString &fileName)
{
    // Remove the file entry from the map
    fileMap.remove(fileName);

    // Emit the signal to notify about the file deletion
    emit fileChangeSignal(fileMap);
}