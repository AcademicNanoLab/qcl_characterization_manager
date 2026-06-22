/**
 * @file WelcomePage.cpp
 * @brief Implementation of WelcomePage widget displaying welcome screen with logo, image, and buttons.
 * 
 * This widget features a two-column layout: 
 * the left side with header text and buttons, 
 * the right side with logo and an image that resize dynamically.
 * 
 * Handles user interaction via buttons and adapts the UI when resized.
 * 
 * @author Aleksandar Demic
 */

#include "welcomepage.h"
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include "ui/components/buttons/ButtonGroup.h"
#include "ui/components/text/Text.h"
#include "ui/dialogs/HelpDialog.h" 

/**
 * @brief Constructs the WelcomePage widget with header text, buttons, logo, and QCL image.
 * 
 * Sets up layouts, initializes UI components, and connects button signals to slots.
 * Initializes images with default sizes and arranges them alongside interactive buttons.
 * 
 * @param parent The parent widget, defaulting to nullptr.
 */
WelcomePage::WelcomePage(QWidget *parent)
    : Widget{parent}, imgLabel(new QLabel(this)), logoLabel(new QLabel(this))  // Initialize both labels
{
    this->setContentsMargins(50, 0, 0, 0);

    QHBoxLayout *layout = new QHBoxLayout();
    setLayout(layout);

    // Left Layout (Header and Buttons)
    QVBoxLayout *leftLayout = new QVBoxLayout();
    layout->addLayout(leftLayout);

    // Add header
    leftLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));
    Text *header = new Text("  QCL Characterization Manager", "h1", this);
    header->setMinimumWidth(300);
    leftLayout->addWidget(header);

    // Add Buttons (with expanded width to make them responsive)
    ButtonGroup *welcomeMenu = new ButtonGroup(ButtonGroup::BoxLayout::VLayout);

    PushButton *startButton = new PushButton("Start Wizard", "contained");
    startButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    PushButton *analyzeButton = new PushButton("Process customised files", "outlined");
    analyzeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    PushButton *helpButton = new PushButton("Help", "text");
    helpButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    welcomeMenu->addButton(startButton);
    welcomeMenu->addButton(analyzeButton);
    welcomeMenu->addButton(helpButton);

    welcomeMenu->layout()->setSpacing(20);
    leftLayout->addWidget(welcomeMenu);

    // Add more space below the buttons
    leftLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));

    // Connect buttons to the appropriate signal/slot
    connect(welcomeMenu, &ButtonGroup::buttonClickedId, this, &WelcomePage::buttonClickedIdSlot);

    connect(helpButton, &QPushButton::clicked, this, [this]() {
        HelpDialog *dialog = new HelpDialog(this);
        dialog->exec();
    });

    // Right Layout (Logo and Image)
    QVBoxLayout *rightLayout = new QVBoxLayout();
    layout->addLayout(rightLayout);

    // Add Logo
    QHBoxLayout *logoLayout = new QHBoxLayout();
    QPixmap logo(":/src/resources/images/logo.png");
    logoLabel->setPixmap(logo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // Initial logo size
    logoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum));
    logoLayout->addWidget(logoLabel);
    rightLayout->addLayout(logoLayout);
    rightLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));

    // Add QCL Image (with resize handling)
    QHBoxLayout *imgLayout = new QHBoxLayout();
    imgLabel->setPixmap(QPixmap(":/src/resources/images/qcl.png"));
    imgLabel->setAlignment(Qt::AlignCenter);
    imgLayout->addStretch();  // Push image upwards
    imgLayout->addWidget(imgLabel);
    imgLayout->addStretch();  // Stretch image proportionally

    rightLayout->addLayout(imgLayout);
}

/**
 * @brief Handles resize events to dynamically scale the QCL image and logo.
 * 
 * Overrides QWidget::resizeEvent to adjust pixmap sizes based on the current widget dimensions,
 * maintaining aspect ratio and smooth transformations.
 * 
 * @param event The resize event triggering this handler.
 */
void WelcomePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Get the available size of the widget
    QSize availableSize = size();
    
    // Resize QCL image (imgLabel)
    int imgWidth = availableSize.width() / 2;  // Adjust width to half of the widget's width
    int imgHeight = availableSize.height() / 2;  // Adjust height to half of the widget's height
    imgLabel->setPixmap(QPixmap(":/src/resources/images/qcl.png").scaled(imgWidth, imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Resize logo (logoLabel)
    int logoWidth = availableSize.width() / 4;  // Adjust logo width (e.g., 1/4 of widget width)
    int logoHeight = availableSize.height() / 4;  // Adjust logo height (e.g., 1/4 of widget height)
    logoLabel->setPixmap(QPixmap(":/src/resources/images/logo.png").scaled(logoWidth, logoHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}