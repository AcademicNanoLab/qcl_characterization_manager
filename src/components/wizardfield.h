#ifndef WIZARDFIELD_H
#define WIZARDFIELD_H

#include <QObject>
#include <QVariant>

class WizardFieldWidget;

class WizardField : public QObject
{
    Q_OBJECT
public:
    enum class WizardFieldType { StringField, DoubleField, DimensionField, FileField };
    explicit WizardField(const QString &name,
                         WizardFieldType wizardFieldType,
                         const QObject *object,
                         const char *changedSignal);
    void connectFileField(const QObject *object,
                          const char *changedSignal,
                          const QString &fileVariableName);
    void getWizardField(QString &wizardFieldString);
    void addToMap(QVariantMap &map);
    bool fieldValid();
    bool fieldValid(QString &value);
    bool fieldValid(double value);
    bool fieldValid(QVariantMap &map);
    bool resetErrorState();

private slots:
    void fieldChange(const QVariant &value);
    void fieldChange(const QVariantMap &value);

private:
    QString name;
    QString fileVariableName;
    QVariant value;
    WizardFieldType wizardFieldType;
    WizardFieldWidget *widget;
};

#endif // WIZARDFIELD_H
