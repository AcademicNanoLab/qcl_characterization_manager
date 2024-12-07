#include "wizard.h"
#include <QFile>
#include <QLayout>
#include "components/pushbutton.h"

Wizard::Wizard(QWidget *parent)
    : Widget{parent}
    , wizardPages(new QStackedWidget())
    , wizardMenu(new ButtonGroup(ButtonGroup::BoxLayout::VLayout))
    , wizardButtons(new ButtonGroup(ButtonGroup::BoxLayout::HLayout))
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    QVBoxLayout *vBoxLayout = new QVBoxLayout();

    // layout
    vBoxLayout->setContentsMargins(hBoxLayout->contentsMargins());
    vBoxLayout->setSpacing(20);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    // make the "Previous" and "Next" buttons
    wizardButtons->layout()->setContentsMargins(0, 0, 0, 0);
    wizardButtons->addButton(new PushButton("Cancel", "contained"));
    ((QHBoxLayout *) wizardButtons->layout())
        ->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum));
    wizardButtons->addButton(new PushButton("Previous", "contained"));
    wizardButtons->addButton(new PushButton("Next", "contained"));

    // make divider
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    // make layout
    vBoxLayout->addWidget(wizardPages);
    vBoxLayout->addWidget(wizardButtons);

    hBoxLayout->addWidget(wizardMenu);
    hBoxLayout->addWidget(line);
    hBoxLayout->addLayout(vBoxLayout);

    // connect signals (wizardMenu <-> wizardPages)
    connect(wizardMenu, &ButtonGroup::buttonClickedId, this, &Wizard::setCurrentIndexWrapper);
    connect(wizardPages, &QStackedWidget::currentChanged, wizardMenu, &ButtonGroup::idClickedSlot);

    // connect signals from wizardButtons
    connect(wizardButtons, &ButtonGroup::buttonClickedId, this, &Wizard::wizardButtonsClick);
}

// This adds new pages to the Wizard
void Wizard::addPage(WizardPage *page)
{
    wizardPages->addWidget(page);
    wizardMenu->addMenuButton(wizardPages, SIGNAL(currentChanged(int)), page->getTitle());
}

// This is a wrapper for wizardPages->setCurrentIndex() that handles field validation
void Wizard::setCurrentIndexWrapper(int id)
{
    int cur = wizardPages->currentIndex();
    if (id < cur) // previous pages
        wizardPages->setCurrentIndex(id);
    else {
        for (int i = cur; i < id; i++)
            if (!((WizardPage *) wizardPages->widget(i))->pageValid()) {
                wizardPages->setCurrentIndex(i);
                return;
            }
        wizardPages->setCurrentIndex(id);
    }

    // handle finishing wizard
    if (id == wizardPages->count() - 1 && id != 0)
        ((PushButton *) wizardButtons->layout()->itemAt(3)->widget())->setText("Finish");
    else if (id >= wizardPages->count())
        finishWizard();
    else
        ((PushButton *) wizardButtons->layout()->itemAt(3)->widget())->setText("Next");
}

// This handles what happens when wizardButtons are clicked
void Wizard::wizardButtonsClick(int id)
{
    int cur = wizardPages->currentIndex();
    if (id == 0) // cancel
        finishWizard();
    else if (id == 1) // previous
        setCurrentIndexWrapper(cur - 1);
    else if (id == 2) // next
        setCurrentIndexWrapper(cur + 1);
}

// This clears wizard pages and sets main window to the welcome page
void Wizard::finishWizard()
{
    // This is to be implemented in classes that inherit the wizard
    finishWizardAction();

    emit finished(0);
    wizardPages->setCurrentIndex(0);
    for (WizardPage *wizardPage : wizardPages->findChildren<WizardPage *>())
        wizardPage->clear();
}
