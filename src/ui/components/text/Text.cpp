/**
 * @file Text.cpp
 * @brief Implementation of a QLabel-based component with support for text styling variants.
 * 
 * The Text class extends QLabel to provide variant-based styling for different display contexts.
 * It supports variants such as "body" and "highlighted", and emits a signal when the variant changes.
 * This makes it useful for consistent UI typography across an application.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "Text.h"

/**
 * @brief Constructs a Text widget with specified content and variant.
 * 
 * Initializes a QLabel with provided text and styling variant. Enables word wrapping
 * and removes default indentation.
 * 
 * @param text The text to display.
 * @param variant The styling variant (e.g., "body", "highlighted").
 * @param parent The parent widget.
 */
Text::Text(const QString &text, const QString &variant, QWidget *parent)
    : QLabel{text, parent}, variant(variant)
{
    setVariant(variant);  // Set the variant when constructing
    setIndent(0);          // Align text to left without padding
    setWordWrap(true);     // Enable word wrap
}

/**
 * @brief Returns the current text variant.
 * 
 * @return The variant string applied to this Text widget.
 */
QString Text::getVariant() const
{
    return variant;
}

/**
 * @brief Sets a new variant and applies associated styles.
 * 
 * Changes the visual appearance of the label based on the given variant.
 * If the variant differs from the current one, the stylesheet is updated
 * and a `variantChanged` signal is emitted.
 * 
 * @param newVariant The new styling variant to apply.
 */
void Text::setVariant(const QString &newVariant)
{
    if (variant != newVariant) {
        variant = newVariant;
        
        // Apply styles based on variant
        if (variant == "body") {
            setStyleSheet("font-size: 14px; color: black;");
        } else if (variant == "highlighted") {
            setStyleSheet("font-size: 16px; color: red;");
        }
        
        emit variantChanged(newVariant);  // Notify change
    }
}