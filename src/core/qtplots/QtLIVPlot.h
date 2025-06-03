#ifndef QTLIVPLOT_H
#define QTLIVPLOT_H

#include "qcustomplotwrapper.h"
#include "core/dataprocessing/LIVDataProcessor.h"

class QtLIVPlot : public QCustomPlotWrapper
{
    Q_OBJECT
public:
    explicit QtLIVPlot(LIVDataProcessor *data,
					 const QString &outputDirectory,
                     const double &w,
                     const double &l,
                     QWidget *parent = nullptr);
private:
    double m_w;
    double m_l;
private slots:
    void updateSecondaryXAxis();
};

#endif // QTLIVPLOT_H
