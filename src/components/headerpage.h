#ifndef HEADERPAGE_H
#define HEADERPAGE_H

#include <QScrollArea>
#include "widget.h"

// Page with a header and scrollable vertical layout.
// Add items using addWidget.
class HeaderPage : public QScrollArea
{
    Q_OBJECT
public:
    explicit HeaderPage(const QString &title, QWidget *parent = nullptr);
    void addWidget(QWidget *widget);
    QString getTitle();

signals:

private:
    QString title;
    Widget *contents;
};

#endif // HEADERPAGE_H
