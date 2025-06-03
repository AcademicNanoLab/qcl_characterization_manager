/**
 * @file PushButton.h
 * @brief Declaration of PushButton with customizable variant property for styling.
 *
 * Extends QPushButton to include a 'variant' property allowing flexible styling via Qt stylesheets.
 * Provides constructors to initialize with icon or text and variant.
 * 
 * @author Aleksandar Demic
 */

#ifndef PUSHBUTTON_H
	#define PUSHBUTTON_H
	#include <QPushButton>
	#include <QStyle>

	/**
	 * @class PushButton
	 * @brief QPushButton subclass supporting a variant property for style customization.
	 */
	class PushButton : public QPushButton
	{
			Q_OBJECT
			Q_PROPERTY(QString variant READ getVariant WRITE setVariant NOTIFY variantChanged)

		public:
			explicit PushButton(const QIcon &icon, QWidget *parent = nullptr); ///< Constructs button with icon.
			explicit PushButton(const QString &text, const QString &variant, QWidget *parent = nullptr); ///< Constructs button with text and variant.

			QString getVariant() const; ///< Returns the current variant.
			void setVariant(const QString &variant); ///< Sets the variant and triggers update.

		signals:
			void variantChanged(const QString &variant); ///< Emitted when the variant changes.

		private:
			QString variant; ///< Stores the variant string for styling.
	};
#endif // PUSHBUTTON_H