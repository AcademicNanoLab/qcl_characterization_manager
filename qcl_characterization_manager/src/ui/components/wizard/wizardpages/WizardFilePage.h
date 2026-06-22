/**
 * @file WizardFilePage.h
 * @brief Wizard page for handling file-related input and notes.
 * 
 * This page allows input of file-related information and notes,
 * supports setting a custom key base for data mapping,
 * and manages text editing for notes.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDFILEPAGE_H
	#define WIZARDFILEPAGE_H

	#include "ui/components/wizard/WizardPage.h"
	#include "ui/components/wizard/WizardFieldWidget.h"

	#include <QTextEdit>  

	/**
	 * @class WizardFilePage
	 * @brief Wizard page managing file input and associated notes.
	 */
	class WizardFilePage : public WizardPage
	{
			Q_OBJECT

		public:
			explicit WizardFilePage(const QString &title,
									const QString &fileVariableName,
									QWidget *parent = nullptr); ///< Constructor

			void setKeyBase(const QString &key); ///< Set custom key base for map keys
			void addToMap(QVariantMap &map, const QString &keyPrefix = ""); ///< Add fields to map with optional prefix

		private:
			QTextEdit *notesEdit = nullptr; ///< Text edit widget for notes
			QString getKeyBase() const;      ///< Get the current key base string

			QString keyBase; ///< Custom key base string for data keys
	};
#endif // WIZARDFILEPAGE_H
