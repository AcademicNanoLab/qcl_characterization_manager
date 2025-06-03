/**
 * @file WizardFileFieldWidget.h
 * @brief Widget for file input fields in the wizard, supporting file selection and management.
 *
 * Allows selecting, displaying, and removing files associated with a wizard field.
 * Emits signals on file changes and manages file-related UI elements.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDFILEFIELDWIDGET_H
	#define WIZARDFILEFIELDWIDGET_H
	#include <QFileDialog>
	#include <QVBoxLayout>
	#include <QVariant>
	#include "ui/components/containers/Widget.h"
	#include "WizardFieldWidget.h"
	#include "WizardPage.h"

	/**
	 * @class WizardFileFieldWidget
	 * @brief Widget for handling file inputs within a wizard page.
	 */
	class WizardFileFieldWidget : public WizardFieldWidget
	{
			Q_OBJECT
		public:
			explicit WizardFileFieldWidget(WizardPage *page,
										   const QString &name,
										   const QString &fileVariableName,
										   QWidget *parent = nullptr); ///< Constructor with file variable name.

			void clear(); ///< Clears the selected files.

			QMap<QString, QVariant> fileMap;        ///< Map storing file names and related data.
			QString fileVariableName;                ///< Variable name associated with the file input.
			QFileDialog *fileDialog;                 ///< Dialog for selecting files.
			QString downloadPath;                    ///< Path where files are downloaded or stored.
			Widget *fileFieldScrollContents;        ///< Scroll area contents widget for files.
			QVBoxLayout *fileFieldScrollLayout;     ///< Layout managing file widgets.

		signals:
			void fileChangeSignal(const QVariantMap &map); ///< Emitted when files change.

		public slots:
			void fileSelected(const QString &fileName); ///< Slot for when a file is selected.
			void addButtonClicked();                      ///< Slot for add button clicked.
			void fileTextChangedSlot(const QString &fileName, const QString &fileVariableValue); ///< Slot for text change in file entry.
			void fileDeletedSlot(const QString &fileName); ///< Slot for when a file is deleted.

		private:
			void updateFileWidget(const QString &fileName); ///< Updates UI widget for a specific file.
	};
#endif // WIZARDFILEFIELDWIDGET_H