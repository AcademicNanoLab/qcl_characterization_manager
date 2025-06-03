#ifndef QCUSTOMPLOTWRAPPER_H
#define QCUSTOMPLOTWRAPPER_H

#include "qcustomplot.h"
#include "core/dataprocessing/IDataProcessor.h"



class QCustomPlotWrapper : public QCustomPlot
{
	Q_OBJECT
	public:
		explicit QCustomPlotWrapper(IDataProcessor *data, QWidget *parent = nullptr);

		// related to plotting
		double setTickStep(QList<QCPAxis *> axisList, double tickStep);
		double setTickStep(QList<QCPAxis *> axisList, double &lower, double &upper);
		void setRange(QList<QCPAxis *> axis, double lower, double upper, double adjustVal = -1);
		QColor valueToColor(const QString &valueString, const QString &variable);
		QColor toRainbowColor(const QString &valueString, double lower, double upper);

		// pointer to data object
		IDataProcessor *data;
};

#endif // QCUSTOMPLOTWRAPPER_H
