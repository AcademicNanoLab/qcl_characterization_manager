/**
 * @file MessageBox.h
 * @brief Wrapper around QMessageBox to customize button cursor behavior.
 *
 * This class extends QMessageBox to ensure the cursor changes to a pointer
 * when hovering over buttons, improving user experience in dialogs.
 * 
 * @author Aleksandar Demic
 */

#ifndef MESSAGEBOX_H
	#define MESSAGEBOX_H

	#include <QMessageBox>

	/**
	 * @class MessageBox
	 * @brief Customized message box with pointer cursor on buttons.
	 */
	class MessageBox : public QMessageBox
	{
		Q_OBJECT
		public:
			explicit MessageBox(QMessageBox::Icon icon,
								const QString &title,
								const QString &text,
								QMessageBox::StandardButtons buttons = NoButton,
								QWidget *parent = nullptr,
								Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); ///< Constructs a message box with custom cursor behavior.
	};
#endif // MESSAGEBOX_H