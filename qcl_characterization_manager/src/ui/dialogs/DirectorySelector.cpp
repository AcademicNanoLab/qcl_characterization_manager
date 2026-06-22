/**
 * @file WizardMeasurementSetupPage.cpp
 * @brief Implementation of WizardMeasurementSetupPage for configuring LIV and Spectra measurement setups.
 *
 * This file defines the WizardMeasurementSetupPage class which constructs the UI page for setting up
 * measurement parameters in pulsed or continuous wave (CW) mode. It provides input fields and dropdowns
 * with validation for hardware selections and graph options, split into LIV and Spectra columns.
 *
 * @author Aleksandar Demic
 */

#include "DirectorySelector.h"
#include <QHBoxLayout>
#include <QDir>
#include <QFormLayout>  // Include this header for QFormLayout
#include "ui/components/buttons/PushButton.h"
#include "ui/components/buttons/ButtonGroupButton.h"
#include "ui/components/buttons/ImageButton.h"
#include "ui/components/text/Text.h"  // Assumed correct class name "Text"

const QString ButtonStyleContainedSecondary = "contained-secondary";

/**
 * @brief Constructs a DirectorySelector widget.
 * 
 * Initializes the UI with a read-only line edit displaying the current directory,
 * a button to choose a directory, and a customizable action button.
 * Connects signals for directory changes and button clicks.
 * 
 * @param dirChangeObject QObject to receive the directory changed signal.
 * @param dirChangeAction Slot to call on directory change.
 * @param buttonText Text label for the action button.
 * @param buttonClickObject QObject to receive the button click signal.
 * @param buttonClickAction Slot to call on button click.
 * @param parent Parent widget.
 */
DirectorySelector::DirectorySelector(QObject *dirChangeObject,
                                     const char *dirChangeAction,
                                     const QString &buttonText,
                                     QObject *buttonClickObject,
                                     const char *buttonClickAction,
                                     QWidget *parent)
    : QWidget(parent),
      chooseDirTextEdit(new QLineEdit(this))
{
    // Setting up layout with a QFormLayout for better label-input pairing
    QFormLayout *layout = new QFormLayout(this);
    setLayout(layout);

    // Create and add the directory label text
    Text *chooseDirText = new Text("Directory:", "body");
    layout->addRow(chooseDirText, chooseDirTextEdit);

    // Set the QLineEdit to read-only
    chooseDirTextEdit->setReadOnly(true);

    // Button to trigger directory selection
    PushButton *chooseDirButton = new PushButton("Choose", ButtonStyleContainedSecondary);
    layout->addRow(chooseDirButton);

    // Button with custom text from the parameter
    PushButton *button = new PushButton(buttonText, ButtonStyleContainedSecondary);
    layout->addRow(button);

    // Connections (we'll ignore them for now as per your request)
    connect(chooseDirButton, SIGNAL(clicked()), this, SLOT(chooseDir()));
    connect(this, SIGNAL(dirChanged(QString)), dirChangeObject, dirChangeAction);
    connect(button, SIGNAL(clicked()), buttonClickObject, buttonClickAction);

    // Set the initial directory to the user's "Documents" folder or current path if not available
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (defaultPath.isEmpty()) {
        defaultPath = QDir::currentPath();  // Fallback if no Documents path
    }
    chooseDirTextEdit->setText(defaultPath);
}

/**
 * @brief Opens a directory selection dialog and updates the displayed directory.
 * 
 * Emits the dirChanged signal if the user selects a valid directory.
 */
void DirectorySelector::chooseDir()
{
    // Use QFileDialog to select a directory
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::currentPath());
    if (!dir.isEmpty()) {
        chooseDirTextEdit->setText(dir);
        emit dirChanged(dir);
    }
}
