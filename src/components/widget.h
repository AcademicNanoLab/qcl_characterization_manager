#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

// Overrides QWidget so that it can be styled using qss and emits clicked() signal
class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *pe);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void clicked();
};

#endif // WIDGET_H
