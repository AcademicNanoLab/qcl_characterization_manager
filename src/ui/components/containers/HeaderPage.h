/**
 * @file HeaderPage.h
 * @brief Declaration of HeaderPage with scrollable layout and dynamic widget addition.
 *
 * Provides a scrollable page featuring a header title and a vertical layout
 * to which widgets can be dynamically added. Supports hiding the header title.
 * 
 * @author Aleksandar Demic
 */

#ifndef HEADERPAGE_H
	#define HEADERPAGE_H

	#include <QVBoxLayout>
	#include <QScrollArea>
	#include <QString>
	#include <QWidget>

	// Forward declarations for improved compile times
	class Text;
	class Widget;

	/**
	 * @class HeaderPage
	 * @brief Scrollable page with header and dynamic widget layout.
	 *
	 * Displays a title at the top and allows adding widgets to a vertical layout
	 * inside a scroll area. The title can be hidden after construction.
	 */
	class HeaderPage : public QScrollArea
	{
			Q_OBJECT

		public:
			explicit HeaderPage(const QString &title, QWidget *parent = nullptr); ///< Constructs page with title and optional parent.

			void addWidget(QWidget *widget); ///< Adds a widget to the vertical layout.

			QString getTitle() const; ///< Returns the page title.

			void hideTitle(); ///< Hides the header title widget.

		private:
			void configureLayout(QVBoxLayout *layout); ///< Sets up layout properties.

			QString title;       ///< The page title.
			Widget *contents;    ///< Main content area holding child widgets.
	};
#endif // HEADERPAGE_H