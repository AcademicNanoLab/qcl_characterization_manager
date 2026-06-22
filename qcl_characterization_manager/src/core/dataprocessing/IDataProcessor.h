/**
 * @file IDataProcessor.h
 * @brief Interface for data processors handling LIV and spectral data.
 * 
 * Defines common fields and normalization interface for derived data processors.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#ifndef IDATAPROCESSOR_H
	#define IDATAPROCESSOR_H

	#include <QObject>
	#include <QString>
	#include <QVariantMap>
	#include <QVector>
	#include <QList>
	#include <QRegularExpression>

	/**
	 * @class IDataProcessor
	 * @brief Abstract base class for LIV and spectra data processing.
	 */
	class IDataProcessor : public QObject
	{
			Q_OBJECT

		public:
			explicit IDataProcessor(const QString &fileName,
								   const QString &unit,
								   const QString &traceVariable,
								   QObject *parent = nullptr); ///< Constructor initializing common data fields

			virtual void normalizeData() = 0; ///< Normalize data (pure virtual)

			QString fileName;        ///< Data source file name
			QString unit;            ///< Unit of measurement
			QString traceVariable;   ///< Variable name for measurement axis (e.g., temperature, current)

			QList<QString> valueList; ///< List of measurement values (e.g., temps or currents)

		protected:
			QRegularExpression *whiteSpaceRx; ///< Regex for whitespace splitting
	};
#endif // IDATAPROCESSOR_H