#include "text.h"

Text::Text(const QString &text, const QString &variant, QWidget *parent)
    : QLabel{text, parent}
{
    setVariant(variant);
    setIndent(0);      // make text align to left without any padding
    setWordWrap(true); // make text wrap
}

QString Text::getVariant()
{
    return variant;
}

void Text::setVariant(QString newVariant)
{
    variant = newVariant;
    emit variantChanged(newVariant);
}
