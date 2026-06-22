/**
 * @file ButtonGroupButton.h
 * @brief Declaration of ButtonGroupButton extending PushButton with an ID.
 *
 * Provides a button class that holds a unique integer ID, useful for grouping
 * and identifying buttons within a ButtonGroup.
 * 
 * @author Aleksandar Demic
 */

#ifndef BUTTONGROUPBUTTON_H
	#define BUTTONGROUPBUTTON_H
	#include "PushButton.h"

	/**
	 * @class ButtonGroupButton
	 * @brief QPushButton subclass with an integer ID property.
	 *
	 * Used to uniquely identify buttons inside a ButtonGroup.
	 */
	class ButtonGroupButton : public PushButton
	{
			Q_OBJECT

		public:
			explicit ButtonGroupButton(int id,
									   const QString &text,
									   const QString &variant = "contained",
									   QWidget *parent = nullptr); ///< Constructs button with ID, text, and variant.

			int id; ///< Unique identifier for this button.

		public slots:
			void buttonClickedIdSlot(int id); ///< Slot to handle click events by ID.
	};
#endif // BUTTONGROUPBUTTON_H