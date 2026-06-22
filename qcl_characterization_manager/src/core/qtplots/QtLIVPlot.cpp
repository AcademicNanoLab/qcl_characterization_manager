#include "QtLIVPlot.h"
#include <QtConcurrent>
#include <cmath>

QtLIVPlot::QtLIVPlot(LIVDataProcessor* data, const QString& outputDirectory, const double& w, const double& l, QWidget* parent)
    : QCustomPlotWrapper{ data, parent }, m_w(w), m_l(l)
{
    const double areaScaleFactor = 100000 / (w * l);  // cm² to m² conversion

    int listSize = data->getXList().size();
    for (int i = 0; i < listSize; ++i) {
        const QString& value = data->getValueList()[i];
        QVector<double>& x = data->getXList()[i];
        QVector<double>& y1 = data->getY1List()[i];
        QVector<double>& y2 = data->getY2List()[i];

        // Ensure valueToColor function is available or provide default color logic
        QColor lineColor = valueToColor(value, data->traceVariable);

        addGraph(xAxis, yAxis);        // V-I curve
        graph()->setData(x, y1);
        graph()->setPen(QPen(lineColor, 4));
        graph()->setName(value + data->unit);

        addGraph(xAxis, yAxis2);       // Normalized light output
        graph()->setData(x, y2);
        graph()->setPen(QPen(lineColor, 4));
        graph()->removeFromLegend();
    }

    // Axis labels and fonts
    QFont symbolFont("Arial", 14, QFont::Bold);
    QFont unitFont("Arial", 14, QFont::Bold);

    xAxis->setLabel("𝐼 [A]");
    xAxis2->setLabel("𝐽 [A·cm⁻²]");
    yAxis->setLabel("𝑉 [V]");
    yAxis2->setLabel("𝐿 [a.u.]");

    xAxis->setLabelFont(symbolFont);    xAxis2->setLabelFont(symbolFont);
    yAxis->setLabelFont(symbolFont);    yAxis2->setLabelFont(symbolFont);

    xAxis->setTickLabelFont(unitFont);  xAxis2->setTickLabelFont(unitFont);
    yAxis->setTickLabelFont(unitFont);  yAxis2->setTickLabelFont(unitFont);

    // Axis styling
    QPen axisPen(Qt::black, 2);
    QPen tickPen(Qt::black, 2);
    QPen subTickPen(Qt::black, 1.5);

    for (auto axis : { xAxis, xAxis2, yAxis, yAxis2 }) {
        axis->setBasePen(axisPen);
        axis->setTickPen(tickPen);
        axis->setSubTickPen(subTickPen);
    }

    // Axis ranges
    double xMin = data->getMinX();
    double xMax = data->getMaxX();
    double y1Min = data->getMinY1();
    double y1Max = data->getMaxY1();
    double normMax = data->getScaleFactor(); // Should be 100 by default

    xAxis->setRange(xMin, xMax);
    xAxis2->setRange(xMin * areaScaleFactor, xMax * areaScaleFactor);
    yAxis->setRange(y1Min, y1Max);
    yAxis2->setRange(0, normMax);

    // Set tick step manually using a fixed ticker
    setTickStep({ xAxis, xAxis2, yAxis, yAxis2 }, 10);

    // Optionally you can directly set the tickers:
    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickStep(10);
    xAxis->setTicker(fixedTicker);
    xAxis2->setTicker(fixedTicker);
    yAxis->setTicker(fixedTicker);
    yAxis2->setTicker(fixedTicker);
}

void QtLIVPlot::updateSecondaryXAxis()
{
    double scaleFactor = 100000 / (m_w * m_l); // Use class members instead
    xAxis2->setRange(xAxis->range().lower * scaleFactor, xAxis->range().upper * scaleFactor);
}
