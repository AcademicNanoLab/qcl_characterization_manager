#ifndef IMAGECAROUSEL_H
#define IMAGECAROUSEL_H

#include <QStackedWidget>
#include "plotComponents/qcustomplot.h"
#include "widget.h"

class ImageCarousel : public Widget
{
    Q_OBJECT
public:
    explicit ImageCarousel(QWidget *parent = nullptr);
    void addImage(const QString &imagePath);
    void addImage(QCustomPlot *plot);
    void setImageStackIndex(int index);
    void clear();

    QStackedWidget *imageStack;

signals:
    void currentChanged(int id);

public slots:
    void prevClicked() { setImageStackIndex(imageStack->currentIndex() - 1); }
    void nextClicked() { setImageStackIndex(imageStack->currentIndex() + 1); }
    void currentChangedSlot(int id) { emit currentChanged(id); }

private:
};

#endif // IMAGECAROUSEL_H
