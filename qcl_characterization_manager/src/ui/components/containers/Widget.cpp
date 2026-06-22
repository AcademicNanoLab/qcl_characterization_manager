/**
 * @file Widget.cpp
 * @brief Implementation of a base QWidget subclass with enhanced style and click handling.
 * 
 * This class provides a styled QWidget that supports stylesheet-based rendering using QStyleOption.
 * It also emits a custom signal when clicked with the left mouse button.
 * 
 * Intended as a reusable building block for UI components that require basic interactivity
 * and consistent styling support.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "Widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOption>

/**
 * @brief Constructs a basic Widget with optional parent.
 * 
 * Initializes the base QWidget. Additional setup can be added as needed.
 * 
 * @param parent The parent widget.
 */
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // Constructor body (currently empty)
    // Any additional initialization can be added here in the future
}

/**
 * @brief Handles custom painting of the widget.
 * 
 * Uses QStyleOption and QPainter to enable stylesheet-based styling for the widget.
 * This is essential for proper appearance when using QSS (Qt Style Sheets).
 * 
 * @param pe The paint event (unused in current implementation).
 */
void Widget::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);  // Avoid unused parameter warning

    // Initialize QStyleOption to allow custom QSS styling
    QStyleOption option;
    option.initFrom(this);

    // Create a QPainter object to paint on the widget
    QPainter painter(this);

    // Use the current style to draw the widget using the QStyle option
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

    // Optional: Call base class paintEvent if needed for default painting
    // QWidget::paintEvent(pe); // Uncomment this if you need to preserve the default painting
}

/**
 * @brief Handles mouse release events to detect left-clicks.
 * 
 * Emits a custom `clicked()` signal when the user releases the left mouse button over the widget.
 * 
 * @param event The mouse release event.
 */
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    // Only handle the left mouse button click
    if (event->button() == Qt::LeftButton) {
        emit clicked();  // Emit the clicked() signal when the widget is clicked
    }
}