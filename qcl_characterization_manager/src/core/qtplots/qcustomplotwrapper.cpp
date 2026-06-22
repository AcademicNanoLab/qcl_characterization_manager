#include "qcustomplotwrapper.h"

QCustomPlotWrapper::QCustomPlotWrapper(IDataProcessor *data, QWidget *parent)
    : QCustomPlot{parent}
    , data(data)
{
    // turn on OpenGL hardware acceleration // scaling issue, TODO
    // setOpenGl(true);

    // turn on interactions
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // font
    setFont(QFont("Helvetica", 15));

    // set object name - for saving to PNG
    setObjectName(data->fileName);
}

// Step step value to tickStep
double QCustomPlotWrapper::setTickStep(QList<QCPAxis *> axisList, double tickStep)
{
    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickStep(tickStep);
    fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone); // can't scale tick step

    for (QCPAxis *axis : axisList)
        axis->setTicker(fixedTicker);

    return tickStep;
}

// Using minimum and maximum value, sets an appropriate step value for the axis
double QCustomPlotWrapper::setTickStep(QList<QCPAxis *> axisList, double &lower, double &upper)
{
    double step;
    double diff = upper - lower;
    int maxTicks = 9;
    double rawStep = diff / maxTicks;

    // Determine the order of magnitude of the step
    double magnitude = pow(10, floor(log10(rawStep)));

    // Round the step
    if (rawStep / magnitude >= 7)
        step = 10 * magnitude;
    else if (rawStep / magnitude >= 5)
        step = 5 * magnitude;
    else if (rawStep / magnitude >= 2)
        step = 2 * magnitude;
    else
        step = magnitude;

    return setTickStep(axisList, step);
}

// Using minimum and maximum value, sets an appropriate range for axis
void QCustomPlotWrapper::setRange(QList<QCPAxis *> axisList,
                                  double lower,
                                  double upper,
                                  double adjustVal)
{
    if (adjustVal == -1)
        adjustVal = (upper - lower) * 0.1;
    double adjustedLower = lower - adjustVal;
    double adjustedUpper = upper + adjustVal;

    for (QCPAxis *axis : axisList)
        axis->setRange(adjustedLower, adjustedUpper);
}

// Map a value to a color
QColor QCustomPlotWrapper::valueToColor(const QString &valueString, const QString &variable)
{
    if (variable == "temperature") // (10K to 150K)
        return toRainbowColor(valueString, 10, 150);
    else // (80mA to 13000mA)
        return toRainbowColor(valueString, 80, 13000);
}

// Map values to a rainbow (purple->red)
QColor QCustomPlotWrapper::toRainbowColor(const QString &valueString, double lower, double upper)
{
    // Normalize value to the range 0.0 to 1.0
    double normalizedValue = (valueString.toDouble() - lower) / (upper - lower);

    // Interpolate between purple ~ red
    double hue = 270.0 * (1.0 - normalizedValue);

    // Convert HSV to QColor
    return QColor::fromHsvF(hue / 360.0, 1, 1);
}
