#include "livplot.h"
#include <QtConcurrent>

LIVPlot::LIVPlot(QCustomPlotDataWrapper *data, const double &w, const double &l, QWidget *parent)
    : QCustomPlotWrapper{data, parent}
{
    for (int i = 0; i < data->xList.length(); i++) {
        QString *value = &data->valueList[i];
        QVector<double> *x = &data->xList[i], *y1 = &data->y1List[i], *y2 = &data->y2List[i];

        double minY2 = data->minY2;
        double range = data->maxY2 - minY2;
        QtConcurrent::blockingMap(*y2, [minY2, range](double &val) { val = (val - minY2) / range; });

        // line color
        QColor lineColor = valueToColor(*value, data->variable);

        // create first graph
        addGraph(xAxis, yAxis);
        graph()->setData(*x, *y1);
        graph()->setPen(QPen(lineColor, 2));
        graph()->setName(*value + data->suffix);

        // create second graph
        addGraph(xAxis, yAxis2);
        graph()->setData(*x, *y2);
        graph()->setPen(QPen(lineColor, 2));
        graph()->removeFromLegend();
    }

    // give the axes some labels:
    xAxis->setLabel("𝐼 (A)");
    xAxis2->setLabel("𝐽 (A·cm⁻²)");
    yAxis->setLabel("𝑉 (V)");
    yAxis2->setLabel("Normalised detector signal (a.u.)");

    // show xAxis2, yAxis2
    xAxis2->setVisible(true);
    yAxis2->setVisible(true);

    // fix minY2, maxY2
    data->minY2 = 0;
    data->maxY2 = 1;

    // scale factor for I -> J
    double scaleFactor = 100000 / (w * l);

    // work with ticks
    double xAxisStep = setTickStep({xAxis}, data->minX, data->maxX);
    setTickStep({xAxis2}, xAxisStep * scaleFactor);
    setTickStep({yAxis}, data->minY1, data->maxY1);
    setTickStep({yAxis2}, data->minY2, data->maxY2);

    // set axes ranges, so we see all data:
    setRange({xAxis}, data->minX, data->maxX);
    setRange({xAxis2}, data->minX * scaleFactor, data->maxX * scaleFactor);
    setRange({yAxis}, data->minY1, data->maxY1);
    setRange({yAxis2}, data->minY2, data->maxY2);

    // add legend, adjust legend position
    legend->setVisible(true);
    axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignLeft);

    // change size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
