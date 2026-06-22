/**
 * @file ButtonGroup.h
 * @brief Declaration of ButtonGroup combining QButtonGroup and QGroupBox functionality.
 *
 * Provides a container widget managing a group of buttons with unique IDs.
 * Supports adding menu buttons, image buttons, and generic buttons.
 * Handles signal forwarding and layout management for consistent UI behavior.
 * 
 * @author Aleksandar Demic
 */

#ifndef BUTTONGROUP_H
	#define BUTTONGROUP_H
	#include <QAbstractButton>
	#include <QButtonGroup>
	#include <QGroupBox>
	#include <QStyle>
	#include <memory> 
	#include "PushButton.h"
	#include "ButtonGroupButton.h"
	#include "ImageButton.h" 

	class Wizard;

	/**
	 * @class ButtonGroup
	 * @brief Combines QButtonGroup and QGroupBox to manage button collections.
	 *
	 * Supports adding buttons of various types with unique IDs,
	 * connects button signals to external objects, and manages layout.
	 */
	class ButtonGroup : public QGroupBox
	{
			Q_OBJECT
			std::unique_ptr<QButtonGroup> buttonGroup;  ///< Smart pointer managing internal QButtonGroup.

		private:
			int currentButtonId = 0;  ///< Tracks unique IDs for buttons added.

			/// Connects a signal from an object to a button's slot.
			void connectButtonToObjectSignal(const QObject *object, const char *changeSignal, ButtonGroupButton *button);

			/// Template to add a custom button of specified type, reducing code duplication.
			template <typename ButtonType>
			void addCustomButton(const QObject *object, const char *changeSignal, const QString &arg, int id = -1);

		public:
			/// Enum for selecting button group layout orientation.
			enum BoxLayout { VLayout, HLayout };

			explicit ButtonGroup(BoxLayout boxLayout, QWidget *parent = nullptr); ///< Constructs button group with layout.

			void addButton(QAbstractButton *button, int id = -1); ///< Adds a generic button with optional ID.
			void addMenuButton(const QObject *object, const char *changeSignal, const QString &text, int id = -1); ///< Adds a text menu button.
			void addImageButton(const QObject *object, const char *changeSignal, const QString &filePath, int id = -1); ///< Adds an image button.

			void initButton(); ///< Initializes internal button group and settings.
			void clear(); ///< Clears all buttons from the group.

		signals:
			void buttonClicked(int id); ///< Emitted on generic button click.
			void buttonClickedId(int id); ///< Emitted with button ID.
			void buttonClickedText(QString id); ///< Emitted with button text.
			void buttonClickedPtr(QAbstractButton *button); ///< Emitted with button pointer.

		public slots:
			void idClickedSlot(int id) ///< Slot for button click by ID.
			{
				emit buttonClickedId(id);
				emit buttonClicked(0);
			}

			void buttonClickedSlot(QAbstractButton *button) ///< Slot for button click by pointer.
			{
				emit buttonClickedText(button->text());
				emit buttonClickedPtr(button);
			}
	};
#endif // BUTTONGROUP_H