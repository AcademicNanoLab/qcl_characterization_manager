#ifndef QCUSTOMPLOTWRAPPER_H
#define QCUSTOMPLOTWRAPPER_H

#include "qcustomplot.h"

// Widget that makes vectors for plotting using a map of files.
// The map of files are in this format:
// {
//      name:       [string variablevalue]
// }
class QCustomPlotDataWrapper : public QObject
{
    Q_OBJECT
public:
    explicit QCustomPlotDataWrapper(const QString &fileName,
                                    const QVariantMap &files,
                                    const QString &variable,
                                    bool threeColFileFlg = false,
                                    QWidget *parent = nullptr);
    // saved strings
    QString fileName;
    QString suffix;
    QString variable;

    // related to parsing file
    void generateVectors(const QVariantMap &files, bool threeColFileFlg);
    void generateVectors(const QString &fileName,
                         QVector<double> *x,
                         QVector<double> *y1,
                         QVector<double> *y2);
    QRegularExpression *whiteSpaceRx;

    // saved vectors
    double minX, minY1, minY2;
    double maxX, maxY1, maxY2;
    QList<QString> valueList;
    QList<QVector<double>> xList, y1List, y2List;

signals:
};

class QCustomPlotWrapper : public QCustomPlot
{
    Q_OBJECT
public:
    explicit QCustomPlotWrapper(QCustomPlotDataWrapper *data, QWidget *parent = nullptr);

    // related to plotting
    double setTickStep(QList<QCPAxis *> axisList, double tickStep);
    double setTickStep(QList<QCPAxis *> axisList, double &lower, double &upper);
    void setRange(QList<QCPAxis *> axis, double lower, double upper, double adjustVal = -1);
    QColor valueToColor(const QString &valueString, const QString &variable);
    QColor toRainbowColor(const QString &valueString, double lower, double upper);

    // pointer to data object
    QCustomPlotDataWrapper *data;
};

#endif // QCUSTOMPLOTWRAPPER_H
