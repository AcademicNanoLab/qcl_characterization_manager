/**
 * @file WizardFileFieldWidgetFile.cpp
 * @brief Implementation of the WizardFileFieldWidgetFile class, representing individual file entries in the file field widget.
 * 
 * This class displays a single file's name, allows editing an associated variable value,
 * and provides the ability to delete the file entry.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#include "WizardFileFieldWidgetFile.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDir>
#include "ui/components/buttons/PushButton.h"
#include "DoubleLineEdit.h"

/**
 * @brief Constructs a file widget displaying the file name and associated variable input.
 *
 * Initializes the widget with the file's name, a text input for the associated variable,
 * and a delete button.
 *
 * @param fileNameText Full path or name of the file.
 * @param fileVariableName Name of the variable associated with this file.
 * @param parent Optional parent widget.
 */
WizardFileFieldWidgetFile::WizardFileFieldWidgetFile(const QString &fileNameText,
                                                     const QString &fileVariableName,
                                                     QWidget *parent)
    : Widget{parent}
    , fileName(fileNameText)
    , fileVariableName(fileVariableName)
    , fileVariableValue("")
{
    QHBoxLayout *layout = new QHBoxLayout();
    setLayout(layout);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    layout->setAlignment(Qt::AlignLeft);

    // Display file name
    Text *fileNameWidget = new Text(parseFileName(fileNameText), "h3");
    fileNameWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    layout->addWidget(fileNameWidget);

    // Text field for the variable value
    DoubleLineEdit *fileVariableValueEdit = new DoubleLineEdit(fileVariableName);
    fileVariableValueEdit->setPlaceholderText("Enter " + fileVariableName);
    fileVariableValueEdit->setFixedWidth(150);
    layout->addWidget(fileVariableValueEdit);

    // Connect text change to custom signal
	connect(fileVariableValueEdit,
			&DoubleLineEdit::_textChanged,
			this,
			[this](const QString &, const QString &value) {
				this->textChange(value);
			});

    // Close button
    PushButton *closeButton = new PushButton(QIcon(":/src/resources/images/close-button.svg"));
    closeButton->setFixedSize(30, 30);
    layout->addWidget(closeButton);

    // Connect the delete button
    connect(closeButton, &PushButton::clicked, this, &WizardFileFieldWidgetFile::onDeleteButtonClicked);
}

/**
 * @brief Extracts and returns the file name from a full file path.
 *
 * Converts the path separators to the native platform style and returns
 * only the file name portion.
 *
 * @param fileName The full file path string.
 * @return The extracted file name with native separators.
 */
QString WizardFileFieldWidgetFile::parseFileName(const QString &fileName)
{
    // Ensure platform compatibility for file path separators
    return QDir::toNativeSeparators(fileName).split(QDir::separator()).last();
}