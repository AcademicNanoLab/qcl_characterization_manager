#include "setupparamspage.h"
#include <QLabel>
#include <QRegularExpression>
#include "components/wizardfieldwidget.h"

SetupParamsPage::SetupParamsPage(QWidget *parent)
    : WizardPage{"Setup Parameters", parent}
{
    addWidget(
        new WizardTextFieldWidget(this, "Sample Name", WizardTextFieldWidget::BoxLayout::HLayout));
    addWidget(
        new WizardTextFieldWidget(this, "Device Name", WizardTextFieldWidget::BoxLayout::HLayout));
    addWidget(new WizardRadioFieldWidget(this,
                                         "Waveguide",
                                         {"SM", "DM"},
                                         WizardTextFieldWidget::BoxLayout::HLayout));
    addWidget(new WizardDimensionFieldWidget(this, "Dimensions"));
}
