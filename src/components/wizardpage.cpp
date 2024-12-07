#include "wizardpage.h"
#include "wizardfield.h"
#include "wizardfieldwidget.h"

WizardPage::WizardPage(const QString &title, QWidget *parent)
    : HeaderPage{title, parent}
    , wizardFieldList({})
{}

// Add new field
void WizardPage::addField(WizardField *wizardField)
{
    wizardFieldList.append(wizardField);
}

// Get wizard fields as string
void WizardPage::getWizardFields(QString &wizardFieldString)
{
    for (WizardField *wizardField : wizardFieldList)
        wizardField->getWizardField(wizardFieldString);
}

// Check if we can move on to next page.
bool WizardPage::pageValid()
{
    for (WizardField *wizardField : wizardFieldList)
        if (!wizardField->fieldValid())
            return false;
    return true;
}

// Clear all the wizard fields in the page.
void WizardPage::clear()
{
    for (WizardFieldWidget *wizardFieldWidget : this->findChildren<WizardFieldWidget *>())
        wizardFieldWidget->clear();
}

// Write all the fields into the text file.
// This function is only called for an open file pointer.
void WizardPage::writeToFile(QFile *file)
{
    QString wizardFieldString;
    for (WizardField *wizardField : wizardFieldList)
        wizardField->getWizardField(wizardFieldString);

    QTextStream out(file);
    out << wizardFieldString;
}

// Write all the fields into the map.
void WizardPage::addToMap(QVariantMap &map)
{
    for (WizardField *wizardField : wizardFieldList)
        wizardField->addToMap(map);
}

/**************************************** WizardFilePage *****************************************/
WizardFilePage::WizardFilePage(const QString &title,
                               const QString &fileVariableName,
                               QWidget *parent)
    : WizardPage(title, parent)
{
    addWidget(new WizardFileFieldWidget(this, title, fileVariableName));
}
