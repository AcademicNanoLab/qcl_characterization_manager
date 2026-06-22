/**
 * @file WizardPage.cpp
 * @brief Implementation of the WizardPage class managing a collection of wizard fields.
 * 
 * This class represents a single page in the wizard interface,
 * containing multiple WizardField objects. It provides functionality to
 * add fields, validate the page, clear fields, serialize field data, and
 * populate a QVariantMap with field values.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#include "WizardPage.h"
#include "WizardField.h"
#include "WizardFieldWidget.h"

/**
 * @brief Constructs a WizardPage with the given title and optional parent widget.
 * @param title The title of the wizard page.
 * @param parent Optional parent widget.
 */
WizardPage::WizardPage(const QString &title, QWidget *parent)
    : HeaderPage(title, parent)
    , wizardFieldList({})
{}

/**
 * @brief Adds a WizardField to the page.
 * @param wizardField Pointer to the WizardField to add.
 */
void WizardPage::addField(WizardField *wizardField)
{
    wizardFieldList.append(wizardField);
}

/**
 * @brief Appends string representations of all wizard fields into the provided QString.
 * @param wizardFieldString The string to append wizard field data to.
 */
void WizardPage::getWizardFields(QString &wizardFieldString)
{
    for (WizardField *wizardField : wizardFieldList)
        wizardField->getWizardField(wizardFieldString);
}

/**
 * @brief Checks if all fields in the page are valid.
 * @return True if all fields are valid, otherwise false.
 */
bool WizardPage::pageValid()
{
    for (WizardField *wizardField : wizardFieldList)
        if (!wizardField->fieldValid())
            return false;
    return true;
}

/**
 * @brief Clears all WizardFieldWidgets contained within this page.
 */
void WizardPage::clear()
{
    for (WizardFieldWidget *wizardFieldWidget : this->findChildren<WizardFieldWidget *>())
        wizardFieldWidget->clear();
}

/**
 * @brief Writes all wizard field data to the given QFile.
 * @param file The file to write data to.
 */
void WizardPage::writeToFile(QFile *file)
{
    QString wizardFieldString;
    for (WizardField *wizardField : wizardFieldList)
        wizardField->getWizardField(wizardFieldString);

    QTextStream out(file);
    out << wizardFieldString;
}

/**
 * @brief Adds all wizard field data to the given QVariantMap.
 * @param map The map to populate with field data.
 */
void WizardPage::addToMap(QVariantMap &map)
{
    for (WizardField *wizardField : wizardFieldList)
        wizardField->addToMap(map);
}