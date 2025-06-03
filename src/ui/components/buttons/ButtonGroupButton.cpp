/**
 * @file ButtonGroupButton.cpp
 * @brief Implementation of a variant-based button used within a button group.
 * 
 * This class extends PushButton and includes logic for updating its visual state
 * based on a comparison between its own ID and the selected button's ID.
 * 
 * The visual state is adjusted dynamically using "contained", "outlined", or "disabled" variants.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "ButtonGroupButton.h"

/**
 * @brief Constructs a ButtonGroupButton with a given ID, text, and variant.
 * 
 * Initializes the button with a unique ID and sets its text and visual variant.
 * 
 * @param id The identifier for this button.
 * @param text The text label to display.
 * @param variant The visual variant ("contained", "outlined", or "disabled").
 * @param parent The parent widget.
 */
ButtonGroupButton::ButtonGroupButton(int id,
                                     const QString &text,
                                     const QString &variant,
                                     QWidget *parent)
    : PushButton(text, variant, parent)
    , id(id)
{}

/**
 * @brief Slot that updates the button's visual variant based on a selected ID.
 * 
 * Compares the provided ID with this button's ID to determine and apply the appropriate variant.
 * If the variant changes, the style is refreshed.
 * 
 * @param id The ID of the selected button in the group.
 */
void ButtonGroupButton::buttonClickedIdSlot(int id)
{
    QString newVariant;
    if (this->id < id) {
        newVariant = "contained";
    } else if (this->id == id) {
        newVariant = "outlined";
    } else {
        newVariant = "disabled";
    }

    // Only update variant if it changed
    if (newVariant != this->getVariant()) {
        this->setVariant(newVariant);
        this->style()->polish(this);  // Force style update
    }
}