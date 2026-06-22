/**
 * @file DirectorySelector.h
 * @brief Declaration of DirectorySelector for directory selection UI.
 *
 * Defines a widget that allows users to choose a directory via a file dialog.
 * Emits a signal when the directory changes and integrates with other components
 * through passed-in signal/slot connections.
 * 
 * @author Aleksandar Demic
 */

#ifndef DIRECTORYSELECTOR_H
	#define DIRECTORYSELECTOR_H

	#include <QFileDialog>
	#include <QLineEdit>
	#include <QWidget>
	#include <QStandardPaths>

	/**
	 * @class DirectorySelector
	 * @brief Widget that allows users to select a directory.
	 *
	 * Provides a text box and file dialog to select and display directories.
	 * Emits signals on changes and supports dynamic signal-slot integration.
	 */
	class DirectorySelector : public QWidget
	{
		Q_OBJECT
		public:
			explicit DirectorySelector(QObject *dirChangeObject,
									   const char *dirChangeAction,
									   const QString &buttonText,
									   QObject *buttonClickObject,
									   const char *buttonClickAction,
									   QWidget *parent = nullptr); ///< Constructs the directory selector widget.

		signals:
			void dirChanged(const QString &dir); ///< Emitted when the directory is changed.

		public slots:
			void chooseDir(); ///< Opens file dialog to choose a directory.

		private:
			QLineEdit *chooseDirTextEdit; ///< Text box displaying selected directory.
	};
#endif // DIRECTORYSELECTOR_H