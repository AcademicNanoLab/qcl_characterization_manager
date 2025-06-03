/**
 * @file HeaderPage.cpp
 * @brief Implementation of a scrollable page with an optional header title and dynamic widget support.
 * 
 * The HeaderPage class provides a vertically scrollable area that can contain a dynamic list of widgets.
 * An optional title can be displayed at the top, styled using a custom Text component.
 * The layout is aligned to the top and has no margins to allow clean stacking of elements.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#include "HeaderPage.h"
#include <QVBoxLayout>
#include "ui/components/text/Text.h"
#include "Widget.h"

/**
 * @brief Constructs a HeaderPage with a given title and optional parent.
 * 
 * Initializes a scrollable area with a vertical layout. If a title is provided,
 * it is added at the top of the page using the custom Text component.
 * 
 * @param title The title to display at the top of the page.
 * @param parent The parent widget.
 */
HeaderPage::HeaderPage(const QString &title, QWidget *parent)
    : QScrollArea(parent), title(title), contents(new Widget())
{
    // Create a new vertical layout for the page contents
    QVBoxLayout *layout = new QVBoxLayout();

    // Set the layout for the content widget
    contents->setLayout(layout);

    // Configure layout properties (alignment and margins)
    configureLayout(layout);

    // Only add the title if it's not empty
    if (!title.isEmpty()) {
        layout->addWidget(new Text(title, "h1", contents));  // Add the title as a Text widget
    }

    // Set the main widget and make it resizable
    setWidget(contents);
    setWidgetResizable(true);
}

/**
 * @brief Adds a widget to the page layout.
 * 
 * Appends a new widget to the vertical layout within the scrollable content area.
 * Logs a warning if a null widget is passed.
 * 
 * @param widget The widget to add to the layout.
 */
void HeaderPage::addWidget(QWidget *widget)
{
    if (widget) {
        contents->layout()->addWidget(widget);
    } else {
        qWarning("Attempted to add a null widget.");
    }
}

/**
 * @brief Returns the title of the page.
 * 
 * @return The string value of the page's title.
 */
QString HeaderPage::getTitle() const
{
    return title;
}

/**
 * @brief Configures layout properties such as alignment and margins.
 * 
 * Sets the layout alignment to top and removes all margins for a tight layout.
 * This is a helper function used internally by the constructor.
 * 
 * @param layout The layout to configure.
 */
void HeaderPage::configureLayout(QVBoxLayout *layout)
{
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
}

/**
 * @brief Hides the title text widget after it has been added.
 * 
 * Iterates through layout widgets to locate the Text component representing the title,
 * and hides it if found.
 */
void HeaderPage::hideTitle()
{
    // Loop through the layout items and find the Text widget for the title
    for (int i = 0; i < contents->layout()->count(); ++i) {
        QWidget *widget = contents->layout()->itemAt(i)->widget();
        if (widget && widget->metaObject()->className() == QString("Text")) {
            widget->setVisible(false);  // Hide the title Text widget
            break;  // Exit the loop after finding the title widget
        }
    }
}