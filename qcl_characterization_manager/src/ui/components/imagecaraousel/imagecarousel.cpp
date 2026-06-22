/**
 * @file ImageCarousel.cpp
 * @brief Implementation of an interactive image carousel with support for images and plots.
 * 
 * The ImageCarousel widget allows navigation through a series of images or QCustomPlot widgets.
 * It includes left/right navigation buttons and maintains a QStackedWidget to manage the items.
 * 
 * Each image is displayed in a centered layout and scaled appropriately. The carousel supports
 * adding both image files and dynamic plots, making it flexible for data visualization UIs.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "imagecarousel.h"
#include <QHBoxLayout>
#include <QLabel>
#include "ui/components/buttons/PushButton.h"
#include "ui/components/buttons/ButtonGroupButton.h"
#include "ui/components/buttons/ImageButton.h"

/**
 * @brief Constructs an ImageCarousel widget with navigation controls.
 * 
 * Sets up the layout with previous and next buttons and initializes
 * the image stack. Connects navigation and stack update signals.
 * 
 * @param parent The parent widget.
 */
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

/**
 * @brief Adds an image to the carousel from a file path.
 * 
 * Loads and scales the image, wraps it in a QWidget with a QLabel,
 * and adds it to the stacked widget.
 * 
 * @param imagePath Path to the image file.
 */
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

/**
 * @brief Adds a QCustomPlot to the carousel.
 * 
 * Inserts the given plot widget directly into the stacked image area.
 * 
 * @param plot The QCustomPlot widget to add.
 */
void ImageCarousel::addImage(QCustomPlot *plot)
{
    imageStack->addWidget(plot);
}

/**
 * @brief Sets the index of the currently displayed item.
 * 
 * Wraps around if the index is out of bounds (cyclic behavior).
 * 
 * @param index The index to display in the carousel.
 */
void ImageCarousel::setImageStackIndex(int index)
{
    if (index < 0)
        imageStack->setCurrentIndex(imageStack->count() - 1);
    else if (index >= imageStack->count())
        imageStack->setCurrentIndex(0);
    else
        imageStack->setCurrentIndex(index);
}

/**
 * @brief Clears all images and plots from the carousel.
 * 
 * Removes and safely deletes all widgets from the QStackedWidget,
 * then resets the index to ensure no item is shown.
 */
void ImageCarousel::clear()
{
    while (imageStack->count() > 0) {
        QWidget *widget = imageStack->widget(0);
        imageStack->removeWidget(widget);
        widget->deleteLater();
    }
	imageStack->update();
	imageStack->setCurrentIndex(-1); // Ensures no image is shown
}