#ifndef WIZARDTEXTFIELDWIDGET_H
#define WIZARDTEXTFIELDWIDGET_H

#include <QLineEdit>
#include <QDateEdit>
#include "WizardFieldWidget.h"

// Widget for text or date input.
class WizardTextFieldWidget : public WizardFieldWidget
{
    Q_OBJECT
public:
    explicit WizardTextFieldWidget(WizardPage *page,
                                   const QString &name,
                                   BoxLayout boxLayout,
                                   bool isDateField = false,
                                   QWidget *parent = nullptr);

    void clear() override;

    QVariant value() const;
    bool isValid() const;

private:
    bool isDateField;
    QLineEdit *textField = nullptr;
    QDateEdit *dateField = nullptr;
};

#endif // WIZARDTEXTFIELDWIDGET_H
