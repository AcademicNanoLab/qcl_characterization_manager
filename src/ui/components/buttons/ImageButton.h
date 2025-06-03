/**
 * @file ImageButton.h
 * @brief Declaration of ImageButton, a button displaying an image with an ID.
 *
 * Extends ButtonGroupButton to include an image file path and emits signals
 * with ID and file path when clicked.
 * 
 * @author Aleksandar Demic
 */

#ifndef IMAGEBUTTON_H
	#define IMAGEBUTTON_H
	#include "ButtonGroupButton.h"

	/**
	 * @class ImageButton
	 * @brief Button that displays an image and holds an ID.
	 *
	 * Emits signals on click with button ID and image file path.
	 */
	class ImageButton : public ButtonGroupButton
	{
			Q_OBJECT

		public:
			explicit ImageButton(int id, const QString &filePath, QWidget *parent = nullptr); ///< Constructs image button with ID and image path.

			QString filePath; ///< Path to the image displayed on the button.

		signals:
			void buttonClicked(int id, const QString &filePath); ///< Emitted when the button is clicked.

		public slots:
			void clickedSlot(); ///< Handles internal click event.
			void buttonClickedIdSlot(int id); ///< Slot to handle click event by ID.
	};
#endif // IMAGEBUTTON_H