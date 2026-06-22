/**
 * @file Widget.h
 * @brief Custom QWidget supporting QSS styling and click signal.
 *
 * This subclass of QWidget allows for enhanced styling via Qt Style Sheets (QSS)
 * and emits a clicked() signal when the widget is clicked (mouse released).
 * 
 * @author Aleksandar Demic
 */

#ifndef WIDGET_H
	#define WIDGET_H
	#include <QWidget>

	class QPaintEvent;
	class QMouseEvent;

	/**
	 * @class Widget
	 * @brief QWidget subclass emitting clicked signal and supporting QSS.
	 *
	 * Overrides paintEvent for custom painting and mouseReleaseEvent to emit clicked().
	 */
	class Widget : public QWidget
	{
			Q_OBJECT

		public:
			explicit Widget(QWidget *parent = nullptr); ///< Constructs widget with optional parent.

		protected:
			void paintEvent(QPaintEvent *pe) override; ///< Paint event override to enable QSS styling.
			void mouseReleaseEvent(QMouseEvent *event) override; ///< Emits clicked() on mouse release.

		signals:
			void clicked(); ///< Emitted when the widget is clicked.
	};
#endif // WIDGET_H