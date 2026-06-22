/**
 * @file MessageBox.cpp
 * @brief Custom QMessageBox subclass that sets all buttons' cursor to pointing hand.
 * 
 * This class enhances the QMessageBox by changing the cursor of all buttons to a pointing hand cursor
 * to improve user experience.
 * 
 * @author Aleksandar Demic
 */
 
#include "MessageBox.h"
#include <QAbstractButton>

/**
 * @brief Constructs a MessageBox with specified icon, title, text, buttons, and parent widget.
 * 
 * Initializes the message box and sets the cursor for all buttons to Qt::PointingHandCursor.
 * 
 * @param icon The icon to display in the message box.
 * @param title The window title of the message box.
 * @param text The main text message displayed in the message box.
 * @param buttons The buttons to include in the message box.
 * @param parent The parent widget for this message box.
 * @param f Window flags for the message box window.
 */
MessageBox::MessageBox(QMessageBox::Icon icon,
                       const QString &title,
                       const QString &text,
                       QMessageBox::StandardButtons buttons,
                       QWidget *parent,
                       Qt::WindowFlags f)
    // Proper initialization of the base class constructor
    : QMessageBox(icon, title, text, buttons, parent, f)
{
    // Set cursor to pointing hand for all buttons in the message box
    for (QAbstractButton *button : this->buttons()) {
        if (button) {
            button->setCursor(Qt::PointingHandCursor); // Set pointing hand cursor for button
        } else {
            // Logging in case a button is not valid (though it shouldn't happen)
            qWarning() << "A button was not valid.";
        }
    }
}