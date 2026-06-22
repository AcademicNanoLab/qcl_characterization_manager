/**
 * @file WizardRadioFieldWidget.h
 * @brief Widget for radio button input in a wizard page.
 * 
 * Provides a radio button group with selectable options,
 * allowing user selection within a wizard field.
 * Supports clearing (resetting) selections.
 * 
 * @author Aleksandar Demic
 */

#ifndef WIZARDRADIOFIELDWIDGET_H
	#define WIZARDRADIOFIELDWIDGET_H

	#include "ui/components/buttons/ButtonGroup.h"
	#include "WizardFieldWidget.h"
	#include "WizardPage.h"

	/**
	 * @class WizardRadioFieldWidget
	 * @brief Radio button input widget for wizard fields.
	 */
	class WizardRadioFieldWidget : public WizardFieldWidget
	{
			Q_OBJECT
		public:
			explicit WizardRadioFieldWidget(WizardPage *page,
											const QString &name,
											QList<QString> optionsList,
											const BoxLayout boxLayout,
											QWidget *parent = nullptr); ///< Constructor initializing radio options and layout
			void clear() { radioButtonGroup->initButton(); } ///< Clear selection by resetting buttons

		private:
			ButtonGroup *radioButtonGroup; ///< Group managing radio buttons
	};
#endif // WIZARDRADIOFIELDWIDGET_H