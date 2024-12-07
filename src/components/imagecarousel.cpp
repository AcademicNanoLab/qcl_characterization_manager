#include "imagecarousel.h"
#include <QHBoxLayout>
#include <QLabel>
#include "pushbutton.h"

ImageCarousel::ImageCarousel(QWidget *parent)
    : Widget{parent}
    , imageStack(new QStackedWidget())
{
    QHBoxLayout *layout = new QHBoxLayout();
    setLayout(layout);

    // prev and next buttons
    PushButton *prevButton = new PushButton("<", "text");
    PushButton *nextButton = new PushButton(">", "text");

    // connect buttons
    connect(prevButton, &PushButton::clicked, this, &ImageCarousel::prevClicked);
    connect(nextButton, &PushButton::clicked, this, &ImageCarousel::nextClicked);

    // connect stack
    connect(imageStack, &QStackedWidget::currentChanged, this, &ImageCarousel::currentChangedSlot);

    layout->addWidget(prevButton);
    layout->addWidget(imageStack);
    layout->addWidget(nextButton);
}

// Add new image to stack using image path
void ImageCarousel::addImage(const QString &imagePath)
{
    Widget *imageWidget = new Widget();
    imageWidget->setLayout(new QHBoxLayout());

    QPixmap img(imagePath);
    QLabel *imgLabel = new QLabel();
    imgLabel->setPixmap(img.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLabel->setAlignment(Qt::AlignCenter);
    imageWidget->layout()->addWidget(imgLabel);
    imageStack->addWidget(imageWidget);
}

// Add new plot
void ImageCarousel::addImage(QCustomPlot *plot)
{
    imageStack->addWidget(plot);
}

// Change image stack index
void ImageCarousel::setImageStackIndex(int index)
{
    if (index < 0)
        imageStack->setCurrentIndex(imageStack->count() - 1);
    else if (index >= imageStack->count())
        imageStack->setCurrentIndex(0);
    else
        imageStack->setCurrentIndex(index);
}

void ImageCarousel::clear()
{
    for (QWidget *widget : imageStack->findChildren<QWidget *>())
        widget->deleteLater();
}
