#include "wizardstack.h"
#include "components/messagebox.h"
#include "components/wizardpage.h"
#include "wizardPages/setupparamspage.h"
#include "wizardPages/tmppage.h"

WizardStack::WizardStack(QWidget *parent)
    : Wizard{parent}
{
    addPage(new SetupParamsPage());
    addPage(new WizardFilePage("Pulse LIV", "Temperature (K)"));
    addPage(new WizardFilePage("Pulse FDR - fixed temperature", "Current (mA)"));
    addPage(new WizardFilePage("Pulse FDR - fixed current", "Temperature (K)"));
    addPage(new WizardFilePage("CW LIV", "Temperature (K)"));
    addPage(new WizardFilePage("CW FDR - fixed temperature", "Current (mA)"));
    addPage(new WizardFilePage("CW FDR - fixed current", "Temperature (K)"));

    // connect reaching "CW LIV" page with skip message
    connect(wizardPages, &QStackedWidget::currentChanged, this, &WizardStack::currentChangedSlot);
}

void WizardStack::currentChangedSlot(int id)
{
    if (id != 4)
        return;
    MessageBox *msgBox = new MessageBox(QMessageBox::Information,
                                        "",
                                        "You don't need to fill all CW files.",
                                        QMessageBox::Ok | QMessageBox::Cancel);
    msgBox->setInformativeText("Would you like to skip to the end?");

    // execute
    int ret = msgBox->exec();

    switch (ret) {
    case QMessageBox::Ok:
        finishWizard();
    }
}

// This function writes the variables into the file + sends variables to analysis page
void WizardStack::finishWizardAction()
{
    QFile file("result.txt");
    if (file.open(QIODevice::WriteOnly)) {
        // setup params pages first
        for (SetupParamsPage *page : wizardPages->findChildren<SetupParamsPage *>())
            page->writeToFile(&file);
        for (WizardFilePage *page : wizardPages->findChildren<WizardFilePage *>())
            page->writeToFile(&file);
    }
    file.close();

    // sending variables
    QVariantMap map;
    for (WizardPage *page : wizardPages->findChildren<WizardPage *>())
        page->addToMap(map);
    emit sendFields(map);
}
