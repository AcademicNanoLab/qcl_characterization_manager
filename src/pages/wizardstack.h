#ifndef WIZARDSTACK_H
#define WIZARDSTACK_H

#include "components/wizard.h"

class WizardStack : public Wizard
{
    Q_OBJECT
public:
    explicit WizardStack(QWidget *parent = nullptr);
    void finishWizardAction();

signals:
    void sendFields(const QVariantMap &map);

public slots:
    void currentChangedSlot(int id);
};

#endif // WIZARDSTACK_H
