/**
 * @file WizardTextFieldWidget.cpp
 * @brief Implementation of a wizard text field widget supporting both string and date input.
 * 
 * This widget extends WizardFieldWidget to provide a text input field or a date selector,
 * based on initialization parameters. It handles input styling, validation states,
 * and emits change signals when the input value is modified.
 * 
 * The widget supports both horizontal and vertical layouts for flexible UI integration.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#include "WizardTextFieldWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QTimer>

/**
 * @brief Constructs a WizardTextFieldWidget.
 * 
 * Depending on the `isDateField` flag, initializes either a QLineEdit for text input
 * or a QDateEdit for date input with custom styling and behavior.
 * Connects input changes to signals to notify the parent wizard page.
 * Supports horizontal or vertical layout arrangement.
 * 
 * @param page Pointer to the parent WizardPage.
 * @param name The name identifier for this field.
 * @param boxLayout Layout type (horizontal or vertical).
 * @param isDateField Flag indicating if the field should be a date selector.
 * @param parent The parent QWidget.
 */
WizardTextFieldWidget::WizardTextFieldWidget(WizardPage *page,
                                             const QString &name,
                                             BoxLayout boxLayout,
                                             bool isDateField,
                                             QWidget *parent)
    : WizardFieldWidget(page, name, WizardField::WizardFieldType::StringField, boxLayout, parent)
    , isDateField(isDateField)
{
    if (isDateField) {
        dateField = new QDateEdit(this);
        dateField->setDisplayFormat("dd-MM-yyyy");
        dateField->setDate(QDate::currentDate());
        dateField->setCalendarPopup(true);
        dateField->setButtonSymbols(QAbstractSpinBox::NoButtons);

        dateField->setStyleSheet(
            "QDateEdit {"
            "  font: bold 13px;"
            "  padding: 10px;"
            "  border: 2px solid #bbb;"
            "  border-radius: 5px;"
            "  background-color: white;"
            "  color: black;"
            "}"
            "QDateEdit:focus {"
            "  border-color: #3399ff;"
            "  background-color: #f9f9f9;"
            "}"
            "QDateEdit:hover {"
            "  border-color: #66aaff;"
            "  background-color: #f5f5f5;"
            "}"
        );

        fieldArea->layout()->addWidget(dateField);

        connect(dateField, &QDateEdit::dateChanged, this, [this]() {
            emit changeSignal(value());
            setErrorState(ErrorState::NoError);
        });

        //  Notify wizard that default value is already set
        QTimer::singleShot(0, this, [this]() {
            emit changeSignal(value());
        });

    } else {
        textField = new QLineEdit(this);
        textField->setPlaceholderText("Enter value");
        textField->setStyleSheet(
            "QLineEdit {"
            "  font: bold 13px;"
            "  padding: 10px;"
            "  border: 2px solid #bbb;"
            "  border-radius: 5px;"
            "  background-color: white;"
            "  color: black;"
            "}"
            "QLineEdit:focus {"
            "  border-color: #3399ff;"
            "  background-color: #f9f9f9;"
            "}"
            "QLineEdit:hover {"
            "  border-color: #66aaff;"
            "  background-color: #f5f5f5;"
            "}"
        );

        fieldArea->layout()->addWidget(textField);

        connect(textField, &QLineEdit::textChanged, this, [this]() {
            emit changeSignal(value());
            setErrorState(ErrorState::NoError);
        });
    }

    if (boxLayout == BoxLayout::HLayout) {
        QWidget *spacer = new QWidget(this);
        spacer->setFixedWidth(10);
        fieldArea->layout()->addWidget(spacer);
    }
}

/**
 * @brief Clears the content of the input field.
 * 
 * Resets the QDateEdit to the current date if it's a date field,
 * or clears the QLineEdit text if it's a string field.
 */
void WizardTextFieldWidget::clear()
{
    if (isDateField) {
        dateField->setDate(QDate::currentDate());
    } else {
        textField->clear();
    }
}

/**
 * @brief Retrieves the current value of the field.
 * 
 * Returns the selected date as a formatted string ("dd-MM-yyyy") if it's a date field,
 * otherwise returns the text entered in the line edit.
 * 
 * @return QVariant containing the current value.
 */
QVariant WizardTextFieldWidget::value() const
{
    if (isDateField) {
        return dateField->date().toString("dd-MM-yyyy");
    } else {
        return textField->text();
    }
}

/**
 * @brief Validates the current input in the field.
 * 
 * Checks if the date is valid when in date mode, or that the text is non-empty
 * when in string mode.
 * 
 * @return True if the current input is considered valid, false otherwise.
 */
bool WizardTextFieldWidget::isValid() const
{
    if (isDateField) {
        return dateField->date().isValid();
    } else {
        return !textField->text().trimmed().isEmpty();
    }
}