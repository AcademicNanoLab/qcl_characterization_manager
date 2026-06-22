/**
 * @file IthDataProcessor.h
 * @brief Processes threshold current (Ith) data with fitting capabilities.
 * 
 * Provides exponential and polynomial fitting of Ith versus temperature,
 * and exposes fitting parameters and processed data vectors.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#ifndef ITHDATAPROCESSOR_H
	#define ITHDATAPROCESSOR_H

	#include "LIVDataProcessor.h"
	#include <QVector>
	#include <QObject>

	/**
	 * @class IthDataProcessor
	 * @brief Handles threshold current data processing and fitting.
	 */
	class IthDataProcessor : public QObject
	{
			Q_OBJECT

		public:
			explicit IthDataProcessor(LIVDataProcessor *data, double threshold = 3.0, QObject *parent = nullptr); ///< Constructor

			const QVector<double>& getTemperatures() const { return T; } ///< Get temperatures vector
			const QVector<double>& getThresholdCurrents() const { return Ith; } ///< Get Ith vector
			const QVector<double>& getDynamicRanges() const { return DR; } ///< Get dynamic ranges vector

			std::pair<QVector<double>, QVector<double>> applyExponentialFit(int numPoints); ///< Apply exponential fit and return fitted T and Ith
			std::pair<QVector<double>, QVector<double>> applyPolynomialFit(int numPoints, int order); ///< Apply polynomial fit and return fitted T and DR

			void getExponentialFitParams(double &A, double &B, double &C0) const; ///< Get exponential fit parameters A, B, C0
			void getPolynomialCoefficients(QVector<double>& coefficients) const; ///< Get polynomial coefficients

			bool canPlot() const { return Ith.size() >= 2; } ///< Check if data is sufficient for plotting

		private:
			QVector<double> T; ///< Temperatures
			QVector<double> Ith; ///< Threshold currents
			QVector<double> DR; ///< Dynamic ranges

			double A_exp; ///< Exponential fit parameter A
			double B_exp; ///< Exponential fit parameter B
			double C0_exp; ///< Exponential fit parameter C0
			QVector<double> polynomialCoefficients; ///< Polynomial fit coefficients

			bool polynomialFit(const QVector<double>& x, const QVector<double>& y, int order, QVector<double>& coefficients); ///< Polynomial fitting helper
			bool exponentialFit(const QVector<double>& x, const QVector<double>& y, double& A, double& B, double& C0); ///< Exponential fitting helper
			QVector<double> linspace(double start, double end, int numPoints); ///< Generate linear spaced vector
			void process(LIVDataProcessor *data, double threshold); ///< Process data from LIVDataProcessor
	};
#endif // ITHDATAPROCESSOR_H