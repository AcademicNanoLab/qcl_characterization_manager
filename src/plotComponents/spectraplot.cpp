#include "spectraplot.h"
#include <QtConcurrent>

/**************************************** SpectraPlot *****************************************/
SpectraPlot::SpectraPlot(QCustomPlotDataWrapper *data, QWidget *parent)
    : QCustomPlotWrapper{data, parent}
{}

/**************************************** SpectraPlotStacked *****************************************/
SpectraPlotStacked::SpectraPlotStacked(QCustomPlotDataWrapper *data, QWidget *parent)
    : SpectraPlot{data, parent}
{
    // save margins for subplot
    QMargins _margins = axisRect()->insetLayout()->margins() / 2;

    // toggle setting, otherwise always adds to the first legend
    setAutoAddPlottableToLegend(false);

    // plotLayout spacing issues
    plotLayout()->setRowSpacing(0);
    plotLayout()->setColumnSpacing(0);
    plotLayout()->setAutoMargins(QCP::MarginSide::msRight | QCP::MarginSide::msTop);
    plotLayout()->setMargins(QMargins(70, 20, 20, 50));

    // clear plotLayout
    plotLayout()->clear();

    for (int i = 0; i < data->xList.length(); i++) {
        int invertedIdx = data->xList.length() - 1 - i;
        QString *value = &data->valueList[invertedIdx];
        QVector<double> *x = &data->xList[invertedIdx], *y1 = &data->y1List[invertedIdx];

        // line color
        QColor lineColor = valueToColor(*value, data->variable);

        // create axis rectangle
        QCPAxisRect *_axisRect = new QCPAxisRect(this);
        _axisRect->setupFullAxesBox(true);
        _axisRect->insetLayout()->setSizeConstraintRect(
            QCPColorScaleAxisRectPrivate::SizeConstraintRect::scrOuterRect);

        // create graph
        QCPGraph *_graph = addGraph(_axisRect->axis(QCPAxis::atBottom),
                                    _axisRect->axis(QCPAxis::atLeft));
        _graph->setData(*x, *y1);
        _graph->setPen(QPen(lineColor, 2));
        _graph->setName(*value + data->suffix);

        // set ranges, so all the plots have the same range
        setRange({_axisRect->axis(QCPAxis::atBottom), _axisRect->axis(QCPAxis::atTop)},
                 data->minX,
                 data->maxX,
                 0);
        setRange({_axisRect->axis(QCPAxis::atLeft), _axisRect->axis(QCPAxis::atRight)},
                 data->minY1,
                 data->maxY1);

        // add to plot
        plotLayout()->addElement(i, 0, _axisRect);
        plotLayout()->setRowStretchFactor(i, 1);

        // add legend
        QCPLegend *_legend = new QCPLegend();
        _axisRect->insetLayout()->addElement(_legend, Qt::AlignTop | Qt::AlignLeft);
        _axisRect->insetLayout()->setMargins(_margins);
        _legend->setLayer("legend");
        _graph->addToLegend(_legend);

        // edit margins
        _axisRect->setAutoMargins(QCP::MarginSide::msNone);
        _axisRect->setMargins(QMargins(0, 0, 0, 0));

        // no xTickLabels for everything except last graph
        if (i != data->xList.length() - 1)
            _axisRect->axis(QCPAxis::atBottom)->setTickLabels(false);
    }

    // create yAxis label
    QCPItemText *yLabel = new QCPItemText(this);
    yLabel->setPositionAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    yLabel->position->setType(QCPItemPosition::ptViewportRatio);
    yLabel->position->setCoords(0.03, 0.47);
    yLabel->setRotation(-90);
    yLabel->setClipToAxisRect(false);
    yLabel->setFont(font());
    yLabel->setText("(A.U.)");

    // create xAxis label - ensures the stacked graphs are all same size
    QCPItemText *xLabel = new QCPItemText(this);
    xLabel->setPositionAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    xLabel->position->setType(QCPItemPosition::ptViewportRatio);
    xLabel->position->setCoords(0.53, 0.97);
    xLabel->setClipToAxisRect(false);
    xLabel->setFont(font());
    xLabel->setText("𝑣 (THz)");

    // change size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // set object name - for saving to PNG
    setObjectName(objectName() + " - stacked");
}

/**************************************** SpectraPlotSamePlot *****************************************/
SpectraPlotSamePlot::SpectraPlotSamePlot(QCustomPlotDataWrapper *data, QWidget *parent)
    : SpectraPlot{data, parent}
{
    double mulFactor = 1.1;
    double len = data->xList.length() * mulFactor + 0.2;
    for (int i = 0; i < data->xList.length(); i++) {
        QString *value = &data->valueList[i];
        QVector<double> *x = &data->xList[i], *y1 = &data->y1List[i];

        double minY1 = data->minY1;
        double range = data->maxY1 - minY1;
        QtConcurrent::blockingMap(*y1, [minY1, range, i, mulFactor](double &val) {
            val = ((val - minY1) / range) + i * mulFactor;
        });

        // line color
        QColor lineColor = valueToColor(*value, data->variable);

        // create graph
        addGraph(xAxis, yAxis);
        graph()->setData(*x, *y1);
        graph()->setPen(QPen(lineColor, 2));

        // create text label
        QCPItemText *textLabel = new QCPItemText(this);
        textLabel->setPositionAlignment(Qt::AlignBottom | Qt::AlignLeft);
        textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
        textLabel->position->setCoords(0.01, 1 - (i * mulFactor / len) - 0.075);
        textLabel->setText(*value + data->suffix);
    }

    // give the axes some labels:
    xAxis->setLabel("𝑣 (THz)");
    yAxis->setLabel("(A.U.)");

    // show xAxis2, yAxis2, hide yAxis ticks
    axisRect()->setupFullAxesBox(true);
    yAxis->setTickLabels(false);

    // fix minY2, maxY2
    data->minY1 = 0;
    data->maxY1 = mulFactor * (data->xList.length());

    // set axes ranges, so we see all data:
    setRange({xAxis, xAxis2}, data->minX, data->maxX, 0);
    setRange({yAxis, yAxis2}, data->minY1, data->maxY1, 0.1);

    // change size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // set object name - for saving to PNG
    setObjectName(objectName() + " - same plot");
}
