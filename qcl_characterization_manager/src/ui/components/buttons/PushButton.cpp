/**
 * @file PushButton.cpp
 * @brief Implementation of a customized QPushButton with variant styles and cursor settings.
 * 
 * This class extends QPushButton to support different visual variants such as
 * "contained", "outlined", and "disabled". It also sets the cursor to a pointing hand
 * to indicate interactivity.
 * 
 * The constructors allow creating buttons with either an icon or text, and specify the variant.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "PushButton.h"

/**
 * @brief Constructs a PushButton with an icon and default text variant.
 * 
 * This constructor sets the button to use an icon and a default text variant.
 * It also sets the cursor to a pointing hand to indicate interactivity.
 * 
 * @param icon The icon to display on the button.
 * @param parent The parent widget for this button.
 */
PushButton::PushButton(const QIcon &icon, QWidget *parent)
    : QPushButton(icon, "", parent)
{
    this->setVariant("text");
    this->setCursor(Qt::PointingHandCursor);
}

/**
 * @brief Constructs a PushButton with text and a specified variant.
 * 
 * This constructor creates a button with specified text and a visual variant
 * (e.g., "contained", "outlined", or "disabled"). The cursor is also set to
 * a pointing hand to indicate interactivity.
 * 
 * @param text The text to display on the button.
 * @param variant The variant style to apply to the button.
 * @param parent The parent widget for this button.
 */
PushButton::PushButton(const QString &text, const QString &variant, QWidget *parent)
    : QPushButton(text, parent)
{
    this->setVariant(variant);
    this->setCursor(Qt::PointingHandCursor);
}

/**
 * @brief Gets the current variant style of the button.
 * 
 * This function returns the current visual variant applied to the button, 
 * such as "contained", "outlined", or "disabled".
 * 
 * @return The current variant style of the button.
 */
QString PushButton::getVariant() const
{
    return variant;
}

/**
 * @brief Sets the variant style of the button.
 * 
 * This function sets the visual variant of the button. It only allows
 * known variants: "contained", "outlined", or "disabled".
 * 
 * @param variant The variant style to apply to the button.
 */
void PushButton::setVariant(const QString &variant)
{
    // Validate the variant, only allow known values
    if (variant == "contained" || variant == "outlined" || variant == "disabled") {
        this->variant = variant;
    }
}