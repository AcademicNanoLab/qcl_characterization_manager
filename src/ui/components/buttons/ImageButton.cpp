/**
 * @file ImageButton.cpp
 * @brief Implementation of a button that displays an image and handles selection logic.
 * 
 * This class extends ButtonGroupButton to create a button with a custom image,
 * loaded from a file. The image is scaled and used as the button icon.
 * 
 * The class handles user interaction by emitting the button's ID and file path
 * when clicked, and visually reflects selection status based on ID comparison.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "ImageButton.h"
#include <QPixmap>

/**
 * @brief Constructs an ImageButton with a given ID and image file path.
 * 
 * This constructor initializes the button with an image loaded from the specified file path.
 * The image is scaled to 150x150 pixels (maintaining aspect ratio) and set as the button icon.
 * 
 * @param id The identifier for the button within a group.
 * @param filePath The path to the image file used as the button icon.
 * @param parent The parent widget for this button.
 */
ImageButton::ImageButton(int id, const QString &filePath, QWidget *parent)
    : ButtonGroupButton{id, "Image", "contained", parent}, filePath(filePath)
{
    QPixmap pixmapOriginal(filePath);
    QPixmap pixmap = pixmapOriginal.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QIcon icon(pixmap);
    this->setIcon(icon);
    this->setIconSize(pixmap.rect().size());
}

/**
 * @brief Slot triggered when the button is clicked.
 * 
 * Emits a signal with the button's ID and image file path.
 */
void ImageButton::clickedSlot()
{
    emit buttonClicked(id, filePath);
}

/**
 * @brief Slot that responds to a button click event identified by ID.
 * 
 * If the given ID matches this button's ID, its variant is set to "outlined".
 * Otherwise, the variant is set to "disabled". Forces a style update.
 * 
 * @param id The ID of the clicked button.
 */
void ImageButton::buttonClickedIdSlot(int id)
{
    if (this->id == id)
        this->setVariant("outlined");
    else
        this->setVariant("disabled");
    this->style()->polish(this);  // Force style update
}
