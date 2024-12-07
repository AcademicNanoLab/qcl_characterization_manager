#include "widget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

Widget::Widget(QWidget *parent)
    : QWidget{parent}
{}

// [NOT TO BE CALLED]
// This allows QWidget to be styled using qss
void Widget::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
};

// [NOT TO BE CALLED]
// This allows QWdiget to emit clicked() signal
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}
