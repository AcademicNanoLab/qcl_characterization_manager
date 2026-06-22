/**
 * @file ImageCarousel.h
 * @brief Declaration of ImageCarousel widget displaying images or plots in a stacked view.
 *
 * Provides a carousel widget to display images or QCustomPlot objects.
 * Supports navigation through images and emits signals when current image changes.
 * 
 * @author Aleksandar Demic
 */

#ifndef IMAGECAROUSEL_H
	#define IMAGECAROUSEL_H
	#include <QStackedWidget>
	#include "core/qtplots/qcustomplot.h"
	#include "ui/components/containers/Widget.h"

	/**
	 * @class ImageCarousel
	 * @brief Widget for displaying a carousel of images or plots.
	 *
	 * Manages a stack of images or QCustomPlot widgets, allowing navigation
	 * and emitting signals on image changes.
	 */
	class ImageCarousel : public Widget
	{
			Q_OBJECT
		public:
			explicit ImageCarousel(QWidget *parent = nullptr); ///< Constructs image carousel.

			void addImage(const QString &imagePath); ///< Adds an image by file path.
			void addImage(QCustomPlot *plot); ///< Adds a QCustomPlot to the carousel.
			void setImageStackIndex(int index); ///< Sets the currently visible image index.
			void clear(); ///< Clears all images from the carousel.

			QStackedWidget *imageStack; ///< Internal stacked widget managing images.

		signals:
			void currentChanged(int id); ///< Emitted when the current image index changes.

		public slots:
			void prevClicked() { setImageStackIndex(imageStack->currentIndex() - 1); } ///< Shows previous image.
			void nextClicked() { setImageStackIndex(imageStack->currentIndex() + 1); } ///< Shows next image.
			void currentChangedSlot(int id) { emit currentChanged(id); } ///< Emits currentChanged signal.
		private:
			// No private members
	};
#endif // IMAGECAROUSEL_H