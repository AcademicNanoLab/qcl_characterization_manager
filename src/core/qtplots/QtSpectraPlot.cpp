#include "QtSpectraPlot.h"
#include <QtConcurrent>
#include <QFont>
#include <QPen>
#include <cmath>

/**************************************** SpectraPlot *****************************************/
QtSpectraPlot::QtSpectraPlot(SpectraDataProcessor *data, QWidget *parent)
    : QCustomPlotWrapper{data, parent}
{
    QFont labelFont("Arial", 14, QFont::Bold);
    QFont tickLabelFont("Arial", 12, QFont::Bold);

    xAxis->setLabelFont(labelFont);
    yAxis->setLabelFont(labelFont);
    xAxis->setTickLabelFont(tickLabelFont);
    yAxis->setTickLabelFont(tickLabelFont);

    xAxis->setLabel("𝜈 [THz]");
    yAxis->setLabel("[a.u.]");
}

/**************************************** SpectraPlotStacked *****************************************/
QtSpectraPlotStacked::QtSpectraPlotStacked(SpectraDataProcessor *data, QWidget *parent)
    : QtSpectraPlot{data, parent}
{
    const auto &xList = data->getXList();
    const auto &y1List = data->getY1List();
    const auto &valueList = data->getValueList();

    // Compute global min/max
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY1 = std::numeric_limits<double>::max();
    double maxY1 = std::numeric_limits<double>::lowest();

    for (int i = 0; i < xList.length(); ++i) {
        for (double x : xList[i]) {
            double scaledX = x / 33.356;
            minX = std::min(minX, scaledX);
            maxX = std::max(maxX, scaledX);
        }
        for (double y : y1List[i]) {
            minY1 = std::min(minY1, y);
            maxY1 = std::max(maxY1, y);
        }
    }

    QMargins _margins = axisRect()->insetLayout()->margins() / 2;
    setAutoAddPlottableToLegend(false);
    plotLayout()->setRowSpacing(0);
    plotLayout()->setColumnSpacing(0);
    plotLayout()->setAutoMargins(QCP::MarginSide::msRight | QCP::MarginSide::msTop);
    plotLayout()->setMargins(QMargins(70, 20, 20, 50));
    plotLayout()->clear();

    QFont tickLabelFont("Arial", 12, QFont::Bold);

    for (int i = 0; i < xList.length(); i++) {
        int invertedIdx = xList.length() - 1 - i;
        const QString &value = valueList[invertedIdx];
        QVector<double> x = xList[invertedIdx];
        QVector<double> y1 = y1List[invertedIdx];

        for (double &val : x) {
            val /= 33.356;
        }

        QColor lineColor = valueToColor(value, data->traceVariable);

        QCPAxisRect *_axisRect = new QCPAxisRect(this);
        _axisRect->setupFullAxesBox(true);
        _axisRect->insetLayout()->setSizeConstraintRect(QCPColorScaleAxisRectPrivate::SizeConstraintRect::scrOuterRect);

        QCPGraph *_graph = addGraph(_axisRect->axis(QCPAxis::atBottom), _axisRect->axis(QCPAxis::atLeft));
        _graph->setData(x, y1);
        _graph->setPen(QPen(lineColor, 4));
        _graph->setName(value + data->unit);

        setRange({_axisRect->axis(QCPAxis::atBottom), _axisRect->axis(QCPAxis::atTop)}, minX, maxX, 0);
        setRange({_axisRect->axis(QCPAxis::atLeft), _axisRect->axis(QCPAxis::atRight)}, minY1, maxY1);

        plotLayout()->addElement(i, 0, _axisRect);
        plotLayout()->setRowStretchFactor(i, 1);

        QCPLegend *_legend = new QCPLegend();
        _axisRect->insetLayout()->addElement(_legend, Qt::AlignTop | Qt::AlignLeft);
        _axisRect->insetLayout()->setMargins(_margins);
        _legend->setLayer("legend");
        _graph->addToLegend(_legend);

        _axisRect->setAutoMargins(QCP::MarginSide::msNone);
        _axisRect->setMargins(QMargins(0, 0, 0, 0));

        if (i != xList.length() - 1)
            _axisRect->axis(QCPAxis::atBottom)->setTickLabels(false);
    }

    xAxis->setTickLabelFont(tickLabelFont);
    yAxis->setTickLabelFont(tickLabelFont);

    QCPItemText *yLabel = new QCPItemText(this);
    yLabel->setPositionAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    yLabel->position->setType(QCPItemPosition::ptViewportRatio);
    yLabel->position->setCoords(0.03, 0.47);
    yLabel->setRotation(-90);
    yLabel->setFont(QFont("Arial", 14, QFont::Bold));
    yLabel->setText("[a.u.]");

    QCPItemText *xLabel = new QCPItemText(this);
    xLabel->setPositionAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    xLabel->position->setType(QCPItemPosition::ptViewportRatio);
    xLabel->position->setCoords(0.53, 0.97);
    xLabel->setFont(QFont("Arial", 14, QFont::Bold));
    xLabel->setText("𝜈 [THz]");

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setObjectName(objectName() + " - stacked");
}

/**************************************** SpectraPlotSamePlot *****************************************/
QtSpectraPlotSamePlot::QtSpectraPlotSamePlot(SpectraDataProcessor *data, QWidget *parent)
    : QtSpectraPlot{data, parent}
{
    const auto &xList = data->getXList();
    const auto &y1List = data->getY1List();
    const auto &valueList = data->getValueList();

    double mulFactor = 1.1;
    double len = xList.length() * mulFactor + 0.2;

    double peakFrequency = 0.0;
    double maxIntensity = -std::numeric_limits<double>::max();

    for (int i = 0; i < xList.length(); i++) {
        const QString &value = valueList[i];
        QVector<double> x = xList[i];
        QVector<double> y1 = y1List[i];

        for (double &val : x) {
            val /= 33.356;
        }

        for (int j = 0; j < x.size(); ++j) {
            if (y1[j] > maxIntensity) {
                maxIntensity = y1[j];
                peakFrequency = x[j];
            }
        }

        double minY1 = *std::min_element(y1.begin(), y1.end());
        double range = *std::max_element(y1.begin(), y1.end()) - minY1;

        QtConcurrent::blockingMap(y1, [minY1, range, i, mulFactor](double &val) {
            val = ((val - minY1) / range) + i * mulFactor;
        });

        QColor lineColor = valueToColor(value, data->traceVariable);

        addGraph(xAxis, yAxis);
        graph()->setData(x, y1);
        graph()->setPen(QPen(lineColor, 4));

        QCPItemText *textLabel = new QCPItemText(this);
        textLabel->setPositionAlignment(Qt::AlignBottom | Qt::AlignLeft);
        textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
        textLabel->position->setCoords(0.01, 1 - (i * mulFactor / len) - 0.075);
        textLabel->setFont(QFont("Arial", 12, QFont::Bold));
        textLabel->setText(value + data->unit);
    }

    double newMinX = std::max(peakFrequency - 0.7, *std::min_element(xList[0].begin(), xList[0].end()) / 33.356);
    double newMaxX = std::min(peakFrequency + 0.7, *std::max_element(xList[0].begin(), xList[0].end()) / 33.356);
    xAxis->setRange(newMinX, newMaxX);

    QFont tickLabelFont("Arial", 12, QFont::Bold);
    xAxis->setTickLabelFont(tickLabelFont);
    yAxis->setTickLabelFont(tickLabelFont);

    setObjectName(objectName() + " - same plot");
}
