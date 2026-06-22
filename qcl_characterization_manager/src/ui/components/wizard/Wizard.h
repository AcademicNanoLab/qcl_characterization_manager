/**
 * @file Wizard.h
 * @brief Declaration of Wizard widget, a simplified customizable QWizard replacement.
 *
 * Provides a stacked widget-based wizard with pages and navigation buttons.
 * Supports adding pages, retrieving wizard data, and finishing actions.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARD_H
	#define WIZARD_H

	#include <QStackedWidget>
	#include "ui/components/buttons/ButtonGroup.h"
	#include "ui/components/containers/widget.h"
	#include "WizardPage.h"

	/**
	 * @class Wizard
	 * @brief Custom wizard widget based on QStackedWidget.
	 *
	 * Manages wizard pages and navigation buttons, emitting signals on finish.
	 * Must implement finishWizardAction() for finish behavior.
	 */
	class Wizard : public Widget
	{
			Q_OBJECT
		public:
			explicit Wizard(QWidget *parent = nullptr); ///< Constructs wizard with optional parent.
			void addPage(WizardPage *page); ///< Adds a wizard page.
			void getWizardFields(); ///< Retrieves data from wizard pages.
			void finishWizard(); ///< Performs finishing steps.
			virtual void finishWizardAction() = 0; ///< Abstract method to handle finishing action.
			QStackedWidget *wizardPages; ///< Container for wizard pages.

		signals:
			void finished(int mainStackIdx); ///< Emitted when wizard finishes.

		public slots:
			void setCurrentIndexWrapper(int id); ///< Sets current page index.
			void wizardButtonsClick(int id); ///< Handles wizard button clicks.

		private:
			ButtonGroup *wizardMenu;    ///< Navigation menu for wizard pages.
			ButtonGroup *wizardButtons; ///< Control buttons for wizard (Next, Back, Finish).
	};
#endif // WIZARD_H
