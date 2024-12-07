#include "welcomepage.h"
#include <QImageReader>
#include <QVBoxLayout>
#include "components/buttongroup.h"
#include "components/text.h"

WelcomePage::WelcomePage(QWidget *parent)
    : Widget{parent}
{
    this->setContentsMargins(50, 0, 0, 0);
    QHBoxLayout *layout = new QHBoxLayout();

    // specify layout
    setLayout(layout);

    // header-button layout
    QVBoxLayout *leftLayout = new QVBoxLayout();
    layout->addLayout(leftLayout);

    // add header
    leftLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));
    Text *header = new Text("QCL \nCharacterization \nManager", "h1", this);
    header->setMinimumWidth(300);
    leftLayout->addWidget(header);

    // add buttons
    ButtonGroup *welcomeMenu = new ButtonGroup(ButtonGroup::BoxLayout::VLayout);
    welcomeMenu->addButton(new PushButton("Start Wizard", "contained"));
    welcomeMenu->addButton(new PushButton("Analyze Files", "outlined"));
    welcomeMenu->layout()->setSpacing(20);
    leftLayout->addWidget(welcomeMenu);
    leftLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));

    // connect button being clicked with emitting the next page
    connect(welcomeMenu, &ButtonGroup::buttonClickedId, this, &WelcomePage::buttonClickedIdSlot);

    // images layout
    QVBoxLayout *rightLayout = new QVBoxLayout();
    layout->addLayout(rightLayout);

    // logo
    QHBoxLayout *logoLayout = new QHBoxLayout();
    QPixmap logo(":/src/images/logo.png");
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(logo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum));
    logoLayout->addWidget(logoLabel);
    rightLayout->addLayout(logoLayout);
    rightLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));

    // qcl
    QHBoxLayout *imgLayout = new QHBoxLayout();
    QPixmap img(":/src/images/qcl.png");
    QLabel *imgLabel = new QLabel();
    imgLabel->setPixmap(img.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum));
    imgLayout->addWidget(imgLabel);
    rightLayout->addLayout(imgLayout);
}
