#ifndef LIVPLOT_H
#define LIVPLOT_H

#include "qcustomplotwrapper.h"

class LIVPlot : public QCustomPlotWrapper
{
    Q_OBJECT
public:
    explicit LIVPlot(QCustomPlotDataWrapper *data,
                     const double &w,
                     const double &l,
                     QWidget *parent = nullptr);
};

#endif // LIVPLOT_H
