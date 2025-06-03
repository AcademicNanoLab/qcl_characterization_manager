/**
 * \file        SpectraDataProcessor.cpp
 * \brief       Parses and processes spectral measurement data from multiple files.
 *              Extracts frequency and amplitude vectors, sorts and normalizes data,
 *              and calculates spectral peaks and quality factors.
 * \author      Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "SpectraDataProcessor.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <algorithm>  
#include <cmath>      

/**
 * \brief Finds the index of the value in a vector closest to a target value.
 * \param vec Vector of double values to search.
 * \param target Target value to find the closest element to.
 * \return Index of the closest value in the vector, or -1 if the vector is empty.
 */
static int findClosestIndex(const QVector<double> &vec, double target) {
    if (vec.isEmpty()) return -1;

    int closestIndex = 0;
    double minDiff = std::abs(vec[0] - target);
    for (int i = 1; i < vec.size(); ++i) {
        double diff = std::abs(vec[i] - target);
        if (diff < minDiff) {
            minDiff = diff;
            closestIndex = i;
        }
    }
    return closestIndex;
}

/**
 * \brief Initializes the SpectraDataProcessor object.
 * 
 * Processes input files to generate frequency and amplitude vectors,
 * ensures the frequency vectors are ascending, normalizes the data,
 * and calculates peak parameters for each trace.
 * 
 * \param fileName Base filename or identifier used in processing.
 * \param files Map of filenames to trace variable values.
 * \param traceVariable Name of the trace variable (e.g., current unit).
 * \param fmin Minimum frequency bound for processing data.
 * \param fmax Maximum frequency bound for processing data.
 */
SpectraDataProcessor::SpectraDataProcessor(const QString &fileName,
                                           const QVariantMap &files,
                                           const QString &traceVariable,
                                           double fmin,
                                           double fmax)
    : IDataProcessor(fileName, "mA", traceVariable), fmin(fmin), fmax(fmax)
{
    generateVectors(files);
    ensureAscendingX();
    normalizeData();

    // Print Grace-compatible legend strings for each trace
    for (int i = 0; i < xList.size(); ++i) {
        qDebug().noquote() << QString("Legend %1: %2")
            .arg(i)
            .arg(generateLegendForTrace(i));
    }
}

/**
 * \brief Generates frequency (x) and amplitude (y1) vectors from the provided files.
 * 
 * Reads data from each file, stores traces, sorts them by their associated values,
 * and calculates center and side spectral peaks for each trace.
 * 
 * \param files Map of filenames to trace variable values.
 */
void SpectraDataProcessor::generateVectors(const QVariantMap &files)
{
    struct Trace {
        QString valueStr;
        QVector<double> x;
        QVector<double> y1;
    };

    QList<Trace> traces;

    // Step 1: Read and store traces
    for (auto [name, value] : files.toStdMap()) {
        QVector<double> x, y1;
        generateVectorsFromFile(name, &x, &y1);

        Trace t;
        t.valueStr = value.toString();
        t.x = std::move(x);
        t.y1 = std::move(y1);

        traces.append(std::move(t));
    }

    // Step 2: Sort by valueStr as numeric
    std::sort(traces.begin(), traces.end(), [](const Trace &a, const Trace &b) {
        return a.valueStr.toDouble() < b.valueStr.toDouble();
    });

    // Step 3: Populate main data structures
    for (const Trace &t : traces) {
        valueList.append(t.valueStr);
        xList.append(t.x);
        y1List.append(t.y1);
    }

    // Step 4: Calculate peaks
    for (int i = 0; i < xList.size(); ++i) 
    {
        const auto& x = xList[i];
        const auto& y = y1List[i];

        // Calculate center peak
        double centerFreq = findCenterMode(x, y);
        double fwhm = calculateFWHM(x, y, centerFreq);
        double qFactor = calculateQFactor(centerFreq, fwhm);
        double amplitude = *std::max_element(y.begin(), y.end());

        centerModeData.append(Peak{centerFreq, amplitude, fwhm, qFactor});

        // Calculate side peaks and remove center
        QVector<Peak> sidePeaks = findSideModes(x, y, 0.15);

        sidePeaks.erase(std::remove_if(sidePeaks.begin(), sidePeaks.end(),
            [centerFreq](const Peak& p) {
                return std::abs(p.frequency - centerFreq) < 1e-6; // Remove f0 within 1 MHz
            }),
            sidePeaks.end());

        sideModeData.append(std::move(sidePeaks));
    }
}

/**
 * \brief Ensures that all frequency vectors (xList) are sorted in ascending order.
 * 
 * If any frequency vector is found to be descending, the method reverses both
 * the frequency vector and the corresponding amplitude vector to maintain alignment.
 */
void SpectraDataProcessor::ensureAscendingX()
{
    for (int traceIdx = 0; traceIdx < xList.size(); ++traceIdx) {
        QVector<double>& x = xList[traceIdx];
        QVector<double>& y = y1List[traceIdx];

        if (x.size() < 2)
            continue;

        if (x.first() > x.last()) {
            qDebug() << "Reversing trace" << traceIdx << "because x is descending.";

            // Reverse x and y vectors to keep them aligned
            std::reverse(x.begin(), x.end());
            std::reverse(y.begin(), y.end());
        }
    }
}

/**
 * \brief Reads spectral data from a file and populates frequency and amplitude vectors.
 * 
 * Parses a text file where each line contains two columns: frequency and amplitude.
 * Converts frequency units to THz and filters out frequencies below 0.005 THz.
 * 
 * \param fileName Path to the input data file.
 * \param x Pointer to a QVector<double> that will store frequency values (in THz).
 * \param y1 Pointer to a QVector<double> that will store amplitude values.
 */
void SpectraDataProcessor::generateVectorsFromFile(const QString &fileName, QVector<double> *x, QVector<double> *y1)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading:" << fileName;
        return;
    }

    QTextStream in(&file);
    QStringList lines = in.readAll().split('\n', Qt::SkipEmptyParts);
    file.close();

    x->reserve(lines.size());
    y1->reserve(lines.size());

    for (const QString &line : lines) {
        QStringList fields = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (fields.length() != 2) {
            continue;
        }

        double xVal = fields[0].toDouble();
        xVal *= 0.0299792458; // convert to THz

        double y1Val = fields[1].toDouble();

        if (xVal <= 0.005) continue;

        x->append(xVal);
        y1->append(y1Val);
    }
}

/**
 * \brief Normalizes amplitude vectors so that the maximum value in each vector is 1.
 * 
 * Iterates over all amplitude vectors (y1List) and divides all elements by the
 * maximum amplitude value in that vector to scale data between 0 and 1.
 */
void SpectraDataProcessor::normalizeData()
{
    for (QVector<double> &y1 : y1List) {
        if (!y1.isEmpty()) {
            double maxVal = *std::max_element(y1.begin(), y1.end());
            if (maxVal != 0.0) {
                for (double &val : y1) {
                    val /= maxVal;
                }
            }
        }
    }
}

/**
 * \brief Finds the frequency corresponding to the highest amplitude in the spectrum.
 * 
 * Searches the amplitude vector for the maximum value and returns the corresponding frequency.
 * 
 * \param x Vector of frequency values.
 * \param y1 Vector of amplitude values.
 * \return Frequency (in the same units as x) at the peak amplitude.
 */
double SpectraDataProcessor::findCenterMode(const QVector<double>& x, const QVector<double>& y1) const
{
    // Find the frequency with the highest amplitude (f0)
    auto maxIt = std::max_element(y1.begin(), y1.end());
    int maxIndex = std::distance(y1.begin(), maxIt);
    qDebug() << "Center frequency f0:" << x[maxIndex];

    return x[maxIndex];
}

/**
 * \brief Linearly interpolates to find the frequency where amplitude crosses the half-maximum value.
 * 
 * Given two points (x1, y1) and (x2, y2), computes the frequency at which the amplitude
 * equals the specified halfMax by linear interpolation.
 * 
 * \param x1 First frequency value.
 * \param y1 Amplitude at x1.
 * \param x2 Second frequency value.
 * \param y2 Amplitude at x2.
 * \param halfMax The target half-maximum amplitude value.
 * \return Interpolated frequency at half-maximum crossing.
 */
double SpectraDataProcessor::interpolateHalfMaxCrossing(
    double x1, double y1, double x2, double y2, double halfMax) const
{
    if (x1 == x2 || y1 == y2) return x1;
    double slope = (y2 - y1) / (x2 - x1);
    return x1 + (halfMax - y1) / slope;
}

/**
 * \brief Finds the frequency at which the amplitude crosses the half-maximum on the left side of the peak.
 * 
 * Searches leftwards from the peak index to find where the amplitude crosses the halfMax threshold.
 * Uses linear interpolation for an accurate crossing frequency.
 * 
 * \param x Vector of frequency values.
 * \param y1 Vector of amplitude values.
 * \param peakIndex Index of the peak in the vectors.
 * \param halfMax Half-maximum amplitude value.
 * \return Frequency of the left half-maximum crossing, or NaN if not found.
 */
double SpectraDataProcessor::findLeftHalfMaxCrossing(
    const QVector<double>& x, const QVector<double>& y1,
    int peakIndex, double halfMax) const
{
    for (int i = peakIndex; i > 0; --i) {
        if ((y1[i] >= halfMax && y1[i - 1] < halfMax) ||
            (y1[i] <= halfMax && y1[i - 1] > halfMax)) {
            return interpolateHalfMaxCrossing(x[i], y1[i], x[i - 1], y1[i - 1], halfMax);
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

/**
 * \brief Finds the frequency at which the amplitude crosses the half-maximum on the right side of the peak.
 * 
 * Searches rightwards from the peak index to find where the amplitude crosses the halfMax threshold.
 * Uses linear interpolation for an accurate crossing frequency.
 * 
 * \param x Vector of frequency values.
 * \param y1 Vector of amplitude values.
 * \param peakIndex Index of the peak in the vectors.
 * \param halfMax Half-maximum amplitude value.
 * \return Frequency of the right half-maximum crossing, or NaN if not found.
 */
double SpectraDataProcessor::findRightHalfMaxCrossing(
    const QVector<double>& x, const QVector<double>& y1,
    int peakIndex, double halfMax) const
{
    for (int i = peakIndex; i < x.size() - 1; ++i) {
        if ((y1[i] >= halfMax && y1[i + 1] < halfMax) ||
            (y1[i] <= halfMax && y1[i + 1] > halfMax)) {
            return interpolateHalfMaxCrossing(x[i], y1[i], x[i + 1], y1[i + 1], halfMax);
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

/**
 * \brief Calculates the Full Width at Half Maximum (FWHM) of the peak centered at peakFreq.
 * 
 * Finds the half-maximum crossing frequencies on both sides of the peak and calculates their difference.
 * Returns 0.0 if the peak frequency is not found or if the calculation is invalid.
 * 
 * \param x Vector of frequency values.
 * \param y1 Vector of amplitude values.
 * \param peakFreq Frequency at the peak center.
 * \return FWHM value, or 0.0 if invalid.
 */
double SpectraDataProcessor::calculateFWHM(const QVector<double>& x, const QVector<double>& y1, double peakFreq) const
{
    int peakIndex = findClosestIndex(x, peakFreq);
    if (peakIndex == -1) {
        qDebug() << "Peak frequency not found in x data!";
        return 0.0;
    }

    double peakAmplitude = y1[peakIndex];
    double halfMax = peakAmplitude / 2.0;

    double leftFreq = findLeftHalfMaxCrossing(x, y1, peakIndex, halfMax);
    double rightFreq = findRightHalfMaxCrossing(x, y1, peakIndex, halfMax);

    if (std::isnan(leftFreq) || std::isnan(rightFreq) || leftFreq == rightFreq) {
        qDebug() << "Error: FWHM calculation invalid. leftFreq:" << leftFreq
                 << " rightFreq:" << rightFreq << " peakFreq:" << peakFreq
                 << " peakIndex:" << peakIndex << " y1[peakIndex]:" << peakAmplitude;
        return 0.0;
    }

    double fwhm = std::abs(rightFreq - leftFreq);
    qDebug() << "FWHM:" << fwhm;
    return fwhm;
}

/**
 * \brief Calculates the quality factor (Q-factor) of a peak.
 * 
 * Q-factor is defined as the ratio of the center frequency (f0) to the full width at half maximum (FWHM).
 * Returns 0 if the FWHM is zero or negative.
 * 
 * \param f0 Center frequency of the peak.
 * \param fwhm Full width at half maximum of the peak.
 * \return Quality factor (Q-factor) of the peak.
 */
double SpectraDataProcessor::calculateQFactor(double f0, double fwhm) const
{
    if (fwhm <= 0.0) return 0.0;
    return f0 / fwhm;
}

/**
 * \brief Smooths a data vector using a simple moving average filter.
 * 
 * Applies a moving average with the specified window size to reduce noise in the data.
 * 
 * \param y Input vector of data points.
 * \param windowSize Size of the smoothing window (should be an odd number).
 * \return Smoothed vector of data points.
 */
QVector<double> SpectraDataProcessor::smooth(const QVector<double>& y, int windowSize) const
{
    QVector<double> smoothed(y.size());
    int halfWindow = windowSize / 2;

    for (int i = 0; i < y.size(); ++i) {
        int start = std::max(0, i - halfWindow);
		int end = std::min(static_cast<int>(y.size()) - 1, i + halfWindow);
        double sum = 0.0;
        int count = 0;

        for (int j = start; j <= end; ++j) {
            sum += y[j];
            ++count;
        }

        smoothed[i] = sum / count;
    }

    return smoothed;
}

/**
 * \brief Checks if a peak at a given index is prominent based on local minima.
 * 
 * A peak is considered prominent if its height exceeds the baseline (max of local minima on both sides)
 * by at least the specified minimum prominence.
 * 
 * \param y Vector of amplitude values.
 * \param index Index of the peak to check.
 * \param minProminence Minimum required prominence to qualify as prominent.
 * \return True if the peak is prominent, false otherwise.
 */
bool SpectraDataProcessor::isProminentPeak(const QVector<double>& y, int index, double minProminence) const
{
    if (index < 0 || index >= y.size())
        return false;

    double leftMin = y[index];
    double rightMin = y[index];

    int range = 10;
    int start = std::max(0, index - range);
    int end = std::min(static_cast<int>(y.size()) - 1, index + range);

    for (int i = start; i < index; ++i) {
        leftMin = std::min(leftMin, y[i]);
    }

    for (int i = index + 1; i <= end; ++i) {
        rightMin = std::min(rightMin, y[i]);
    }

    double base = std::max(leftMin, rightMin);
    return (y[index] - base) >= minProminence;
}

/**
 * \brief Finds side modes (secondary peaks) in the spectrum above a given threshold.
 * 
 * Smooths the data and identifies local maxima that exceed the threshold and prominence criteria.
 * Calculates their FWHM and Q-factor before appending them to the list of side modes.
 * 
 * \param x Vector of frequency values.
 * \param y1 Vector of amplitude values.
 * \param threshold Threshold as a fraction of the maximum amplitude for detecting peaks.
 * \return Vector of identified side mode peaks.
 */
QVector<Peak> SpectraDataProcessor::findSideModes(const QVector<double>& x, const QVector<double>& y1, double threshold) const
{
    QVector<Peak> sideModes;

    if (y1.isEmpty()) return sideModes;

    QVector<double> ySmooth = smooth(y1, 5);
    double maxAmp = *std::max_element(ySmooth.begin(), ySmooth.end());
    if (maxAmp == 0.0) return sideModes;

    double minPeakHeight = threshold * maxAmp;
    double minProminence = 0.05 * maxAmp;

    for (int i = 1; i < ySmooth.size() - 1; ++i) {
        if (ySmooth[i] > minPeakHeight &&
            ySmooth[i] > ySmooth[i - 1] &&
            ySmooth[i] > ySmooth[i + 1] &&
            isProminentPeak(ySmooth, i, minProminence))
        {
            double f0 = x[i];
            double amp = ySmooth[i];
            double fwhm = calculateFWHM(x, ySmooth, f0);
            if (fwhm > 0.0) {
                double q = calculateQFactor(f0, fwhm);
                sideModes.append(Peak{f0, amp, fwhm, q});
            }
        }
    }

    return sideModes;
}

/**
 * \brief Adjusts the frequency range and step size for plotting or analysis.
 * 
 * Calculates a step size based on the input range and number of ticks, then
 * aligns the range such that the target frequency corresponds to a tick mark.
 * 
 * \param xmin Initial minimum frequency value.
 * \param xmax Initial maximum frequency value.
 * \param targetFreq Frequency value to align within the adjusted range.
 * \param[out] adjustedXmin Output adjusted minimum frequency.
 * \param[out] adjustedXmax Output adjusted maximum frequency.
 * \param[out] step Output step size between ticks.
 * \param numTicks Number of ticks desired within the range.
 */
void SpectraDataProcessor::adjustRange(double xmin, double xmax, double targetFreq, 
                                       double& adjustedXmin, double& adjustedXmax, double& step, 
                                       int numTicks) const
{
    step = (xmax - xmin) / numTicks;
    double offsetTicks = std::round((targetFreq - xmin) / step);
    adjustedXmin = targetFreq - offsetTicks * step;
    adjustedXmax = adjustedXmin + numTicks * step;
    qDebug() << "Adjusted range:" << adjustedXmin << adjustedXmax << "step:" << step << "targetFreq:" << targetFreq;
}

/**
 * \brief Returns the adjusted minimum frequency value for the data range.
 * 
 * If a user-specified minimum frequency (fmin) is set, returns it directly.
 * Otherwise, calculates the minimum based on peak frequencies and FWHM, including a margin,
 * and aligns it to a tick grid relative to the lowest center mode frequency.
 * 
 * \return Adjusted minimum frequency value.
 */
double SpectraDataProcessor::getXmin() const
{
	if (fmin != 0.0)
		return fmin;
		
    if (centerModeData.isEmpty() || xList.isEmpty() || y1List.isEmpty())
        return 0.0;

    double xmin = centerModeData[0].frequency - centerModeData[0].fwhm;
    double xmax = centerModeData[0].frequency + centerModeData[0].fwhm;

    for (int i = 0; i < sideModeData.size(); ++i) {
        for (const Peak& p : sideModeData[i]) {
            if (p.amplitude > 0.15) {
                xmin = std::min(xmin, p.frequency - p.fwhm);
                xmax = std::max(xmax, p.frequency + p.fwhm);
            }
        }
    }

    xmin -= 0.020; // 20 GHz margin
    xmax += 0.020;

    double lowestF0 = centerModeData[0].frequency;
    for (const Peak& c : centerModeData) {
        if (c.frequency < lowestF0)
            lowestF0 = c.frequency;
    }

    double adjustedXmin, adjustedXmax, step;
    adjustRange(xmin, xmax, lowestF0, adjustedXmin, adjustedXmax, step, 6);
    return adjustedXmin;
}

/**
 * \brief Returns the adjusted maximum frequency value for the data range.
 * 
 * If a user-specified maximum frequency (fmax) is set, returns it directly.
 * Otherwise, calculates the maximum based on peak frequencies and FWHM, including a margin,
 * and aligns it to a tick grid relative to the lowest center mode frequency.
 * 
 * \return Adjusted maximum frequency value.
 */
double SpectraDataProcessor::getXmax() const
{
	if (fmax != 0.0)
		return fmax;
		
    if (centerModeData.isEmpty() || xList.isEmpty() || y1List.isEmpty())
        return 0.0;

    double xmin = centerModeData[0].frequency - centerModeData[0].fwhm;
    double xmax = centerModeData[0].frequency + centerModeData[0].fwhm;

    for (int i = 0; i < sideModeData.size(); ++i) {
        for (const Peak& p : sideModeData[i]) {
            if (p.amplitude > 0.15) {
                xmin = std::min(xmin, p.frequency - p.fwhm);
                xmax = std::max(xmax, p.frequency + p.fwhm);
            }
        }
    }

    xmin -= 0.020;
    xmax += 0.020;

    double lowestF0 = centerModeData[0].frequency;
    for (const Peak& c : centerModeData) {
        if (c.frequency < lowestF0)
            lowestF0 = c.frequency;
    }

    double adjustedXmin, adjustedXmax, step;
    adjustRange(xmin, xmax, lowestF0, adjustedXmin, adjustedXmax, step, 6);
    return adjustedXmax;
}

/**
 * \brief Generates a formatted legend string for a given trace index.
 * 
 * The legend includes the current value in mA, the center frequency (f0), and
 * the closest side mode frequencies (f±1) along with their free spectral ranges (FSR)
 * in GHz relative to the center frequency.
 * 
 * \param traceIndex Index of the trace for which to generate the legend.
 * \return A QString containing the formatted legend.
 *         Returns an empty string if the index is out of range.
 */
QString SpectraDataProcessor::generateLegendForTrace(int traceIndex) const
{
    if (traceIndex >= centerModeData.size() || 
        traceIndex >= sideModeData.size() || 
        traceIndex >= valueList.size())
        return QString();

    QString currentMA = valueList[traceIndex].rightJustified(6, ' ');
    QString legend = QString("%1 mA: ").arg(currentMA);

    const Peak& center = centerModeData[traceIndex];
    const QVector<Peak>& sides = sideModeData[traceIndex];

    legend += QString("f\\s0\\N = %1 THz").arg(center.frequency, 0, 'f', 3);

    // Separate left and right side modes relative to center frequency
    std::optional<Peak> leftSide;
    std::optional<Peak> rightSide;

    // Find closest left and right side modes if they exist
    for (const Peak& p : sides) {
        if (p.frequency < center.frequency) {
            if (!leftSide || std::abs(p.frequency - center.frequency) < std::abs(leftSide->frequency - center.frequency)) {
                leftSide = p;
            }
        } else if (p.frequency > center.frequency) {
            if (!rightSide || std::abs(p.frequency - center.frequency) < std::abs(rightSide->frequency - center.frequency)) {
                rightSide = p;
            }
        }
    }

    // Append right side mode
    if (rightSide) {
        double fsrRight = rightSide->frequency - center.frequency;
        legend += QString(", f\\s1\\N = %1 THz, FSR\\s1\\N = %2 GHz")
            .arg(rightSide->frequency, 0, 'f', 3)
            .arg(fsrRight * 1000.0, 0, 'f', 1);
    }

    // Append left side mode
    if (leftSide) {
        double fsrLeft = center.frequency - leftSide->frequency;
        legend += QString(", f\\s-1\\N = %1 THz, FSR\\s-1\\N = %2 GHz")
            .arg(leftSide->frequency, 0, 'f', 3)
            .arg(fsrLeft * 1000.0, 0, 'f', 1);
    }

    return legend;

	/* Old code that printed Q factors of each mode
    if (traceIndex >= centerModeData.size() || 
        traceIndex >= sideModeData.size() || 
        traceIndex >= valueList.size())
        return QString();

    // Pad value for clean alignment and append unit
    QString paddedValue = valueList[traceIndex].rightJustified(6, ' ');
    QString valuePrefix = QString("%1 mA: ").arg(paddedValue);

    const Peak& center = centerModeData[traceIndex];
    const QVector<Peak>& sides = sideModeData[traceIndex];

    QString legend = valuePrefix + QString("f\\s0\\N = %1 THz, Q\\s0\\N = %2, \\xD\\xn\\f{}\\s0\\N = %3 GHz")
        .arg(center.frequency, 0, 'f', 3)
        .arg(center.qFactor, 0, 'f', 1)
        .arg(center.fwhm * 1000.0, 0, 'f', 1);

    QVector<Peak> sorted = sides;
    std::sort(sorted.begin(), sorted.end(), [&center](const Peak& a, const Peak& b) {
        return std::abs(a.frequency - center.frequency) < std::abs(b.frequency - center.frequency);
    });

    if (!sorted.isEmpty()) {
        double fsr = std::abs(sorted[0].frequency - center.frequency) * 1000.0;
        legend += QString(", FSR = %1 GHz").arg(fsr, 0, 'f', 1);
    }

    int forward = 1, backward = 1;
    for (const Peak& p : sorted) {
        double delta = p.frequency - center.frequency;
        int labelIndex = (delta > 0) ? forward++ : -backward++;

        legend += QString(", f\\s%1\\N = %2 THz, Q\\s%1\\N = %3, \\xD\\xn\\f{}\\s%1\\N = %4 GHz")
            .arg(labelIndex)
            .arg(p.frequency, 0, 'f', 3)
            .arg(p.qFactor, 0, 'f', 1)
            .arg(p.fwhm * 1000.0, 0, 'f', 1);
    }

    return legend;
	*/
}

/**
 * @brief Returns a string summarizing the global frequency range of all peaks in THz.
 * 
 * This method scans all detected peaks in both center and side mode data,
 * finds the minimum and maximum frequencies, and returns them formatted as a string.
 * 
 * If no peaks are detected, it returns an empty string.
 * If the minimum and maximum frequencies are equal, returns a single frequency string.
 * Otherwise, returns the range formatted as: "minFreq THz - maxFreq THz"
 * with frequencies displayed to three decimal places.
 * 
 * @return QString representing the global frequency range with units, or an empty string if no peaks.
 */
QString SpectraDataProcessor::getGlobalFrequencyRangeString() const
{
    if (centerModeData.isEmpty() && sideModeData.isEmpty()) {
        return QString();
    }

    double globalMin = std::numeric_limits<double>::max();
    double globalMax = std::numeric_limits<double>::lowest();

    // Check center modes
    for (const Peak &p : centerModeData) {
        if (p.frequency < globalMin) globalMin = p.frequency;
        if (p.frequency > globalMax) globalMax = p.frequency;
    }

    // Check side modes
    for (const QVector<Peak> &peaks : sideModeData) {
        for (const Peak &p : peaks) {
            if (p.frequency < globalMin) globalMin = p.frequency;
            if (p.frequency > globalMax) globalMax = p.frequency;
        }
    }

    if (qFuzzyCompare(globalMin, globalMax)) {
        // Frequencies are effectively equal, return single value
        return QStringLiteral("%1 THz").arg(globalMin, 0, 'f', 3);
    } else {
        return QStringLiteral("%1 THz - %2 THz").arg(globalMin, 0, 'f', 3).arg(globalMax, 0, 'f', 3);
    }
}