/**
 * @file WizardDimensionFieldWidget.cpp
 * @brief Implementation of a wizard field widget for inputting 3D dimensions.
 * 
 * This widget provides three aligned DoubleLineEdit inputs for length, width, and height,
 * each labeled with their respective units. It supports input validation for double values,
 * styling consistent with other wizard fields, and emits change signals for each dimension.
 * 
 * Dimensions are laid out horizontally with multiplication symbols between fields.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#include "WizardDimensionFieldWidget.h"
#include "ui/components/text/Text.h"

/**
 * @brief Constructs a WizardDimensionFieldWidget.
 * 
 * Initializes three DoubleLineEdit fields for length, width, and height with units.
 * Each input is styled and connected to update signals.
 * The inputs are arranged horizontally with multiplication signs between them.
 * 
 * @param page The parent WizardPage.
 * @param name The identifier name for this widget.
 * @param parent The parent QWidget.
 */
WizardDimensionFieldWidget::WizardDimensionFieldWidget(WizardPage *page,
                                                       const QString &name,
                                                       QWidget *parent)
    : WizardFieldWidget(page,
                        name,
                        WizardField::WizardFieldType::DimensionField,
                        WizardFieldWidget::BoxLayout::HLayout,
                        parent)
    , textFields({})
{
    QGridLayout *fieldLayout = new QGridLayout();
    fieldLayout->setHorizontalSpacing(10);  // Adjust horizontal spacing
    fieldLayout->setVerticalSpacing(10);    // Adjust vertical spacing

    // Fixed the encoding issues in the dimension map
    QList<QList<QString>> dimensionMap = {{"length", "mm"}, {"width", "µm"}, {"height", "µm"}};

    // Loop to create the labels and input fields
    for (int i = 0; i < 3; i++) {
        QString &key = dimensionMap[i][0];

        // Create label for the dimension (length, width, height)
        Text *label = new Text(key + "(" + dimensionMap[i][1] + ")", "body");
        label->setAlignment(Qt::AlignCenter);

        // Create input field (DoubleLineEdit) for the dimension
        DoubleLineEdit *textField = new DoubleLineEdit(key);
        textField->setPlaceholderText("Enter " + key);
        textField->setAlignment(Qt::AlignCenter);

        // Style the text field (same as WizardTextFieldWidget)
        textField->setStyleSheet("QLineEdit {"
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
                                 "}");

        // Add the label and text field to the layout
        fieldLayout->addWidget(label, 0, i * 2);  // Place labels in the first row
        fieldLayout->addWidget(textField, 1, i * 2); // Place text fields in the second row

        // Connect the signal to the slot
        connect(textField,
                &DoubleLineEdit::_textChanged,
                this,
                &WizardDimensionFieldWidget::onDimensionTextChanged);

        // Initialize the text field with an empty value
        emit textField->_textChanged(key, "");

        // Add multiplication symbol if it's not the last dimension
        if (i < 2) {
            Text *multiplier = new Text("×", "body");
            multiplier->setAlignment(Qt::AlignCenter);
            fieldLayout->addWidget(multiplier, 1, i * 2 + 1); // Between text fields
        }

        // Store the text field for future reference
        textFields.append(textField);
    }

    // Add the layout to the widget
    addLayout(fieldLayout);
}