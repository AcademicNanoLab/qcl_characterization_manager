/**
 * @file SpectraDataProcessor.h
 * @brief Processes spectral data from files and extracts peak information.
 * 
 * Provides functionality for normalizing data, extracting peak properties like center mode,
 * FWHM, Q factor, and side modes from spectral traces.
 * 
 * @author Aleksandar Demic
 */

#ifndef SPECTRADATAPROCESSOR_H
	#define SPECTRADATAPROCESSOR_H

	#include "IDataProcessor.h"

	/**
	 * @struct Peak
	 * @brief Represents a spectral peak with frequency, amplitude, FWHM, and Q factor.
	 */
	struct Peak {
		double frequency;  ///< Peak frequency
		double amplitude;  ///< Peak amplitude
		double fwhm;       ///< Full width at half maximum
		double qFactor;    ///< Quality factor
	};

	/**
	 * @class SpectraDataProcessor
	 * @brief Implements spectral data processing and peak analysis.
	 */
	class SpectraDataProcessor : public IDataProcessor
	{
		public:
			explicit SpectraDataProcessor(const QString &fileName,
										  const QVariantMap &files,
										  const QString &traceVariable, 
										  double fmin = 0.0,
										  double fmax = 0.0); ///< Constructor

			void normalizeData() override; ///< Normalize spectral data

			QList<QVector<double>>& getXList() { return xList; } ///< Get X data vectors
			QList<QVector<double>>& getY1List() { return y1List; } ///< Get Y1 data vectors
			const QList<QString>& getValueList() const { return valueList; } ///< Get list of trace labels

			double getXmin() const; ///< Get minimum X value
			double getXmax() const; ///< Get maximum X value

			QVector<Peak> getCenterModeData() const { return centerModeData; } ///< Get center mode peaks
			QVector<QVector<Peak>> getSideModeData() const { return sideModeData; } ///< Get side mode peaks
			QString getGlobalFrequencyRangeString() const; ///< Finds the range of frequencies among all traces and return a string fmin - fmax
			QString generateLegendForTrace(int traceIndex) const; ///< Generate legend string for a trace

		private:
			double fmin = 0.0; ///< Minimum frequency filter
			double fmax = 0.0; ///< Maximum frequency filter

			void generateVectors(const QVariantMap &files); ///< Generate data vectors from input files
			double interpolateHalfMaxCrossing(double x1, double y1, double x2, double y2, double halfMax) const; ///< Interpolate half max crossing point
			double findRightHalfMaxCrossing(const QVector<double>& x, const QVector<double>& y1, int startIndex, double halfMax) const; ///< Find right half max crossing index
			double findLeftHalfMaxCrossing(const QVector<double>& x, const QVector<double>& y1, int startIndex, double halfMax) const; ///< Find left half max crossing index
			QVector<double> smooth(const QVector<double>& y, int windowSize) const; ///< Smooth data with moving average
			bool isProminentPeak(const QVector<double>& y, int index, double minProminence) const; ///< Check if peak is prominent
			void ensureAscendingX(); ///< Ensure X data is ascending
			void adjustRange(double xmin, double xmax, double targetFreq, double& adjustedXmin, double& adjustedXmax, double& step, int numTicks = 6) const; ///< Adjust X axis range

			void generateVectorsFromFile(const QString &fileName, QVector<double> *x, QVector<double> *y1); ///< Load vectors from file

			double findCenterMode(const QVector<double>& x, const QVector<double>& y1) const; ///< Find center mode frequency
			double calculateFWHM(const QVector<double>& x, const QVector<double>& y1, double peakFreq) const; ///< Calculate FWHM of peak
			double calculateQFactor(double f0, double fwhm) const; ///< Calculate Q factor from f0 and FWHM
			QVector<Peak> findSideModes(const QVector<double>& x, const QVector<double>& y1, double threshold = 0.1) const; ///< Find side modes above threshold

			QList<QVector<double>> xList; ///< List of X data vectors
			QList<QVector<double>> y1List; ///< List of Y data vectors
			QList<QString> valueList; ///< List of trace labels

			QVector<Peak> centerModeData; ///< Center mode peak data per trace
			QVector<QVector<Peak>> sideModeData; ///< Side mode peak data per trace
	};
#endif // SPECTRADATAPROCESSOR_H