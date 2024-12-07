#include "pushbutton.h"

PushButton::PushButton(const QIcon &icon, QWidget *parent)
    : QPushButton(icon, "", parent)
{
    this->setVariant("text");
    this->setCursor(Qt::PointingHandCursor);
}

PushButton::PushButton(const QString &text, const QString &variant, QWidget *parent)
    : QPushButton{text, parent}
{
    this->setVariant(variant);
    this->setCursor(Qt::PointingHandCursor);
}

QString PushButton::getVariant()
{
    return variant;
}

void PushButton::setVariant(QString _variant)
{
    variant = _variant;
}

/**************************************** ButtonGroupButton *****************************************/
ButtonGroupButton::ButtonGroupButton(int id,
                                     const QString &text,
                                     const QString &variant,
                                     QWidget *parent)
    : PushButton(text, variant, parent)
    , id(id)
{}

/**************************************** ImageButton *****************************************/
ImageButton::ImageButton(int id, const QString &filePath, QWidget *parent)
    : ButtonGroupButton{id, "", "", parent}
    , filePath(filePath)
{
    QPixmap pixmapOriginal(filePath);
    QPixmap pixmap = pixmapOriginal.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QIcon icon(pixmap);
    this->setIcon(icon);
    this->setIconSize(pixmap.rect().size());
}
