#include "qcustomplotwrapper.h"

QCustomPlotDataWrapper::QCustomPlotDataWrapper(const QString &fileName,
                                               const QVariantMap &files,
                                               const QString &variable,
                                               bool threeColFileFlg,
                                               QWidget *parent)
    : QObject{parent}
    , fileName(fileName)
    , variable(variable)
    , whiteSpaceRx(new QRegularExpression("\\s+"))
{
    // suffix of legend label
    if (variable == "temperature")
        suffix = " K";
    else
        suffix = " mA";

    // make list of vectors
    generateVectors(files, threeColFileFlg);
}

//
//
//
//
//
// Generating vectors
void QCustomPlotDataWrapper::generateVectors(const QVariantMap &files, bool threeColFileFlg)
{
    minX = std::numeric_limits<unsigned int>::max(), minY1 = minX, minY2 = minX;
    maxX = std::numeric_limits<unsigned int>::min(), maxY1 = maxX, maxY2 = maxX;

    if (threeColFileFlg) {
        for (auto [name, value] : files.asKeyValueRange()) {
            QVector<double> x, y1, y2;

            generateVectors(name, &x, &y1, &y2);

            valueList.append(value.toString());
            xList.append(x);
            y1List.append(y1);
            y2List.append(y2);
        }
    } else {
        // need to iterate in order of value
        QVector<QPair<QString, QVariant>> sortedFiles;

        // collect key-value pairs
        for (auto [name, value] : files.asKeyValueRange()) {
            sortedFiles.append(qMakePair(name, value));
        }

        // sort list using value
        std::sort(sortedFiles.begin(),
                  sortedFiles.end(),
                  [](const QPair<QString, QVariant> &a, const QPair<QString, QVariant> &b) {
                      return a.second.toDouble() < b.second.toDouble();
                  });

        for (const auto &pair : sortedFiles) {
            QVector<double> x, y1;

            generateVectors(pair.first, &x, &y1, nullptr);

            valueList.append(pair.second.toString());
            xList.append(x);
            y1List.append(y1);
        }
    }
}

void QCustomPlotDataWrapper::generateVectors(const QString &fileName,
                                             QVector<double> *x,
                                             QVector<double> *y1,
                                             QVector<double> *y2)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "QCustomPlotWrapper::Please use a file that allows read.";
        return;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll(); // Read entire content at once
    QStringList lines = fileContent.split('\n', Qt::SkipEmptyParts);
    file.close();

    x->reserve(lines.size());
    y1->reserve(lines.size());
    if (y2 != nullptr)
        y2->reserve(lines.size());

    for (const QString &line : lines) {
        QStringList fields = line.split(*whiteSpaceRx, Qt::SkipEmptyParts);

        if (fields.length() != 3 && y2 != nullptr) {
            qDebug() << "QCustomPlotWrapper::Your file should have 3 columns.";
            return;
        } else if (fields.length() != 2 && y2 == nullptr) {
            qDebug() << "QCustomPlotWrapper::Your file should have 2 columns.";
            return;
        }

        double xVal = fields[0].toDouble(), y1Val = fields[1].toDouble();
        if (xVal <= 0.005) // filter out noise
            continue;

        // append
        x->append(xVal);
        y1->append(y1Val);

        // minimums
        if (xVal < minX)
            minX = xVal;
        if (y1Val < minY1)
            minY1 = y1Val;

        // maximums
        if (xVal > maxX)
            maxX = xVal;
        if (y1Val > maxY1)
            maxY1 = y1Val;

        if (y2 != nullptr) {
            double y2Val = fields[2].toDouble();
            y2->append(y2Val);
            if (y2Val < minY2)
                minY2 = y2Val;
            if (y2Val > maxY2)
                maxY2 = y2Val;
        }
    }
}

/**************************************** QCustomPlotWrapper *****************************************/
QCustomPlotWrapper::QCustomPlotWrapper(QCustomPlotDataWrapper *data, QWidget *parent)
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
    else // (400mA to 1300mA)
        return toRainbowColor(valueString, 400, 1300);
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
