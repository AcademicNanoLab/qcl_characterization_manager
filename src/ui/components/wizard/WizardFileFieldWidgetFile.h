/**
 * @file WizardFileFieldWidgetFile.h
 * @brief Widget representing a single file entry within a WizardFileFieldWidget.
 *
 * Manages display, editing, and deletion of a file associated with a wizard file field.
 * Provides signals for file deletion and text changes related to the file.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDFILEFIELDWIDGETFILE_H
	#define WIZARDFILEFIELDWIDGETFILE_H

	#include "ui/components/buttons/ButtonGroup.h"
	#include "ui/components/text/Text.h"
	#include "ui/components/containers/Widget.h"

	/**
	 * @class WizardFileFieldWidgetFile
	 * @brief Widget for managing a single file entry in the wizard file field.
	 */
	class WizardFileFieldWidgetFile : public Widget
	{
			Q_OBJECT
		public:
			/**
			 * @brief Constructs a file widget.
			 * @param fileName The name/path of the file.
			 * @param fileVariableName The variable name associated with this file.
			 * @param parent The parent widget.
			 */
			explicit WizardFileFieldWidgetFile(const QString &fileName,
											   const QString &fileVariableName,
											   QWidget *parent = nullptr);

			/**
			 * @brief Parses the file name for display or internal use.
			 * @param fileName The full file name or path.
			 * @return Parsed file name string.
			 */
			QString parseFileName(const QString &fileName);

			/**
			 * @brief Returns a map containing file data (name and associated variable).
			 * @return QMap with file information.
			 */
			QMap<QString, QString> getFileField();

		signals:
			/// Emitted when the file is deleted.
			void fileDeleted(const QString &fileName);

			/// Emitted when the file text changes.
			void fileTextChanged(const QString &fileName, const QString &fileVariableValue);

		public slots:
			/**
			 * @brief Handles text changes related to the file.
			 * @param fileVariableValue The new text value.
			 */
			void textChange(const QString &fileVariableValue)
			{
				this->fileVariableValue = fileVariableValue;
				emit fileTextChanged(fileName, this->fileVariableValue);
			}
			
			/**
			 * @brief Handles deletion button click, emits signal and deletes the widget.
			 */
			void onDeleteButtonClicked()
			{
				emit fileDeleted(fileName);
				Widget::deleteLater();
			}

		private:
			QString fileName;           ///< The file name or path.
			QString fileVariableName;   ///< The associated variable name.
			QString fileVariableValue;  ///< The current text/value related to the file.
	};
#endif // WIZARDFILEFIELDWIDGETFILE_H