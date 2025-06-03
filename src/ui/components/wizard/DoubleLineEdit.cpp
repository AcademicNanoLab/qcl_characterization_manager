/**
 * @file DoubleLineEdit.cpp
 * @brief Implementation of a QLineEdit subclass that validates and emits changes for double values.
 * 
 * This widget restricts input to valid double-precision floating-point numbers using a regular expression validator.
 * It emits a custom signal `_textChanged` with the widget's name and current text whenever the text changes.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "DoubleLineEdit.h"
#include <QRegularExpressionValidator>

/**
 * @brief Constructs a DoubleLineEdit with a given name and optional parent.
 * 
 * Sets up input validation to accept valid double values (including optional sign and decimals).
 * Connects the textChanged signal to an internal slot that emits a custom signal with the widget name.
 * 
 * @param name The identifier name for this line edit.
 * @param parent The parent widget.
 */
DoubleLineEdit::DoubleLineEdit(const QString &name, QWidget *parent)
    : QLineEdit(parent), _name(name)
{
    // More flexible regex to allow optional negative, decimals, and no leading digits
    static const QRegularExpression doubleRx(R"(^-?\d*(\.\d+)?$)");
    this->setValidator(new QRegularExpressionValidator(doubleRx, this));

    connect(this, &DoubleLineEdit::textChanged,
            this, &DoubleLineEdit::textChangedSlot);
}

/**
 * @brief Slot triggered on text changes.
 * 
 * Emits the `_textChanged` signal passing the widget's name and the new text value.
 * 
 * @param value The new text content of the line edit.
 */
void DoubleLineEdit::textChangedSlot(const QString &value)
{
    emit _textChanged(_name, value);  
}