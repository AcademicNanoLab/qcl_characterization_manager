/**
 * @file WizardPage.h
 * @brief Header for WizardPage managing a wizard step with multiple fields.
 * 
 * Provides a container for WizardFields within a wizard step,
 * enabling validation, field collection, clearing, and file writing.
 * Inherits from HeaderPage for scrollable content and title display.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDPAGE_H
	#define WIZARDPAGE_H

	#include <QFile>
	#include "ui/components/containers/HeaderPage.h"
	#include "WizardField.h"

	/**
	 * @class WizardPage
	 * @brief Represents a single page in a wizard containing multiple fields.
	 */
	class WizardPage : public HeaderPage
	{
			Q_OBJECT
		public:
			explicit WizardPage(const QString &title, QWidget *parent = nullptr); ///< Constructor with title and optional parent
			void addField(WizardField *wizardField); ///< Add a WizardField to the page
			void getWizardFields(QString &wizardFieldString); ///< Collect all field data into a string
			bool pageValid(); ///< Validate all fields on the page
			void clear(); ///< Clear all fields
			void writeToFile(QFile *file); ///< Write page data to a file
			virtual void addToMap(QVariantMap &map); ///< Add page data to a QVariantMap

		private:
			QList<WizardField *> wizardFieldList; ///< List of fields on the page
	};
#endif // WIZARDPAGE_H
