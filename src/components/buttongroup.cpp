#include "buttongroup.h"
#include <QAbstractButton>
#include <QLayout>

ButtonGroup::ButtonGroup(BoxLayout boxLayout, QWidget *parent)
    : QGroupBox{parent}
    , buttonGroup(new QButtonGroup(this))
{
    if (boxLayout == BoxLayout::VLayout) {
        setLayout(new QVBoxLayout());
        layout()->setAlignment(Qt::AlignTop);
    } else {
        setLayout(new QHBoxLayout());
        layout()->setAlignment(Qt::AlignLeft);
    }
    setFlat(true);

    // connect buttonGroup signals to this item's signals
    connect(buttonGroup, &QButtonGroup::idClicked, this, &ButtonGroup::idClickedSlot);
    connect(buttonGroup, &QButtonGroup::buttonClicked, this, &ButtonGroup::buttonClickedSlot);
}

// This adds new button to the layout as provided by QAbstractButton
void ButtonGroup::addButton(QAbstractButton *button, int id)
{
    if (id == -1)
        id = this->findChildren<QAbstractButton *>().length();
    buttonGroup->addButton(button, id); // add buttons (group)
    layout()->addWidget(button);        // add buttons (UI)
}

// This adds new ButtonGroupButton with corresponding text
void ButtonGroup::addMenuButton(const QObject *object,
                                const char *changeSignal,
                                const QString &text,
                                int id)
{
    if (id == -1)
        id = this->findChildren<QAbstractButton *>().length();
    ButtonGroupButton *button = new ButtonGroupButton(id, text);
    addButton(button, id);

    // when other buttons are clicked, change style
    connect(object, changeSignal, button, SLOT(buttonClickedIdSlot(int)));
    button->buttonClickedIdSlot(0);
}

// This adds new ButtonGroupButton with corresponding text
void ButtonGroup::addImageButton(const QObject *object,
                                 const char *changeSignal,
                                 const QString &filePath,
                                 int id)
{
    if (id == -1)
        id = this->findChildren<QAbstractButton *>().length();
    ImageButton *button = new ImageButton(id, filePath);
    addButton(button, id);

    // when other buttons are clicked, change style
    connect(object, changeSignal, button, SLOT(buttonClickedIdSlot(int)));
    button->buttonClickedIdSlot(0);
}

// This function initializes button choice to be the first button
void ButtonGroup::initButton()
{
    // initialize button choice
    if (this->buttonGroup->button(0))
        this->buttonGroup->button(0)->animateClick();
}

// This function removes all buttons from the ButtonGroup
void ButtonGroup::clear()
{
    for (PushButton *button : findChildren<PushButton *>())
        button->deleteLater();
}
