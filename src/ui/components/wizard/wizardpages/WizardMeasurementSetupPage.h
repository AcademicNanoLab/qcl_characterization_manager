/**
 * @file WizardMeasurementSetupPage.h
 * @brief Wizard page for setting up measurement parameters.
 * 
 * Handles UI fields for configuring measurement setup,
 * supports pulsed mode configuration, and data mapping.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDMEASUREMENTSETUPPAGE_H
	#define WIZARDMEASUREMENTSETUPPAGE_H

	#include "ui/components/wizard/WizardPage.h"
	#include <QMap>
	#include <QStringList>
	#include <QComboBox>
	#include <QLineEdit>

	/**
	 * @class WizardMeasurementSetupPage
	 * @brief Wizard page managing measurement setup parameters.
	 */
	class WizardMeasurementSetupPage : public WizardPage
	{
			Q_OBJECT

		public:
			explicit WizardMeasurementSetupPage(bool pulsedMode, QWidget *parent = nullptr); ///< Constructor with pulsed mode option
			void addToMap(QVariantMap &map) override;                                       ///< Add page data to map
			bool validatePage();                                                            ///< Validate page input fields

		private:
			QMap<QString, QWidget*> fieldWidgets; ///< Map of field keys to their widgets
			bool pulsed;                          ///< Flag indicating pulsed mode
			QString keyPrefix;                    ///< Key prefix for data mapping

			QStringList loadResourceList(const QString &filename); ///< Load resource list from file

			void addLineEditField(QGridLayout *layout, 
								  const QString &key, 
								  const QString &label, 
								  const QString &placeholder, 
								  int row, int colPrefix);             ///< Add line edit field to layout

			void addDropdownField(QGridLayout *layout, 
								  const QString &key, 
								  const QString &label, 
								  const QString &resourceFile, 
								  int row, int colPrefix);             ///< Add dropdown field to layout

			void setDefaults();                                             ///< Set default values for fields
	};
#endif // WIZARDMEASUREMENTSETUPPAGE_H