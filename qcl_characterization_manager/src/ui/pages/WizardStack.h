/**
 * @file WizardStack.h
 * @brief Declaration of WizardStack, a multi-step wizard for experiment configuration.
 *
 * Defines the WizardStack class which orchestrates navigation through multiple
 * wizard pages for setting up, validating, and exporting measurement configurations.
 * Handles user input, data collection, and error reporting across several steps.
 * 
 * Author: Aleksandar Demic
 */

#ifndef WIZARDSTACK_H
	#define WIZARDSTACK_H

	#include "ui/components/wizard/Wizard.h"
	#include "ui/components/wizard/WizardPage.h"
	#include "ui/components/wizard/wizardpages/WizardGracePage.h"
	#include "ui/components/wizard/wizardpages/WizardMeasurementSetupPage.h"
	#include "ui/components/wizard/wizardpages/WizardSetupParamsPage.h"
	#include <QVariantMap>
	#include <QString>
	#include <QDir>
	#include <QFileDialog>
	#include <QMessageBox>

	/**
	 * @class WizardStack
	 * @brief Multi-step wizard controller for experiment setup and data flow.
	 *
	 * Manages transitions between wizard pages, collects input data, and signals
	 * results or errors to external components. Supports multiple measurement modes.
	 */
	class WizardStack : public Wizard
	{
		Q_OBJECT

		public:
			explicit WizardStack(QWidget *parent = nullptr); ///< Constructs the wizard stack controller.
			void finishWizardAction() override; ///< Finalizes the wizard process and emits collected data.

		signals:
			void sendFields(const QVariantMap &map, const QString &outputDirectory); ///< Emitted when wizard data is ready.
			void fileErrorOccurred(const QString &errorMessage); ///< Emitted if file-related error occurs.

		public slots:
			void currentChangedSlot(int id); ///< Responds to page change within the wizard.
			void handleFileError(const QString &errorMessage); ///< Handles file-related error reporting.
			void updateCollectedDataFromGracePage(const QVariantMap &updatedData); ///< Updates internal data from Grace page.

		private:
			WizardGracePage *gracePage = nullptr; ///< Page for grace plot setup.
			WizardSetupParamsPage *setupParamsPage = nullptr; ///< Page for defining setup parameters.
			WizardMeasurementSetupPage *measurementPagePulsed = nullptr; ///< Measurement page for pulsed mode.
			WizardMeasurementSetupPage *measurementPageCW = nullptr; ///< Measurement page for continuous wave mode.
			QList<WizardPage *> additionalPages; ///< List of additional wizard pages (e.g., file pages).
			QVariantMap collectedData; ///< Internal store for collected wizard data.
	};
#endif // WIZARDSTACK_H