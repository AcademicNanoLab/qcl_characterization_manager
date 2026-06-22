/**
 * @file WizardField.h
 * @brief Represents a field in the wizard with validation and signal connection.
 *
 * Supports multiple field types and manages connection to UI widgets and data validation.
 * Provides methods to get field data and reset error state.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDFIELD_H
	#define WIZARDFIELD_H
	#include <QObject>
	#include <QVariant>

	class WizardFieldWidget;

	/**
	 * @class WizardField
	 * @brief Encapsulates a wizard input field with validation and data handling.
	 */
	class WizardField : public QObject
	{
			Q_OBJECT
		public:
			enum class WizardFieldType { StringField, DoubleField, DimensionField, FileField }; ///< Types of wizard fields.

			explicit WizardField(const QString &name,
								 WizardFieldType wizardFieldType,
								 const QObject *object,
								 const char *changedSignal); ///< Constructs WizardField connecting to signal.

			void connectFileField(const QObject *object,
								  const char *changedSignal,
								  const QString &fileVariableName); ///< Connects file field signals.

			void getWizardField(QString &wizardFieldString); ///< Retrieves field value as string.

			void addToMap(QVariantMap &map); ///< Adds field data to QVariantMap.

			bool fieldValid(); ///< Validates the current field value.

			bool fieldValid(QString &value); ///< Validates given QString value.

			bool fieldValid(double value); ///< Validates given double value.

			bool fieldValid(QVariantMap &map); ///< Validates given QVariantMap value.

			bool resetErrorState(); ///< Resets error state on associated widget.

		private slots:
			void fieldChange(const QVariant &value); ///< Slot for field value changes.

			void fieldChange(const QVariantMap &value); ///< Slot for QVariantMap value changes.

		private:
			QString name;                     ///< Field name identifier.
			QString fileVariableName;         ///< File variable name for file fields.
			QVariant value;                   ///< Current value of the field.
			WizardFieldType wizardFieldType; ///< Type of the wizard field.
			WizardFieldWidget *widget;        ///< Associated UI widget.

			bool noFileSelected(); ///< Checks if no file is selected (for file fields).
	};
#endif // WIZARDFIELD_H