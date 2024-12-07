#include "wizardfieldwidget.h"

#include <QLineEdit>
#include <QRadioButton>
#include <QRegularExpressionValidator>
#include <QScrollBar>
#include <QStandardPaths>
#include "buttongroup.h"

// WizardFieldWidget constructor not specifying layout.
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

// WizardFieldWidget constructor specifying layout, use this to get the default label and layout.
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

void WizardFieldWidget::addLayout(QLayout *_layout)
{
    ((QHBoxLayout *) fieldArea->layout())->addLayout(_layout);
}

void WizardFieldWidget::addWidget(QWidget *widget)
{
    fieldArea->layout()->addWidget(widget);
}

/**************************************** DoubleLineEdit *****************************************/
DoubleLineEdit::DoubleLineEdit(const QString &name, QWidget *parent)
    : QLineEdit{parent}
    , name(name)
{
    static QRegularExpression doubleRx(QRegularExpression::anchoredPattern(R"(\b\d+(\.\d+)?\b)"));
    this->setValidator(new QRegularExpressionValidator(doubleRx, this));
    connect(this, &DoubleLineEdit::textChanged, this, &DoubleLineEdit::textChangedSlot);
}

/**************************************** WizardTextFieldWidget *****************************************/
WizardTextFieldWidget::WizardTextFieldWidget(WizardPage *page,
                                             const QString &name,
                                             const BoxLayout boxLayout,
                                             QWidget *parent)
    : WizardFieldWidget(page, name, WizardField::WizardFieldType::StringField, boxLayout, parent)
{
    // text field
    textField = new QLineEdit();
    textField->setPlaceholderText("enter value");
    addWidget(textField);

    // connect
    connect(textField, &QLineEdit::textChanged, this, &WizardFieldWidget::changeSignalSlot);
}

/**************************************** WizardRadioFieldWidget *****************************************/
WizardRadioFieldWidget::WizardRadioFieldWidget(WizardPage *page,
                                               const QString &name,
                                               QList<QString> optionsList,
                                               const BoxLayout boxLayout,
                                               QWidget *parent)
    : WizardFieldWidget(page, name, WizardField::WizardFieldType::StringField, boxLayout, parent)
{
    radioButtonGroup = new ButtonGroup(ButtonGroup::BoxLayout::HLayout);
    radioButtonGroup->layout()->setContentsMargins(0, 0, 0, 0);
    for (const QString &option : optionsList)
        radioButtonGroup->addButton(new QRadioButton(option));
    addWidget(radioButtonGroup);

    // connect
    connect(radioButtonGroup,
            &ButtonGroup::buttonClickedText,
            this,
            &WizardFieldWidget::changeSignalSlot);

    // init
    radioButtonGroup->initButton();
}

/**************************************** WizardDimensionFieldWidget *****************************************/
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
    QList<QList<QString>> dimensionMap = {{"length", "mm"}, {"width", "μm"}, {"height", "μm"}};

    for (int i = 0; i < 3; i++) {
        QString &key = dimensionMap[i][0];

        Text *label = new Text(key + "(" + dimensionMap[i][1] + ")", "body");
        label->setAlignment(Qt::AlignCenter);

        DoubleLineEdit *textField = new DoubleLineEdit(key);
        textField->setPlaceholderText("enter " + key);

        fieldLayout->setAlignment(Qt::AlignCenter);
        fieldLayout->addWidget(label, 0, i * 2);
        fieldLayout->addWidget(textField, 1, i * 2);

        connect(textField,
                &DoubleLineEdit::_textChanged,
                this,
                &WizardDimensionFieldWidget::textChangedSlot);

        // initialize text
        emit textField->_textChanged(key, "");

        if (i < 2)
            fieldLayout->addWidget(new Text("×", "body"), 1, i * 2 + 1);

        textFields.append(textField);
    }

    addLayout(fieldLayout);
}

/**************************************** WizardFileFieldWidget *****************************************/
WizardFileFieldWidget::WizardFileFieldWidget(WizardPage *page,
                                             const QString &name,
                                             const QString &fileVariableName,
                                             QWidget *parent)
    : WizardFieldWidget(page, name, WizardField::WizardFieldType::FileField, parent)
    , fileVariableName(fileVariableName)
    , fileDialog(new QFileDialog())
{
    this->setObjectName("fileField");
    page->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    errorArea->layout()->addWidget(errorAreaText);

    // fileDialog
    downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    // Since the errorArea is inactive here, we're just using the QVBoxLayout directly
    layout()->setContentsMargins(0, 0, 0, 0);

    // header area
    Widget *headerArea = new Widget();
    QHBoxLayout *headerAreaLayout = new QHBoxLayout();
    headerArea->setLayout(headerAreaLayout);
    layout()->addWidget(headerArea);

    // header area - file name
    Text *fileName = new Text("Filename", "h3");
    fileName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    headerAreaLayout->addWidget(fileName);

    // header area - file variable
    Text *fileVariable = new Text(fileVariableName, "h3");
    fileVariable->setFixedWidth(150);
    headerAreaLayout->addWidget(fileVariable);

    // header area - placeholder making header align with contents
    PushButton *closeButtonPlaceholder = new PushButton(QIcon(""));
    closeButtonPlaceholder->setFixedSize(30, 30);
    closeButtonPlaceholder->setDisabled(true);
    headerAreaLayout->addWidget(closeButtonPlaceholder);

    // divider
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout()->addWidget(line);

    // scroll area
    QScrollArea *fileFieldScrollArea = new QScrollArea();
    fileFieldScrollContents = new Widget();
    fileFieldScrollLayout = new QVBoxLayout();

    fileFieldScrollArea->setWidget(fileFieldScrollContents);
    fileFieldScrollArea->setWidgetResizable(true);
    fileFieldScrollContents->setLayout(fileFieldScrollLayout);
    fileFieldScrollLayout->setAlignment(Qt::AlignTop);
    fileFieldScrollLayout->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(fileFieldScrollArea);

    // add button
    PushButton *addFileButton = new PushButton("+", "contained");
    layout()->addWidget(addFileButton);

    // connect add button
    connect(addFileButton, &PushButton::clicked, this, &WizardFileFieldWidget::addButtonClicked);

    // connect change
    field->connectFileField(this, SIGNAL(fileChangeSignal(QVariantMap)), fileVariableName);
}

/**************************************** WizardFileFieldWidgetFile *****************************************/
WizardFileFieldWidgetFile::WizardFileFieldWidgetFile(const QString &fileNameText,
                                                     const QString &fileVariableName,
                                                     QWidget *parent)
    : Widget{parent}
    , fileName(fileNameText)
    , fileVariableName(fileVariableName)
    , fileVariableValue("")
{
    QHBoxLayout *layout = new QHBoxLayout();
    setLayout(layout);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    layout->setAlignment(Qt::AlignLeft);

    // file name
    Text *fileNameWidget = new Text(parseFileName(fileNameText), "h3");
    fileNameWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    layout->addWidget(fileNameWidget);

    // text field
    DoubleLineEdit *fileVariableValueEdit = new DoubleLineEdit(fileVariableName);
    fileVariableValueEdit->setPlaceholderText("enter " + fileVariableName);
    fileVariableValueEdit->setFixedWidth(150);
    layout->addWidget(fileVariableValueEdit);

    // connect text field
    connect(fileVariableValueEdit,
            &QLineEdit::textChanged,
            this,
            &WizardFileFieldWidgetFile::textChange);

    // close button
    PushButton *closeButton = new PushButton(QIcon(":/src/images/close-button.svg"));
    closeButton->setFixedSize(30, 30);
    layout->addWidget(closeButton);

    // connect close button
    connect(closeButton, &PushButton::clicked, this, &WizardFileFieldWidgetFile::deleteLater);
}

QString WizardFileFieldWidgetFile::parseFileName(const QString &fileName)
{
    return fileName.split('/').last();
}
