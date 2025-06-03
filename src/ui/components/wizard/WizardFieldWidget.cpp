/**
 * @file WizardFieldWidget.cpp
 * @brief Implementation of the WizardFieldWidget class which provides UI elements for wizard fields.
 * 
 * This class encapsulates the user interface components for wizard fields,
 * including error display, labels, and input areas with flexible layouts.
 * It manages error states, field display, and interaction within a WizardPage.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "WizardFieldWidget.h"
#include <QLineEdit>
#include <QRadioButton>
#include <QRegularExpressionValidator>
#include <QScrollBar>
#include <QStandardPaths>
#include "ui/components/buttons/ButtonGroup.h"

/**
 * @brief Constructs a WizardFieldWidget without specifying a layout.
 * 
 * Initializes the field, error display area, and default layout.
 * Associates the widget with the given WizardPage and sets up
 * the error state area but hides it initially.
 * 
 * @param page The WizardPage this widget belongs to.
 * @param name The name of the field.
 * @param wizardFieldType The type of the wizard field.
 * @param parent The parent widget (optional).
 */
WizardFieldWidget::WizardFieldWidget(WizardPage *page,
                                     const QString &name,
                                     WizardField::WizardFieldType wizardFieldType,
                                     QWidget *parent)
    : Widget{parent}
    , field(new WizardField(name, wizardFieldType, this, SIGNAL(changeSignal(QVariant))))
    , fieldArea(new Widget())
    , errorArea(new Widget())
    , errorAreaText(new Text("", "error"))
    , errorState(ErrorState::NoError)
{
    // wizardField
    page->addField(field);

    // setting up layout
    setLayout(new QVBoxLayout());
    layout()->setAlignment(Qt::AlignTop);
    layout()->addWidget(fieldArea);
    layout()->addWidget(errorArea);
    layout()->setContentsMargins(25, 30, 25, 30);

    // setting up errorArea
    errorArea->setLayout(new QHBoxLayout());
    errorArea->layout()->setAlignment(Qt::AlignLeft);
    errorArea->layout()->setContentsMargins(0, 0, 0, 0);
    errorArea->hide();

    // errorAreaText
    errorAreaText->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
}

/**
 * @brief Constructs a WizardFieldWidget with a specified box layout.
 * 
 * Sets up the field area with either a vertical or horizontal layout,
 * adds a label with the field name, and configures the error display area.
 * Calls the basic constructor for common initialization.
 * 
 * @param page The WizardPage this widget belongs to.
 * @param name The name of the field.
 * @param wizardFieldType The type of the wizard field.
 * @param boxLayout The layout style (horizontal or vertical).
 * @param parent The parent widget (optional).
 */
WizardFieldWidget::WizardFieldWidget(WizardPage *page,
                                     const QString &name,
                                     WizardField::WizardFieldType wizardFieldType,
                                     WizardFieldWidget::BoxLayout boxLayout,
                                     QWidget *parent)
    : WizardFieldWidget{page, name, wizardFieldType, parent}
{
    if (boxLayout == BoxLayout::VLayout) {
        fieldArea->setLayout(new QVBoxLayout());
        fieldArea->layout()->setAlignment(Qt::AlignTop);
    } else {
        fieldArea->setLayout(new QHBoxLayout());
        fieldArea->layout()->setAlignment(Qt::AlignLeft);
    }
    fieldArea->layout()->setContentsMargins(0, 0, 0, 0);

    // label
    Text *label = new Text(name, "h3");
    if (boxLayout == BoxLayout::HLayout)
        label->setFixedWidth(120);
    fieldArea->layout()->addWidget(label);

    // spacer
    if (boxLayout == BoxLayout::HLayout) {
        Text *label = new Text("", "h3");
        label->setFixedWidth(120);
        errorArea->layout()->addWidget(label);
    }
    errorArea->layout()->addWidget(errorAreaText);
}

/**
 * @brief Sets the error state and updates the error display area.
 * 
 * Shows or hides the error area depending on the error state,
 * and updates the error message text accordingly.
 * 
 * @param _errorState The error state to set.
 */
void WizardFieldWidget::setErrorState(ErrorState _errorState)
{
    errorState = _errorState;
    if (errorState == ErrorState::NoError)
        errorArea->hide();
    else
        errorArea->show();

    switch (errorState) {
    case (ErrorState::NoError):
        break;
    case (ErrorState::StringEmpty):
        errorAreaText->setText("Please input something");
        break;
    case (ErrorState::DoubleLimit):
        errorAreaText->setText("Please input numbers in the range 0~10000");
        break;
    case (ErrorState::MapError):
        errorAreaText->setText("Please check your input");
        break;
    }
}

/**
 * @brief Adds a layout to the field area.
 * 
 * This method appends the given layout to the current field area layout.
 * 
 * @param _layout The layout to add.
 */
void WizardFieldWidget::addLayout(QLayout *_layout)
{
    ((QHBoxLayout *) fieldArea->layout())->addLayout(_layout);
}

/**
 * @brief Adds a widget to the field area.
 * 
 * Appends the given widget to the current layout within the field area.
 * 
 * @param widget The widget to add.
 */
void WizardFieldWidget::addWidget(QWidget *widget)
{
    fieldArea->layout()->addWidget(widget);
}