#include "messagebox.h"
#include <QAbstractButton>

MessageBox::MessageBox(QMessageBox::Icon icon,
                       const QString &title,
                       const QString &text,
                       QMessageBox::StandardButtons buttons,
                       QWidget *parent,
                       Qt::WindowFlags f)
    : QMessageBox{icon, title, text, buttons, parent, f}
{
    // set cursor to pointer
    for (QAbstractButton *button : this->buttons())
        button->setCursor(Qt::PointingHandCursor);
}
