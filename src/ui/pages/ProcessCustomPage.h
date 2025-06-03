/**
 * @file ProcessCustomPage.h
 * @brief Declaration of ProcessCustomPage handling custom data processing and visualization.
 *
 * Defines the ProcessCustomPage class, a wizard step for processing custom measurement data.
 * It provides UI elements to display images in a carousel, generate data sheets, reset the view,
 * and navigate back. It manages layout and image loading logic for enhanced data interaction.
 * 
 * @author Aleksandar Demic
 */

#ifndef PROCESSCUSTOMPAGE_H
	#define PROCESSCUSTOMPAGE_H

	#include <QVBoxLayout>
	#include <QHBoxLayout>
	#include <QScrollArea>
	#include "ui/components/buttons/ButtonGroup.h"
	#include "ui/components/imagecaraousel/imagecarousel.h"
	#include "ui/components/containers/widget.h"
	#include "ui/components/wizard/WizardPage.h"
	#include "ui/components/buttons/PushButton.h"
	#include "core/graceplots/graceplot.h"

	/**
	 * @class ProcessCustomPage
	 * @brief Wizard page for processing and visualizing custom measurement data.
	 *
	 * Provides UI elements for generating images, navigating results, and exporting data.
	 * Organizes layout dynamically and handles input/output flow for analysis steps.
	 */
	class ProcessCustomPage : public WizardPage
	{
			Q_OBJECT

		public:
			explicit ProcessCustomPage(const QString &title, QWidget *parent = nullptr); ///< Constructs the custom processing page.
			void generateGraceImages();  ///< Generates grace plot images from data.
			void generateDataSheet();    ///< Creates a data sheet from processed results.

		private:
			QString outputDir;
			Widget *nothingToShowWidget;
			Widget *generateImagesControlWidget;
			ImageCarousel *imageCarousel;
			ButtonGroup *imageMenu;
			PushButton *generateDataSheetButton;
			PushButton *resetButton;
			QPushButton *backButton;  ///< Button to navigate back in the wizard.

			void addImage(const QString &imagePath); ///< Adds an image to the carousel.
			void initNothingToShowWidget(); ///< Initializes the empty state widget.
			void initGenerateImagesControlWidget(); ///< Sets up controls for image generation.
			void loadGeneratedImagesFromFigures(); ///< Loads images from generated figures.

		private slots:
			void resetView(); ///< Resets the view layout and image display.
			
		public slots:
			void onBackClicked(); ///< Slot for handling back navigation.

		signals:
			void finished(int id); ///< Emitted when processing is complete.
			void dataProcessed(const QVariantMap &updatedData); ///< Emitted when data has been processed.
};
#endif // PROCESSCUSTOMPAGE_H