/**
 * @file WelcomePage.h
 * @brief Declaration of WelcomePage for displaying a welcome screen UI.
 *
 * This class defines the WelcomePage widget, which includes a logo and image display,
 * and emits signals when specific UI buttons are clicked. It handles dynamic layout 
 * adjustments via overridden resize events.
 * 
 * @author Aleksandar Demic
 */

#ifndef WELCOMEPAGE_H
	#define WELCOMEPAGE_H

	#include "ui/components/containers/widget.h"
	#include <QLabel>  // Include QLabel

	/**
	 * @class WelcomePage
	 * @brief Welcome page widget that displays images and handles user interaction.
	 *
	 * Includes signals for button clicks and adapts layout on resize.
	 */
	class WelcomePage : public Widget
	{
			Q_OBJECT
		public:
			explicit WelcomePage(QWidget *parent = nullptr); ///< Constructs the welcome page widget.

		signals:
			void buttonClickedId(int id); ///< Emitted when a button with a specific ID is clicked.

		public slots:
			void buttonClickedIdSlot(int id) { emit buttonClickedId(id + 1); } ///< Slot forwarding button clicks with ID offset.

		protected:
			void resizeEvent(QResizeEvent *event) override; ///< Handles resizing of the widget layout.

			QLabel *imgLabel;  ///< Label to display the main image.
			QLabel *logoLabel; ///< Label to display the logo image.

	};
#endif // WELCOMEPAGE_H