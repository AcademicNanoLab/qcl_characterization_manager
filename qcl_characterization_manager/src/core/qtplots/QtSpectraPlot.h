#ifndef QTSPECTRAPLOT_H
#define QTSPECTRAPLOT_H

#include "qcustomplotwrapper.h"
#include "core/dataprocessing/SpectraDataProcessor.h"

class QtSpectraPlot : public QCustomPlotWrapper
{
    Q_OBJECT
public:
    explicit QtSpectraPlot(SpectraDataProcessor*data, QWidget *parent = nullptr);
};

// Stacked spectra plot
class QtSpectraPlotStacked : public QtSpectraPlot
{
    Q_OBJECT
public:
    explicit QtSpectraPlotStacked(SpectraDataProcessor *data, QWidget *parent = nullptr);
};

// Spectra plot on the same plot
class QtSpectraPlotSamePlot : public QtSpectraPlot
{
    Q_OBJECT
public:
    explicit QtSpectraPlotSamePlot(SpectraDataProcessor *data, QWidget *parent = nullptr);
};

// Rotated text - needed to make yAxis label for SpectraPlotStacked
class RotatedTextElement : public QCPTextElement
{
public:
    RotatedTextElement(QCustomPlot *parentPlot, const QString &text, int angle = 0)
        : QCPTextElement(parentPlot, text)
        , angle(angle)
    {}

    // Override the draw method to handle rotation
    void draw(QCPPainter *painter) override
    {
        // save old transform
        QTransform oldTransform = painter->transform();

        // initialize
        painter->translate(-painter->transform().dx(), -painter->transform().dy());

        // set font, pen
        painter->setFont(mainFont());
        painter->setPen(QPen(mainTextColor()));

        // transform before drawing text
        if (angle == 0) {
            painter->drawText(mRect, mTextFlags, mText, nullptr);
        } else if (angle == -90) {
            painter->translate(mRect.x(), mRect.y() + mRect.height());
            painter->rotate(angle);
            painter->drawText(QRect(0, 0, mRect.height(), mRect.width()),
                              mTextFlags,
                              mText,
                              nullptr);
        }

        // restore old transform
        painter->setTransform(oldTransform);
    }

private:
    int angle; // Rotation angle in degrees
};

#endif // QTSPECTRAPLOT_H
