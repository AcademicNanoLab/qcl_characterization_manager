/**
 * @file LIVDataProcessor.h
 * @brief Data processor for LIV (Light-Current-Voltage) measurement data.
 * 
 * Parses, normalizes, and provides access to LIV data vectors
 * along with metadata like min/max values and scaling.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#ifndef LIVDATAPROCESSOR_H
	#define LIVDATAPROCESSOR_H

	#include "IDataProcessor.h"

	/**
	 * @class LIVDataProcessor
	 * @brief Processes LIV measurement data and normalizes it.
	 */
	class LIVDataProcessor : public IDataProcessor
	{
		public:
			explicit LIVDataProcessor(const QString &fileName,
									  const QVariantMap &files,
									  const QString &traceVariable,
									  double scaleFactor = 100.0); ///< Constructor

			void normalizeData() override; ///< Normalize the LIV data

			QList<QVector<double>>& getXList() { return xList; } ///< Get list of x data vectors
			QList<QVector<double>>& getY1List() { return y1List; } ///< Get list of y1 data vectors
			QList<QVector<double>>& getY2List() { return y2List; } ///< Get list of y2 data vectors

			const QList<QString>& getValueList() const { return valueList; } ///< Get trace variable values

			double getMinX() const { return minX; } ///< Get minimum X value across data
			double getMaxX() const { return maxX; } ///< Get maximum X value across data
			double getMinY1() const { return minY1; } ///< Get minimum Y1 value
			double getMaxY1() const { return maxY1; } ///< Get maximum Y1 value
			double getPreNormMaxY2() const { return preNormMaxY2; } ///< Get pre-normalized max Y2 value
			double getScaleFactor() const { return scaleFactor; } ///< Get scale factor used

		private:
			void generateVectors(const QVariantMap &files); ///< Generate data vectors from files
			void generateVectorsFromFile(const QString &fileName,
										 QVector<double> *x,
										 QVector<double> *y1,
										 QVector<double> *y2); ///< Generate vectors from a single file

			QList<QVector<double>> xList; ///< X data vectors
			QList<QVector<double>> y1List; ///< Y1 data vectors
			QList<QVector<double>> y2List; ///< Y2 data vectors
			QList<QString> valueList; ///< Trace variable values

			double minX, maxX; ///< Min and max X values
			double minY1, maxY1; ///< Min and max Y1 values
			double preNormMaxY2; ///< Max Y2 value before normalization
			double scaleFactor; ///< Scale factor applied to data
	};
#endif // LIVDATAPROCESSOR_H