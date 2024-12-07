#ifndef WIZARDFIELDWIDGET_H
#define WIZARDFIELDWIDGET_H

#include <QFileDialog>
#include <QLineEdit>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <QVariant>
#include "buttongroup.h"
#include "text.h"
#include "widget.h"
#include "wizardfield.h"
#include "wizardpage.h"

// Base widget for wizard fields, initializes layout and label.
class WizardFieldWidget : public Widget
{
    Q_OBJECT
public:
    enum class BoxLayout { VLayout, HLayout };
    enum class ErrorState { NoError, StringEmpty, DoubleLimit, MapError };
    explicit WizardFieldWidget(WizardPage *page,
                               const QString &name,
                               WizardField::WizardFieldType wizardFieldType,
                               QWidget *parent = nullptr);
    explicit WizardFieldWidget(WizardPage *page,
                               const QString &name,
                               WizardField::WizardFieldType wizardFieldType,
                               WizardFieldWidget::BoxLayout boxLayout,
                               QWidget *parent = nullptr);
    virtual void clear() = 0;
    void setErrorState(ErrorState _errorState);
    void addLayout(QLayout *layout);
    void addWidget(QWidget *widget);
    WizardField *field;

signals:
    void changeSignal(const QVariant &value);

public slots:
    void changeSignalSlot(QVariant value)
    {
        emit changeSignal(value);
        setErrorState(ErrorState::NoError);
    };

public:
    Widget *fieldArea;
    Widget *errorArea;
    Text *errorAreaText;
    ErrorState errorState;
};

// QLineEdit that only allows double input.
class DoubleLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit DoubleLineEdit(const QString &name, QWidget *parent = nullptr);
    QString name;

signals:
    void _textChanged(QString name, QString value);

public slots:
    void textChangedSlot(QString value) { emit _textChanged(name, value); }
};

// Widget for text input.
class WizardTextFieldWidget : public WizardFieldWidget
{
    Q_OBJECT
public:
    explicit WizardTextFieldWidget(WizardPage *page,
                                   const QString &name,
                                   const BoxLayout boxLayout,
                                   QWidget *parent = nullptr);
    void clear() { textField->setText(""); }

private:
    QLineEdit *textField;
};

// Widget for radio input.
class WizardRadioFieldWidget : public WizardFieldWidget
{
    Q_OBJECT
public:
    explicit WizardRadioFieldWidget(WizardPage *page,
                                    const QString &name,
                                    QList<QString> optionsList,
                                    const BoxLayout boxLayout,
                                    QWidget *parent = nullptr);
    void clear() { radioButtonGroup->initButton(); }

private:
    ButtonGroup *radioButtonGroup;
};

// Widget for dimension input (length × width × height).
class WizardDimensionFieldWidget : public WizardFieldWidget
{
    Q_OBJECT
public:
    explicit WizardDimensionFieldWidget(WizardPage *page,
                                        const QString &name,
                                        QWidget *parent = nullptr);
    void clear()
    {
        for (DoubleLineEdit *textField : textFields)
            textField->setText("");
    }
    QMap<QString, QVariant> valueMap;

public slots:
    void textChangedSlot(QString name, QString value)
    {
        valueMap[name] = value;
        emit changeSignal(valueMap);
        setErrorState(ErrorState::NoError);
    }

private:
    QList<DoubleLineEdit *> textFields;
};

// Widget for each file.
class WizardFileFieldWidgetFile : public Widget
{
    Q_OBJECT
public:
    explicit WizardFileFieldWidgetFile(const QString &fileName,
                                       const QString &fileVariableName,
                                       QWidget *parent = nullptr);
    QString parseFileName(const QString &fileName);
    QMap<QString, QString> getFileField();

signals:
    void fileDeleted(const QString &fileName);
    void fileTextChanged(const QString &fileName, const QString &fileVariableValue);

public slots:
    void textChange(const QString &_fileVariableValue)
    {
        fileVariableValue = _fileVariableValue;
        emit fileTextChanged(fileName, fileVariableValue);
    }
    void deleteLater()
    {
        emit fileDeleted(fileName);
        Widget::deleteLater();
    }

private:
    QString fileName;
    QString fileVariableName;
    QString fileVariableValue;
};

// Widget for inputting files.
class WizardFileFieldWidget : public WizardFieldWidget
{
    Q_OBJECT
public:
    explicit WizardFileFieldWidget(WizardPage *page,
                                   const QString &name,
                                   const QString &fileVariableName,
                                   QWidget *parent = nullptr);
    void clear()
    {
        for (WizardFileFieldWidgetFile *individualFileWidget :
             fileFieldScrollContents->findChildren<WizardFileFieldWidgetFile *>())
            individualFileWidget->deleteLater();
    }
    QMap<QString, QVariant> fileMap;
    QString fileVariableName;
    QFileDialog *fileDialog;
    QString downloadPath;
    Widget *fileFieldScrollContents;
    QVBoxLayout *fileFieldScrollLayout;

signals:
    void fileChangeSignal(const QVariantMap &map);

public slots:
    void fileSelected(const QString &fileName)
    {
        WizardFileFieldWidgetFile *fileWidget = new WizardFileFieldWidgetFile(fileName,
                                                                              fileVariableName);
        fileFieldScrollLayout->addWidget(fileWidget);

        // connect each file being changed
        connect(fileWidget,
                &WizardFileFieldWidgetFile::fileTextChanged,
                this,
                &WizardFileFieldWidget::fileTextChangedSlot);
        connect(fileWidget,
                &WizardFileFieldWidgetFile::fileDeleted,
                this,
                &WizardFileFieldWidget::fileDeletedSlot);

        // initialize
        emit fileWidget->fileTextChanged(fileName, "");
    }
    void addButtonClicked()
    {
        QStringList files = fileDialog->getOpenFileNames(this,
                                                         "Select one or more files to open",
                                                         downloadPath,
                                                         "Text files (*.txt)",
                                                         nullptr,
                                                         QFileDialog::ReadOnly);
        for (const QString &file : files)
            fileSelected(file);
    }
    void fileTextChangedSlot(const QString &fileName, const QString &fileVariableValue)
    {
        fileMap[fileName] = fileVariableValue;
        emit fileChangeSignal(fileMap);
        setErrorState(ErrorState::NoError);
    }
    void fileDeletedSlot(const QString &fileName)
    {
        fileMap.remove(fileName);
        emit fileChangeSignal(fileMap);
    }
};

#endif // WIZARDFIELDWIDGET_H
