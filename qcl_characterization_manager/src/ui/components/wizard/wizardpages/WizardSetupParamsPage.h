/**
 * @file WizardSetupParamsPage.h
 * @brief Wizard page for setting up parameters, including output directory selection.
 * 
 * This page provides UI elements to configure setup parameters
 * and retrieve the selected output directory.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDSETUPPARAMSPAGE_H
	#define WIZARDSETUPPARAMSPAGE_H

	#include "ui/components/wizard/wizardpage.h"

	class QLineEdit;  ///< Forward declaration of QLineEdit

	/**
	 * @class WizardSetupParamsPage
	 * @brief Page for setting and retrieving setup parameters in the wizard.
	 */
	class WizardSetupParamsPage : public WizardPage
	{
			Q_OBJECT
		public:
			explicit WizardSetupParamsPage(QWidget *parent = nullptr); ///< Constructor

			QString getOutputDirectory() const; ///< Get the output directory path

		private:
			QLineEdit *dirLineEdit; ///< Directory input field widget
	};
#endif // WIZARDSETUPPARAMSPAGE_H