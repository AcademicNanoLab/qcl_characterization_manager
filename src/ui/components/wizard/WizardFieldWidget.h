/**
 * @file WizardFieldWidget.h
 * @brief Base widget class for wizard input fields with error handling and layout.
 *
 * Provides a layout container with label and error display, supporting multiple layouts.
 * Emits signals on value changes and manages error states.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDFIELDWIDGET_H
	#define WIZARDFIELDWIDGET_H

	#include <QFileDialog>
	#include <QLineEdit>
	#include <QRegularExpression>
	#include <QVBoxLayout>
	#include <QVariant>
	#include "ui/components/containers/Widget.h"
	#include "ui/components/text/Text.h"
	#include "WizardField.h"
	#include "WizardPage.h"

	/**
	 * @class WizardFieldWidget
	 * @brief Abstract base widget for wizard fields supporting validation and error display.
	 */
	class WizardFieldWidget : public Widget
	{
			Q_OBJECT
		public:
			enum class BoxLayout { VLayout, HLayout }; ///< Layout types for the widget.
			enum class ErrorState { NoError, StringEmpty, DoubleLimit, MapError }; ///< Possible error states.

			explicit WizardFieldWidget(WizardPage *page,
									   const QString &name,
									   WizardField::WizardFieldType wizardFieldType,
									   QWidget *parent = nullptr); ///< Constructor with default vertical layout.

			explicit WizardFieldWidget(WizardPage *page,
									   const QString &name,
									   WizardField::WizardFieldType wizardFieldType,
									   WizardFieldWidget::BoxLayout boxLayout,
									   QWidget *parent = nullptr); ///< Constructor with selectable layout.

			virtual void clear() = 0; ///< Clears the widget input (pure virtual).

			void setErrorState(ErrorState _errorState); ///< Sets the error state and updates UI.

			void addLayout(QLayout *layout); ///< Adds a layout to the widget.

			void addWidget(QWidget *widget); ///< Adds a child widget.

			WizardField *field; ///< Associated wizard field data.

		signals:
			void changeSignal(const QVariant &value); ///< Emitted when value changes.

		public slots:
			void changeSignalSlot(QVariant value) ///< Slot to emit changeSignal and reset error state.
			{
				emit changeSignal(value);
				setErrorState(ErrorState::NoError);
			};

		public:
			Widget *fieldArea;     ///< Container area for the field input.
			Widget *errorArea;     ///< Container area for error display.
			Text *errorAreaText;   ///< Text widget to show error messages.
			ErrorState errorState; ///< Current error state of the widget.
	};
#endif // WIZARDFIELDWIDGET_H