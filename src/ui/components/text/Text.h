/**
 * @file Text.h
 * @brief Declaration of Text QLabel subclass with variant property for styling.
 *
 * Extends QLabel to include a 'variant' property to support different text styles.
 * Emits signal when variant changes.
 * 
 * @author Aleksandar Demic
 */

#ifndef TEXT_H
	#define TEXT_H
	#include <QLabel>

	/**
	 * @class Text
	 * @brief QLabel subclass with a style variant property.
	 *
	 * Allows changing and notifying the style variant to support QSS styling.
	 */
	class Text : public QLabel
	{
			Q_OBJECT
			Q_PROPERTY(QString variant READ getVariant WRITE setVariant NOTIFY variantChanged)

		public:
			explicit Text(const QString &text, const QString &variant = "body", QWidget *parent = nullptr); ///< Constructs Text with initial string and variant.

			QString getVariant() const; ///< Returns current variant.

			void setVariant(const QString &variant); ///< Sets variant and emits variantChanged signal.

		signals:
			void variantChanged(const QString &variant); ///< Emitted when variant changes.

		private:
			QString variant; ///< Holds current variant string.
	};
#endif // TEXT_H