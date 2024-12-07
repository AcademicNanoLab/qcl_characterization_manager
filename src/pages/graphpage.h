#ifndef GRAPHPAGE_H
#define GRAPHPAGE_H

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include "components/buttongroup.h"
#include "components/directoryselector.h"
#include "components/imagecarousel.h"
#include "components/widget.h"
#include "plotComponents/livplot.h"
#include "plotComponents/spectraplot.h"

class GraphPage : public Widget
{
    Q_OBJECT
public:
    explicit GraphPage(QWidget *parent = nullptr);
    void initNothingToShowWidget();

    DirectorySelector *chooseImageDirectoryWidget;
    DirectorySelector *chooseSaveGraphDirectoryWidget;
    Widget *nothingToShowWidget;
    ImageCarousel *imageCarousel;
    ButtonGroup *imageMenu;

    void setImageDir(const QString &text);
    QString saveDir;

    bool fieldsIsEmpty() { return fields.size() <= 4; }
    QVariantMap fields;

    void addImage(QString imagePath)
    {
        imageMenu->addImageButton(imageCarousel, SIGNAL(currentChanged(int)), imagePath);
        imageCarousel->addImage(imagePath);
    }
    void addImage(QCustomPlotDataWrapper *data, double w, double l, const QString &label)
    {
        imageMenu->addMenuButton(imageCarousel, SIGNAL(currentChanged(int)), label);
        imageCarousel->addImage(new LIVPlot(data, w, l));
    }
    void addImage(QCustomPlotDataWrapper *data, const QString &label)
    {
        imageMenu->addMenuButton(imageCarousel, SIGNAL(currentChanged(int)), label);
        imageMenu->addMenuButton(imageCarousel, SIGNAL(currentChanged(int)), label);
        imageCarousel->addImage(new SpectraPlotSamePlot(data));
        imageCarousel->addImage(new SpectraPlotStacked(data));
    }

signals:
    void finished(int id);

public slots:
    void setFields(const QVariantMap &_fields);
    void analysisButtonGroupClick(int id);
    void optionsButtonGroupClick(int id);
    void chooseImageDir(const QString &dir);
    void clearImageDir();
    void chooseSaveDir(const QString &dir);
    void saveGraphs();
};

#endif // GRAPHPAGE_H
