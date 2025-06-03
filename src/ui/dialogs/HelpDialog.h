/**
 * @file HelpDialog.h
 * @brief Declaration of HelpDialog, a modal dialog that displays application instructions.
 * 
 * This dialog provides usage instructions for the QCL Characterization Manager.
 * It is invoked from the WelcomePage when the user clicks the Help button.
 */
#ifndef HELPDIALOG_H
	#define HELPDIALOG_H
	#include <QDialog>

	/// @brief Modal dialog that displays help and instructions to the user.
	class HelpDialog : public QDialog 
	{
			Q_OBJECT

		public:
			/// @brief Constructs the HelpDialog with parent widget.
			explicit HelpDialog(QWidget *parent = nullptr);
	};
#endif // HELPDIALOG_H