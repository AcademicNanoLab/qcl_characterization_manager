/**
 * @file ButtonGroup.cpp
 * @brief Implementation of a customizable button group container with support for custom and image buttons.
 * 
 * This class extends QGroupBox and manages a dynamic collection of buttons, either standard or custom types.
 * Buttons can be arranged vertically or horizontally and emit signals on interaction.
 * 
 * The class supports connecting buttons to external object signals, customizing button appearance based on file input,
 * and clearing or initializing the button group.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "ButtonGroup.h"
#include <QAbstractButton>
#include <QLayout>
#include <QDebug>
#include <QFileInfo>

/**
 * @brief Constructs a ButtonGroup with specified layout orientation.
 * 
 * Initializes the button group with either vertical or horizontal layout and sets up signal connections
 * for internal button handling.
 * 
 * @param boxLayout Determines if the layout is vertical or horizontal.
 * @param parent The parent widget.
 */
ButtonGroup::ButtonGroup(BoxLayout boxLayout, QWidget *parent)
    : QGroupBox{parent},
      buttonGroup(std::make_unique<QButtonGroup>(this))  // Smart pointer initialization
{
    if (boxLayout == BoxLayout::VLayout) {
        setLayout(new QVBoxLayout());
        layout()->setAlignment(Qt::AlignTop);
    } else {
        setLayout(new QHBoxLayout());
        layout()->setAlignment(Qt::AlignLeft);
    }
    setFlat(true);

    connect(buttonGroup.get(), &QButtonGroup::idClicked, this, &ButtonGroup::idClickedSlot);
    connect(buttonGroup.get(), &QButtonGroup::buttonClicked, this, &ButtonGroup::buttonClickedSlot);
}

/**
 * @brief Adds a button to the group with an optional custom ID.
 * 
 * If no ID is provided, a unique internal ID is assigned. The button is added to both the layout and the internal QButtonGroup.
 * 
 * @param button The button to be added.
 * @param id Optional ID to assign to the button.
 */
void ButtonGroup::addButton(QAbstractButton *button, int id)
{
    if (id == -1)
        id = currentButtonId++;  // Use internal counter for unique IDs
    buttonGroup->addButton(button, id);  // Add buttons (group)
    layout()->addWidget(button);        // Add buttons (UI)
}

/**
 * @brief Adds a custom button of a specified type to the group.
 * 
 * Creates and adds a button of the given template type, sets its text using the base name of the input string (typically a file),
 * and connects it to a signal on the given object.
 * 
 * @tparam ButtonType The custom button class type.
 * @param object The source object emitting the signal.
 * @param changeSignal The signal to connect from the object.
 * @param arg Argument (e.g. text or filepath) passed to the button constructor.
 * @param id Optional button ID.
 */
template <typename ButtonType>
void ButtonGroup::addCustomButton(const QObject *object, const char *changeSignal, const QString &arg, int id)
{
    if (id == -1)
        id = currentButtonId++;
    ButtonType *button = new ButtonType(id, arg);
    button->setText(QFileInfo(arg).completeBaseName());  // Filename without extension
    addButton(button, id);

    connectButtonToObjectSignal(object, changeSignal, button);
}

/**
 * @brief Connects a button to a signal from an external object.
 * 
 * Used internally to set up connections between external signals and the button's slot.
 * Initializes the button state by simulating a selection with ID 0.
 * 
 * @param object The signal-emitting object.
 * @param changeSignal The signal to connect.
 * @param button The button to connect to the signal.
 */
void ButtonGroup::connectButtonToObjectSignal(const QObject *object, const char *changeSignal, ButtonGroupButton *button)
{
    if (!object) {
        qWarning() << "Invalid object provided to connectButtonToObjectSignal.";
        return;
    }
    // Now we can connect a signal from a const object.
    connect(object, changeSignal, button, SLOT(buttonClickedIdSlot(int)));
    button->buttonClickedIdSlot(0);  // Initialize button state
}

/**
 * @brief Adds a standard text-based menu button to the group.
 * 
 * Internally uses a template method to create a ButtonGroupButton with the given text.
 * 
 * @param object The signal-emitting object.
 * @param changeSignal The signal to connect.
 * @param text The text label for the button.
 * @param id Optional button ID.
 */
void ButtonGroup::addMenuButton(const QObject *object, const char *changeSignal, const QString &text, int id)
{
    addCustomButton<ButtonGroupButton>(object, changeSignal, text, id);  // Use template method for adding ButtonGroupButton
}

/**
 * @brief Adds an image-based button to the group.
 * 
 * Internally uses a template method to create an ImageButton with the given image file path.
 * 
 * @param object The signal-emitting object.
 * @param changeSignal The signal to connect.
 * @param filePath The path to the image file used by the button.
 * @param id Optional button ID.
 */
void ButtonGroup::addImageButton(const QObject *object, const char *changeSignal, const QString &filePath, int id)
{
    addCustomButton<ImageButton>(object, changeSignal, filePath, id);  // Use template method for adding ImageButton
}

/**
 * @brief Initializes the first button in the group by simulating a click.
 * 
 * Useful for setting a default active button on startup.
 */
void ButtonGroup::initButton()
{
    if (this->buttonGroup->button(0)) 
	{
        this->buttonGroup->button(0)->animateClick();
    }
}

/**
 * @brief Clears all buttons from the group and deletes them safely.
 * 
 * Disconnects and schedules deletion of all buttons managed by the internal QButtonGroup.
 */
void ButtonGroup::clear()
{
    // Remove buttons from QButtonGroup and delete all button widgets found as children
    const auto buttons = buttonGroup->buttons();
    for (QAbstractButton* button : buttons) {
        buttonGroup->removeButton(button);  // Remove from group
        button->deleteLater();               // Delete widget safely
    }
}