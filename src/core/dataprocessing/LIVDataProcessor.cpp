/**
 * \file        LIVDataProcessor.cpp
 * \brief       Parses and processes LIV measurement data from files.
 *              Extracts current and optical/electrical output vectors,
 *              normalizes the data, and prepares it for further analysis.
 * \author      Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "LIVDataProcessor.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <limits>
#include <algorithm>

/**
 * \brief Constructs a LIVDataProcessor and immediately processes the input files.
 *
 * Initializes internal min/max tracking, parses all input files, stores sorted data vectors,
 * and normalizes the optical output traces.
 *
 * \param fileName Name of the associated dataset (for reference).
 * \param files Map of file paths to trace variable values (e.g., temperature).
 * \param traceVariable The variable represented by the trace (e.g., "Temperature").
 * \param scaleFactor Factor applied during normalization of y2 (e.g., 1.0 or 100.0).
 */
LIVDataProcessor::LIVDataProcessor(const QString &fileName,
                                   const QVariantMap &files,
                                   const QString &traceVariable,
                                   double scaleFactor)
    : IDataProcessor(fileName, "mA", traceVariable),
      minX(std::numeric_limits<double>::max()),
      maxX(std::numeric_limits<double>::lowest()),
      minY1(std::numeric_limits<double>::max()),
      maxY1(std::numeric_limits<double>::lowest()),
      preNormMaxY2(std::numeric_limits<double>::lowest()),
      scaleFactor(scaleFactor)
{
    generateVectors(files);
    normalizeData();
}

/**
 * \brief Parses and stores data from multiple LIV files.
 *
 * Reads and sorts all provided files based on the numeric value of their trace variable
 * (e.g., temperature). Internally updates min/max values for x, y1, and y2.
 *
 * \param files Map where keys are file paths and values are trace variable values.
 */
void LIVDataProcessor::generateVectors(const QVariantMap &files)
{
    // Temporary containers
    struct Trace {
        QString filePath;
        QString valueStr;
        QVector<double> x, y1, y2;
    };
    QList<Trace> traces;

    // Read all files
    for (auto [name, value] : files.toStdMap()) {
        Trace t;
        t.filePath = name;
        t.valueStr = value.toString();
        generateVectorsFromFile(t.filePath, &t.x, &t.y1, &t.y2);
        traces.append(std::move(t));
    }

    // Sort by valueStr as double
    std::sort(traces.begin(), traces.end(), [](const Trace &a, const Trace &b) {
        return a.valueStr.toDouble() < b.valueStr.toDouble();
    });

    // Now fill sorted internal lists
    for (const Trace &t : traces) {
        xList.append(t.x);
        y1List.append(t.y1);
        y2List.append(t.y2);
        valueList.append(t.valueStr);

        if (!t.x.isEmpty()) {
            auto [minXIt, maxXIt] = std::minmax_element(t.x.begin(), t.x.end());
            minX = std::min(minX, *minXIt);
            maxX = std::max(maxX, *maxXIt);
        }
        if (!t.y1.isEmpty()) {
            auto [minY1It, maxY1It] = std::minmax_element(t.y1.begin(), t.y1.end());
            minY1 = std::min(minY1, *minY1It);
            maxY1 = std::max(maxY1, *maxY1It);
        }
        if (!t.y2.isEmpty()) {
            double maxY2 = *std::max_element(t.y2.begin(), t.y2.end());
            preNormMaxY2 = std::max(preNormMaxY2, maxY2);
        }
    }
}

/**
 * \brief Parses a single LIV data file into x, y1, and y2 vectors.
 *
 * Each line of the file is expected to have three whitespace-separated columns:
 * current (x), electrical output (y1), and optical output (y2).
 * Points with x ≤ 0.005 are ignored. Data is sorted by current before being stored.
 *
 * \param fileName Path to the LIV file.
 * \param x Pointer to the vector that will store current values.
 * \param y1 Pointer to the vector that will store electrical output values.
 * \param y2 Pointer to the vector that will store optical output values.
 */
void LIVDataProcessor::generateVectorsFromFile(const QString &fileName,
                                               QVector<double> *x,
                                               QVector<double> *y1,
                                               QVector<double> *y2)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << fileName;
        return;
    }

    QTextStream in(&file);
    QStringList lines = in.readAll().split('\n', Qt::SkipEmptyParts);
    file.close();

    // Temporary container to hold points before sorting
    QVector<std::tuple<double, double, double>> points;
    points.reserve(lines.size());

    for (const QString &line : lines) {
        QStringList fields = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (fields.size() != 3)
            continue;

        double xVal = fields[0].toDouble();
        double y1Val = fields[1].toDouble();
        double y2Val = fields[2].toDouble();

        if (xVal <= 0.005) continue;

        points.emplace_back(xVal, y1Val, y2Val);
    }

    // Sort by xVal ascending
    std::sort(points.begin(), points.end(), [](const auto &a, const auto &b) {
        return std::get<0>(a) < std::get<0>(b);
    });

    x->reserve(points.size());
    y1->reserve(points.size());
    y2->reserve(points.size());

    for (const auto &[xVal, y1Val, y2Val] : points) {
        x->append(xVal);
        y1->append(y1Val);
        y2->append(y2Val);
    }
}


/**
 * \brief Normalizes all y2 (optical output) vectors using the highest measured power
 *
 * Scales each point in all y2 vectors so that the last value in the first trace maps to
 * `scaleFactor`. Prevents division by zero by falling back to 1.0 if the reference value is 0.
 */
void LIVDataProcessor::normalizeData()
{
    if (y2List.isEmpty()) return;

    // Use global max y2 as normalization reference
    double referenceNorm = preNormMaxY2;
    if (referenceNorm == 0.0) referenceNorm = 1.0;

    for (QVector<double>& y2 : y2List) {
        for (double &val : y2) {
            val = (val / referenceNorm) * scaleFactor;
        }
    }
}