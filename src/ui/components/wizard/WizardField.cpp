/**
 * @file WizardField.cpp
 * @brief Implementation of the WizardField class managing form field data and interactions.
 * 
 * This class represents a generic wizard field, tracking its value and managing signal connections.
 * It handles value changes and optional file field-specific functionality.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#include "WizardField.h"
#include <QAbstractButton>
#include "ui/dialogs/MessageBox.h"
#include "WizardFieldWidget.h"

/**
 * @brief Displays a warning when no file is selected.
 * 
 * This method is currently stubbed to always return true without showing a message box.
 * It is intended to be used to prompt the user about missing file selection.
 * 
 * @return true to continue operation by default.
 */
bool WizardField::noFileSelected()
{
    // No message box to annoy the user by default
    /*
    MessageBox *msgBox = new MessageBox(QMessageBox::Warning,
                                        "",
                                        "You haven't chosen any files.",
                                        QMessageBox::Ok | QMessageBox::Cancel);
    msgBox->setInformativeText("Would you like to continue?");

    // execute the message box
    int ret = msgBox->exec();

    switch (ret) {
    case QMessageBox::Ok:
        return true;  // If user clicks OK, continue
    case QMessageBox::Cancel:
        return false; // If user clicks Cancel, halt the operation
    }
    */
    return true;  // Default return value for now, to continue without message box
}

/**
 * @brief Constructs a WizardField object.
 * 
 * Connects the provided object's change signal to the WizardField's slot for updating the value.
 * Initializes the field with an empty default value.
 * 
 * @param name The field's identifier.
 * @param wizardFieldType The type of the wizard field.
 * @param object The QObject providing the change signal.
 * @param changedSignal The signal emitted by the object on field change.
 */
 WizardField::WizardField(const QString &name,
                         WizardFieldType wizardFieldType,
                         const QObject *object,
                         const char *changedSignal)
    : QObject{}
    , name(name)
    , value(QVariant())
    , wizardFieldType(wizardFieldType)
    , widget((WizardFieldWidget *) object)
{
    // connect field change signal
    connect(object, changedSignal, this, SLOT(fieldChange(QVariant)));

    // initialize field state
    fieldChange(""); // Default empty value initialization
}

/**
 * @brief Connects file-specific change signals to the WizardField.
 * 
 * This method sets up connection for file-related QVariantMap changes and stores the associated variable name.
 * 
 * @param object The QObject emitting file change signals.
 * @param changedSignal The signal to connect for file changes.
 * @param _fileVariableName The name of the file variable associated with this field.
 */
 void WizardField::connectFileField(const QObject *object,
                                   const char *changedSignal,
                                   const QString &_fileVariableName)
{
    // connect field change signal for file fields
    connect(object, changedSignal, this, SLOT(fieldChange(QVariantMap)));
    fileVariableName = _fileVariableName;
}

/**
 * @brief Slot to update the field value on changes.
 * 
 * Called when the connected object's signal is emitted. Updates the internal value and logs the change.
 * 
 * @param _value The new value of the field.
 */
 void WizardField::fieldChange(const QVariant &_value)
{
	    qDebug() << "WizardField change called for" << name << "with value:" << _value;

    value = _value;
}

/**
 * @brief Updates the field value for file fields.
 * 
 * This function is only called for file fields and updates the internal value
 * using the provided QVariantMap.
 * 
 * @param _value The QVariantMap containing file field data.
 */
void WizardField::fieldChange(const QVariantMap &_value)
{
    value = _value;
}

/**
 * @brief Appends the string representation of the wizard field to the given string.
 * 
 * Formats the field name and value(s) based on the field type and appends
 * the result to @p wizardFieldString.
 * 
 * @param wizardFieldString The string to which the formatted field is appended.
 */
void WizardField::getWizardField(QString &wizardFieldString)
{
    QString printName = name;
    while (printName.contains(' '))
        printName.replace(' ', '_');
    wizardFieldString += printName;

    switch (wizardFieldType) 
	{
		case (WizardFieldType::StringField):
		case (WizardFieldType::DoubleField):
			wizardFieldString += " " + value.toString() + "\n";
			break;
		case (WizardFieldType::DimensionField):
			for (auto [key, value] : reinterpret_cast<QVariantMap *>(&this->value)->asKeyValueRange())
				wizardFieldString += " " + value.toString();
			wizardFieldString += "\n";
			break;
		case (WizardFieldType::FileField):
		{
			wizardFieldString += " " + fileVariableName + "\n";

			QVariantMap* map = reinterpret_cast<QVariantMap*>(&this->value);

			// Calculate max length of keys (file paths)
			int maxPathLen = 0;
			for (auto [key, val] : map->asKeyValueRange()) {
				if (key.length() > maxPathLen)
					maxPathLen = key.length();
			}

			// Write each path padded to max length + one space, then number
			for (auto [key, val] : map->asKeyValueRange()) {
				QString paddedPath = key.leftJustified(maxPathLen, ' ');
				wizardFieldString += paddedPath + " " + val.toString() + "\n";
			}
			break;
		}
    }
}

/**
 * @brief Adds the wizard field's value to a QVariantMap if valid.
 * 
 * Checks if the field's value is valid and non-empty, and if so, inserts
 * it into the provided map. Skips empty fields.
 * 
 * @param map The QVariantMap to add the field's value to.
 */
void WizardField::addToMap(QVariantMap &map)
{
    bool flag = false;

    qDebug() << "Adding field:" << name << "with value:" << value; // Add debug log here

    switch (wizardFieldType) {
    case (WizardFieldType::StringField):
    case (WizardFieldType::DoubleField):
        if (!value.toString().isEmpty())
            flag = true;
        break;
    case (WizardFieldType::DimensionField):
    case (WizardFieldType::FileField):
        if (!reinterpret_cast<QVariantMap *>(&value)->isEmpty())
            flag = true;
        break;
    }

    if (flag)
        map[name] = value;
    else
        qDebug() << "Skipping empty field:" << name; // Debug log for skipped fields
}

/**
 * @brief Checks whether the field value is valid.
 * 
 * Performs validity checks based on the type of the wizard field,
 * and updates the widget's error state accordingly.
 * 
 * @return true if the field value is valid, false otherwise.
 */
bool WizardField::fieldValid()
{
    switch (wizardFieldType) {
    case (WizardFieldType::StringField):
        return fieldValid(*reinterpret_cast<QString *>(&value));
    case (WizardFieldType::DoubleField):
        return fieldValid(value.toDouble());
    case (WizardFieldType::DimensionField):
        return fieldValid(*reinterpret_cast<QVariantMap *>(&value));
    case (WizardFieldType::FileField):
        return fieldValid(*reinterpret_cast<QVariantMap *>(&value));
    }
    return resetErrorState();
}

/**
 * @brief Checks if a QString value is valid (non-empty).
 * 
 * Updates the widget's error state if the string is empty.
 * 
 * @param value The QString value to validate.
 * @return true if valid (non-empty), false otherwise.
 */
bool WizardField::fieldValid(QString &value)
{
    if (value.isEmpty()) {
        widget->setErrorState(WizardFieldWidget::ErrorState::StringEmpty);
        return false;
    }
    return resetErrorState();
}

bool WizardField::fieldValid(double value)
{
    if (value < 0 || value > 10000) {
        widget->setErrorState(WizardFieldWidget::ErrorState::DoubleLimit);
        return false;
    }
    return resetErrorState();
}

/**
 * @brief Checks if a double value is within allowed limits.
 * 
 * Validates that the double value is between 0 and 10000 inclusive.
 * Updates the widget's error state if out of bounds.
 * 
 * @param value The double value to validate.
 * @return true if valid, false otherwise.
 */
bool WizardField::fieldValid(QVariantMap &map)
{
    if (map.isEmpty())
        return noFileSelected();

    for (auto [key, value] : map.asKeyValueRange())
        if (!fieldValid(*reinterpret_cast<QString *>(&value)) || !fieldValid(value.toDouble()))
            return false;

    return resetErrorState();
}

/**
 * @brief Resets the error state of the associated widget to no error.
 * 
 * Clears any existing error indications on the widget and returns true
 * to signal successful reset.
 * 
 * @return Always returns true.
 */
bool WizardField::resetErrorState()
{
    widget->setErrorState(WizardFieldWidget::ErrorState::NoError);
    return true;
}