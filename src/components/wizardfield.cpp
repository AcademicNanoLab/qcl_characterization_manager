#include "wizardfield.h"
#include <QAbstractButton>
#include "messagebox.h"
#include "wizardfieldwidget.h"

// extra error handling functions
bool noFileSelected();

// wizard field
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
    // connect field change
    connect(object, changedSignal, this, SLOT(fieldChange(QVariant)));

    // initialize
    fieldChange("");
}

void WizardField::connectFileField(const QObject *object,
                                   const char *changedSignal,
                                   const QString &_fileVariableName)
{
    // connect field change
    connect(object, changedSignal, this, SLOT(fieldChange(QVariantMap)));
    fileVariableName = _fileVariableName;
}

// This function is called for everything excluding the file fields.
void WizardField::fieldChange(const QVariant &_value)
{
    value = _value;
}

// This function is only called for the file fields.
// This is meant to aid efficiency and reduce copying.
void WizardField::fieldChange(const QVariantMap &_value)
{
    value = _value;
}

// Adds wizard field's string representation into wizardFieldString.
void WizardField::getWizardField(QString &wizardFieldString)
{
    QString printName = name;
    while (printName.contains(' '))
        printName.replace(' ', '_');
    wizardFieldString += printName;

    switch (wizardFieldType) {
    case (WizardFieldType::StringField):
    case (WizardFieldType::DoubleField):
        wizardFieldString += " " + value.toString() + "\n";
        break;
    case (WizardFieldType::DimensionField):
        for (auto [key, value] : reinterpret_cast<QVariantMap *>(&value)->asKeyValueRange())
            wizardFieldString += " " + value.toString();
        wizardFieldString += "\n";
        break;
    case (WizardFieldType::FileField):
        wizardFieldString += " " + fileVariableName + "\n";
        for (auto [key, value] : reinterpret_cast<QVariantMap *>(&value)->asKeyValueRange())
            wizardFieldString += key + " " + value.toString() + "\n";
        break;
    }
}

void WizardField::addToMap(QVariantMap &map)
{
    bool flag = false;

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
}

// Field validity checkers from here on.
//
//
//
//
// Returns bool value for field validity.
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

// Check if all values in key-value pairs aren't empty and are between 0~10000
bool WizardField::fieldValid(QVariantMap &map)
{
    if (map.isEmpty())
        return noFileSelected();

    for (auto [key, value] : map.asKeyValueRange())
        if (!fieldValid(*reinterpret_cast<QString *>(&value)) || !fieldValid(value.toDouble()))
            return false;

    return resetErrorState();
}

bool WizardField::resetErrorState()
{
    widget->setErrorState(WizardFieldWidget::ErrorState::NoError);
    return true;
}

//
//
//
// Handle errors
bool noFileSelected()
{
    MessageBox *msgBox = new MessageBox(QMessageBox::Warning,
                                        "",
                                        "You haven't chosen any files.",
                                        QMessageBox::Ok | QMessageBox::Cancel);
    msgBox->setInformativeText("Would you like to continue?");

    // execute
    int ret = msgBox->exec();

    switch (ret) {
    case QMessageBox::Ok:
        return true;
    }
    return false;
}
