#ifndef WIZARDPAGE_H
#define WIZARDPAGE_H

#include <QFile>
#include "headerpage.h"

class WizardField;

// Wizard pages that expose addField, getWizardFields, pageValid functions.
class WizardPage : public HeaderPage
{
    Q_OBJECT
public:
    explicit WizardPage(const QString &title, QWidget *parent = nullptr);
    void addField(WizardField *wizardField);
    void getWizardFields(QString &wizardFieldString);
    bool pageValid();
    void clear();
    void writeToFile(QFile *file);
    void addToMap(QVariantMap &map);

signals:

private:
    QList<WizardField *> wizardFieldList;
};

// Wizard pages that are for uploading files.
class WizardFilePage : public WizardPage
{
    Q_OBJECT
public:
    explicit WizardFilePage(const QString &title,
                            const QString &fileVariableName,
                            QWidget *parent = nullptr);
};

#endif // WIZARDPAGE_H
