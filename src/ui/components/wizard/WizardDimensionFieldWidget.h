/**
 * @file WizardDimensionFieldWidget.h
 * @brief Widget for inputting dimensions (length × width × height).
 *
 * Provides input fields for dimensions and emits a map of updated values on change.
 * Clears all input fields on clear().
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDDIMENSIONFIELDWIDGET_H
	#define WIZARDDIMENSIONFIELDWIDGET_H

	#include <QVariant>
	#include "DoubleLineEdit.h"
	#include "WizardFieldWidget.h"
	#include "WizardPage.h"

	/**
	 * @class WizardDimensionFieldWidget
	 * @brief Widget handling length × width × height dimension inputs.
	 *
	 * Maintains a map of dimension values keyed by their names and emits signals on changes.
	 */
	class WizardDimensionFieldWidget : public WizardFieldWidget
	{
			Q_OBJECT
		public:
			explicit WizardDimensionFieldWidget(WizardPage *page,
												const QString &name,
												QWidget *parent = nullptr); ///< Constructor with page, name, and optional parent.

			void clear() ///< Clears all dimension input fields.
			{
				for (DoubleLineEdit *textField : textFields)
					textField->setText("");
			}

			QMap<QString, QVariant> valueMap; ///< Stores current dimension values by name.

		public slots:
			void onDimensionTextChanged(QString name, QString value) ///< Updates value map and emits change signal.
			{
				valueMap[name] = value;
				emit changeSignal(valueMap);
				setErrorState(ErrorState::NoError);
			}

		private:
			QList<DoubleLineEdit *> textFields; ///< Input fields for each dimension.
	};
#endif // WIZARDDIMENSIONFIELDWIDGET_H