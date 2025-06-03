/**
 * @file DoubleLineEdit.h
 * @brief QLineEdit subclass emitting text changes with an associated name.
 *
 * Extends QLineEdit to emit a signal including a custom name and the new text value
 * whenever the text changes.
 * 
 * @author Aleksandar Demic
 */

#ifndef DOUBLELINEEDIT_H
	#define DOUBLELINEEDIT_H

	#include <QLineEdit>

	/**
	 * @class DoubleLineEdit
	 * @brief Line edit that emits named text change signals.
	 *
	 * Emits _textChanged signal with a name identifier and the current text.
	 */
	class DoubleLineEdit : public QLineEdit
	{
			Q_OBJECT
		public:
			explicit DoubleLineEdit(const QString &name, QWidget *parent = nullptr); ///< Constructs with name and optional parent.

		signals:
			void _textChanged(const QString &name, const QString &value); ///< Emitted when text changes, includes name.

		private slots:
			void textChangedSlot(const QString &value); ///< Slot connected to QLineEdit text changes.

		private:
			QString _name; ///< Identifier name associated with this line edit.
	};
#endif // DOUBLELINEEDIT_H
