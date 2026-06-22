/**
 * @file WizardRadioFieldWidget.cpp
 * @brief Implementation of the WizardRadioFieldWidget class representing a group of radio buttons in a wizard field.
 * 
 * This widget displays multiple radio button options for a single-choice field in a wizard.
 * It connects user interaction signals to the underlying WizardField logic.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#include "WizardRadioFieldWidget.h"
#include <QRadioButton>

/**
 * @brief Constructs a WizardRadioFieldWidget with given parameters.
 * 
 * Creates a radio button group with the specified options and layout, 
 * adds it to the wizard page, and connects signals to handle user interaction.
 * 
 * @param page Pointer to the WizardPage that contains this widget.
 * @param name The name of the wizard field.
 * @param optionsList List of string options for the radio buttons.
 * @param boxLayout Layout type for arranging radio buttons (horizontal or vertical).
 * @param parent Optional parent QWidget.
 */
WizardRadioFieldWidget::WizardRadioFieldWidget(WizardPage *page,
                                               const QString &name,
                                               QList<QString> optionsList,
                                               const BoxLayout boxLayout,
                                               QWidget *parent)
    : WizardFieldWidget(page, name, WizardField::WizardFieldType::StringField, boxLayout, parent)
{
    // Create the ButtonGroup with the specified layout
    radioButtonGroup = new ButtonGroup(ButtonGroup::BoxLayout::HLayout);
    radioButtonGroup->layout()->setContentsMargins(0, 0, 0, 0);

    // Add radio buttons for each option
    for (const QString &option : optionsList)
    {
        QRadioButton *radioButton = new QRadioButton(option);
        radioButtonGroup->addButton(radioButton);
    }

    // Add the radioButtonGroup to the widget
    addWidget(radioButtonGroup);

    // Connect the button click signal to the slot in WizardFieldWidget
    connect(radioButtonGroup,
            &ButtonGroup::buttonClickedText,
            this,
            &WizardFieldWidget::changeSignalSlot);

    // Initialize the button group
    radioButtonGroup->initButton();
}