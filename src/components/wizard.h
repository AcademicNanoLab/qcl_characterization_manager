#ifndef WIZARD_H
#define WIZARD_H

#include <QStackedWidget>
#include "components/buttongroup.h"
#include "components/widget.h"
#include "components/wizardpage.h"

// Simplified and customized widget based on QWizard.
// Add new pages using addPage().
class Wizard : public Widget
{
    Q_OBJECT
public:
    explicit Wizard(QWidget *parent = nullptr);
    void addPage(WizardPage *page);
    void getWizardFields();
    void finishWizard();
    virtual void finishWizardAction() = 0;
    QStackedWidget *wizardPages;

signals:
    void finished(int mainStackIdx);

public slots:
    void setCurrentIndexWrapper(int id);
    void wizardButtonsClick(int id);

private:
    ButtonGroup *wizardMenu;
    ButtonGroup *wizardButtons;
};

#endif // WIZARD_H
