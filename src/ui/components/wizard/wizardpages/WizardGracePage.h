/**
 * @file WizardGracePage.h
 * @brief Wizard page for managing Grace plot images and data sheet generation.
 * 
 * Provides UI for generating and displaying Grace images,
 * generating data sheets, and resetting the view.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDGRACEPAGE_H
	#define WIZARDGRACEPAGE_H

	#include <QVBoxLayout>
	#include <QHBoxLayout>
	#include <QScrollArea>
	#include "ui/components/buttons/ButtonGroup.h"
	#include "ui/components/imagecaraousel/imagecarousel.h"
	#include "ui/components/containers/widget.h"
	#include "ui/components/buttons/PushButton.h"
	#include "ui/components/wizard/WizardPage.h"
	#include "core/graceplots/graceplot.h"

	/**
	 * @class WizardGracePage
	 * @brief Wizard page that manages Grace plot images and data sheets.
	 */
	class WizardGracePage : public WizardPage
	{
			Q_OBJECT

		public:
			explicit WizardGracePage(const QString &title, QWidget *parent = nullptr); ///< Constructor
			void generateGraceImages();                                               ///< Generate Grace plot images
			void generateDataSheet();                                                 ///< Generate data sheet file

		private:
			QVariantMap collectedData;                 ///< Data collected for image and sheet generation
			QString outputDir;                         ///< Output directory path
			Widget *nothingToShowWidget;               ///< Widget shown when no images are available
			Widget *generateImagesControlWidget;       ///< Widget for image generation controls
			ImageCarousel *imageCarousel;               ///< Carousel for displaying images
			ButtonGroup *imageMenu;                     ///< Button group menu for image selection
			PushButton *generateDataSheetButton;       ///< Button to trigger data sheet generation
			PushButton *resetButton;                     ///< Button to reset the view

			void addImage(const QString &imagePath);                  ///< Add image to the carousel
			void initNothingToShowWidget();                           ///< Initialize "Nothing to show" widget
			void initGenerateImagesControlWidget();                   ///< Initialize control widget for image generation
			void loadGeneratedImagesFromFigures();                    ///< Load images from existing figure files

		private slots:
			void resetView();                                          ///< Slot to reset the view

		public slots:
			void setFields(const QVariantMap &map, const QString &outputDir); ///< Set data fields and output directory

		signals:
			void dataProcessed(const QVariantMap &updatedData);      ///< Signal emitted when data processing is complete
	};
#endif // WIZARDGRACEPAGE_H