/**
 * @file MainWindow.h
 * @brief Main application window class declaration.
 * 
 * Defines the main GUI window, managing the primary user interface.
 * 
 * @author Aleksandar Demic
 */

#ifndef MAINWINDOW_H
	#define MAINWINDOW_H

	#include <QMainWindow>

	QT_BEGIN_NAMESPACE
	namespace Ui 
	{
		class MainWindow;
	}
	QT_END_NAMESPACE

	/**
	 * @class MainWindow
	 * @brief Main window class for the application.
	 */
	class MainWindow : public QMainWindow
	{
			Q_OBJECT

		public:
			explicit MainWindow(QWidget *parent = nullptr); ///< Constructor
			~MainWindow();                                   ///< Destructor

		private:
			Ui::MainWindow *ui; ///< UI object pointer
	};
#endif // MAINWINDOW_H