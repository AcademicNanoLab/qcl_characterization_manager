#include "graphpage.h"
#include <QStandardPaths>
#include "components/directoryselector.h"
#include "components/headerpage.h"
#include "components/messagebox.h"
#include "components/text.h"

GraphPage::GraphPage(QWidget *parent)
    : Widget{parent}
    , chooseImageDirectoryWidget(new DirectorySelector(this,
                                                       SLOT(chooseImageDir(QString)),
                                                       "Clear",
                                                       this,
                                                       SLOT(clearImageDir())))
    , chooseSaveGraphDirectoryWidget(
          new DirectorySelector(this, SLOT(chooseSaveDir(QString)), "Save", this, SLOT(saveGraphs())))
    , nothingToShowWidget(new Widget())
    , imageCarousel(new ImageCarousel())
    , imageMenu(new ButtonGroup(ButtonGroup::VLayout))
    , saveDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation))
{
    // layout
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    QVBoxLayout *vBoxLayout = new QVBoxLayout();

    // layout
    vBoxLayout->setContentsMargins(hBoxLayout->contentsMargins());
    vBoxLayout->setSpacing(20);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    // initialize widgets
    initNothingToShowWidget();
    setImageDir("");

    // make page
    HeaderPage *page = new HeaderPage("Graph Page");
    page->addWidget(chooseImageDirectoryWidget);
    page->addWidget(chooseSaveGraphDirectoryWidget);
    page->addWidget(nothingToShowWidget);
    page->addWidget(imageCarousel);

    // make "Back" button
    ButtonGroup *analysisButtons = new ButtonGroup(ButtonGroup::HLayout);
    analysisButtons->layout()->setContentsMargins(0, 0, 0, 0);
    analysisButtons->addButton(new PushButton("Back", "contained"));
    ((QHBoxLayout *) analysisButtons->layout())
        ->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum));
    analysisButtons->addButton(new PushButton("Advanced Options", "contained"));

    // make divider
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    // image menu
    QScrollArea *imageScrollArea = new QScrollArea();
    imageScrollArea->setWidget(imageMenu);
    imageScrollArea->setWidgetResizable(true);
    imageScrollArea->setMinimumWidth(350);
    imageScrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // make layout
    vBoxLayout->addWidget(page);
    vBoxLayout->addWidget(analysisButtons);

    hBoxLayout->addLayout(vBoxLayout);
    hBoxLayout->addWidget(line);
    hBoxLayout->addWidget(imageScrollArea);

    // connect signals
    connect(analysisButtons,
            &ButtonGroup::buttonClickedId,
            this,
            &GraphPage::analysisButtonGroupClick);
    connect(imageMenu,
            &ButtonGroup::buttonClickedId,
            imageCarousel,
            &ImageCarousel::setImageStackIndex);
}

// This is called to modify the Image directory
void GraphPage::setImageDir(const QString &text)
{
    imageCarousel->clear();
    imageMenu->clear();
    if (text.isEmpty() && fieldsIsEmpty()) {
        chooseImageDirectoryWidget->hide();
        chooseSaveGraphDirectoryWidget->hide();
        imageCarousel->hide();
        nothingToShowWidget->show();
    } else if (text.isEmpty()) {
        chooseImageDirectoryWidget->hide();
        chooseSaveGraphDirectoryWidget->show();
        imageCarousel->show();
        nothingToShowWidget->hide();
    } else {
        chooseImageDirectoryWidget->show();
        chooseSaveGraphDirectoryWidget->hide();
        imageCarousel->show();
        nothingToShowWidget->hide();
    }
}

// This is called when the wizard finishes and signals the fields
void GraphPage::setFields(const QVariantMap &_fields)
{
    fields = _fields;
    setImageDir("");

    if (fieldsIsEmpty())
        return;

    // dimensions
    QVariantMap dimensions = fields.value("Dimensions").toMap();
    double w = dimensions.value("width").toDouble();
    double l = dimensions.value("length").toDouble();

    // graph
    if (fields.contains("Pulse LIV")) {
        QString field = "Pulse LIV";
        QCustomPlotDataWrapper *data = new QCustomPlotDataWrapper(field,
                                                                  *reinterpret_cast<QVariantMap *>(
                                                                      &fields[field]),
                                                                  "temperature",
                                                                  true);
        addImage(data, w, l, field);
    }
    if (fields.contains("Pulse FDR - fixed temperature")) {
        QString field = "Pulse FDR - fixed temperature";
        QCustomPlotDataWrapper *data = new QCustomPlotDataWrapper(field,
                                                                  *reinterpret_cast<QVariantMap *>(
                                                                      &fields[field]),
                                                                  "current",
                                                                  false);
        addImage(data, field);
    }
    if (fields.contains("Pulse FDR - fixed current")) {
        QString field = "Pulse FDR - fixed current";
        QCustomPlotDataWrapper *data = new QCustomPlotDataWrapper(field,
                                                                  *reinterpret_cast<QVariantMap *>(
                                                                      &fields[field]),
                                                                  "temperature",
                                                                  false);
        addImage(data, field);
    }
    if (fields.contains("CW LIV")) {
        QString field = "CW LIV";
        QCustomPlotDataWrapper *data = new QCustomPlotDataWrapper(field,
                                                                  *reinterpret_cast<QVariantMap *>(
                                                                      &fields[field]),
                                                                  "temperature",
                                                                  true);
        addImage(data, w, l, field);
    }
    if (fields.contains("CW FDR - fixed temperature")) {
        QString field = "CW FDR - fixed temperature";
        QCustomPlotDataWrapper *data = new QCustomPlotDataWrapper(field,
                                                                  *reinterpret_cast<QVariantMap *>(
                                                                      &fields[field]),
                                                                  "current",
                                                                  false);
        addImage(data, field);
    }
    if (fields.contains("CW FDR - fixed current")) {
        QString field = "CW FDR - fixed current";
        QCustomPlotDataWrapper *data = new QCustomPlotDataWrapper(field,
                                                                  *reinterpret_cast<QVariantMap *>(
                                                                      &fields[field]),
                                                                  "temperature",
                                                                  false);
        addImage(data, field);
    }
}

// This is called when either the "Back" or "Advanced Options" buttons are clicked
void GraphPage::analysisButtonGroupClick(int id)
{
    if (id == 0)
        emit finished(0);
    else if (fieldsIsEmpty()) {
        MessageBox *msgBox
            = new MessageBox(QMessageBox::Warning,
                             "",
                             "You cannot modify image files from a directory. If you'd like to "
                             "generate a new set of graphs, go through the wizard again.",
                             QMessageBox::Ok);
        msgBox->exec();
    }
}

// nothingToShowWidget
//
//
//
// This initializes the widget showing there is nothing to show
// Users can either choose a directory or start the wizard
void GraphPage::initNothingToShowWidget()
{
    // layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);
    nothingToShowWidget->setLayout(layout);
    nothingToShowWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // for styling
    nothingToShowWidget->setObjectName("NothingToShowWidget");

    // heading
    Text *text = new Text("Nothing to show", "h3");
    text->setAlignment(Qt::AlignCenter);
    layout->addWidget(text);

    // button group
    ButtonGroup *optionsButtonGroup = new ButtonGroup(ButtonGroup::VLayout);
    optionsButtonGroup->addButton(new PushButton("Choose from directory", "contained"));
    optionsButtonGroup->addButton(new PushButton("Go through wizard", "contained"));
    layout->addWidget(optionsButtonGroup);

    // connect signals
    connect(optionsButtonGroup,
            &ButtonGroup::buttonClickedId,
            this,
            &GraphPage::optionsButtonGroupClick);
}

// This is called when a button on the "Nothing to Show" menu is clicked
void GraphPage::optionsButtonGroupClick(int id)
{
    if (id == 0)
        chooseImageDirectoryWidget->chooseDir();
    else
        emit finished(1);
}

// [Image Directory] This is called when the "Choose directory" button is clicked
void GraphPage::chooseImageDir(const QString &dir)
{
    setImageDir(dir);
    QDir directory(dir);
    QStringList images = directory.entryList({"*.png", "*.xpm", " *.jpg"}, QDir::Files);

    // add new
    for (const QString &imageName : images) {
        QString imagePath = dir + "/" + imageName;
        addImage(imagePath);
    }
}

// [Image Directory] This is called when the "Clear directory button is clicked
void GraphPage::clearImageDir()
{
    setImageDir("");
}

// [Saving Graphs Directory] This is called when the "Choose directory" button is clicked
void GraphPage::chooseSaveDir(const QString &dir)
{
    saveDir = dir;
}

// [Saving Graphs Directory] This is called when the "Save graphs" button is clicked
void GraphPage::saveGraphs()
{
    for (QCustomPlot *plot : imageCarousel->imageStack->findChildren<QCustomPlot *>()) {
        plot->savePng(saveDir + "/" + plot->objectName() + ".png", 0, 0, 1.0, 80);
    }
}
