/**
 * @file WizardSetupParamsPage.cpp
 * @brief Implementation of the WizardSetupParamsPage class, which creates a setup parameters wizard page.
 * 
 * This page includes fields for author, date, sample name, device name, waveguide options, dimensions, and output directory selection.
 * It provides UI elements for user input and directory browsing with sensible defaults.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "WizardSetupParamsPage.h"
#include <QLabel>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QStandardPaths>
#include <QDir>

// Include the widget header files
#include "ui/components/wizard/WizardPage.h"
#include "ui/components/wizard/WizardFieldWidget.h"
#include "ui/components/wizard/WizardTextFieldWidget.h"
#include "ui/components/wizard/WizardDimensionFieldWidget.h"
#include "ui/components/wizard/WizardRadioFieldWidget.h"

/**
 * @brief Constructs a WizardSetupParamsPage widget.
 * 
 * Sets up all form fields and UI elements, including text fields, radio buttons, and a directory selection widget.
 * The output directory defaults to the desktop path or a fallback if unavailable.
 * Connects the browse button to open a directory selection dialog.
 * 
 * @param parent Optional parent widget.
 */
WizardSetupParamsPage::WizardSetupParamsPage(QWidget *parent)
    : WizardPage{"Setup Parameters", parent}
{
    // Add Author, Date, and Sample Name in the correct order
    addWidget(new WizardTextFieldWidget(this, "Author", WizardTextFieldWidget::BoxLayout::HLayout));
    addWidget(new WizardTextFieldWidget(this, "Date", WizardTextFieldWidget::BoxLayout::HLayout, true)); // Set as date field
    addWidget(new WizardTextFieldWidget(this, "Sample Name", WizardTextFieldWidget::BoxLayout::HLayout));
    addWidget(new WizardTextFieldWidget(this, "Device Name", WizardTextFieldWidget::BoxLayout::HLayout));
    addWidget(new WizardRadioFieldWidget(this, "Waveguide", {"SM", "DM"}, WizardTextFieldWidget::BoxLayout::HLayout));
    addWidget(new WizardDimensionFieldWidget(this, "Dimensions"));

    // --- Directory Selection Widget ---
    QWidget *dirWidget = new QWidget(this);
    QHBoxLayout *dirLayout = new QHBoxLayout(dirWidget);

    QLabel *label = new QLabel("Output Directory", this);
    label->setStyleSheet("QLabel { font: bold 16px; color: black; }");

    dirLineEdit = new QLineEdit(this);
    dirLineEdit->setPlaceholderText("Select a directory...");

    // Get Desktop path (with fallback)
    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    if (desktopDir.isEmpty() || !QDir(desktopDir).exists()) {
        desktopDir = "C:/Users/LXD/Desktop";  // Fallback
    }

    // Set the default text to desktopDir permanently until user changes it
    dirLineEdit->setText(desktopDir);

    dirLineEdit->setStyleSheet(
        "QLineEdit {"
        "    font: bold 13px;"
        "    padding: 10px;"
        "    border: 2px solid #bbb;"
        "    border-radius: 5px;"
        "    background-color: white;"
        "    color: black;"
        "    transition: border-color 0.3s ease, background-color 0.3s ease;"
        "}"
        "QLineEdit:focus {"
        "    border-color: primaryLight;"
        "    background-color: #f9f9f9;"
        "}"
        "QLineEdit:hover {"
        "    border-color: primaryHover;"
        "    background-color: #f5f5f5;"
        "}"
    );

    QPushButton *browseButton = new QPushButton("Browse", this);

    dirLayout->addWidget(label);
    dirLayout->addWidget(dirLineEdit);
    dirLayout->addWidget(browseButton);
    dirWidget->setLayout(dirLayout);
    addWidget(dirWidget);

    connect(browseButton, &QPushButton::clicked, this, [this, desktopDir]() {
        QString currentDir = dirLineEdit->text();
        if (currentDir.isEmpty() || !QDir(currentDir).exists()) {
            currentDir = desktopDir;  // fallback to desktop if empty or invalid
        }
        QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory", currentDir);
        if (!dir.isEmpty()) {
            dirLineEdit->setText(dir);
        }
    });
}

/**
 * @brief Returns the currently selected output directory.
 * 
 * Retrieves the text from the directory line edit widget, representing the user-selected output directory.
 * 
 * @return QString containing the output directory path.
 */
QString WizardSetupParamsPage::getOutputDirectory() const 
{
    return dirLineEdit->text(); // Return the text from the directory line edit
}