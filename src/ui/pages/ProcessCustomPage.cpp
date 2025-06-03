/**
 * @file ProcessCustomPage.cpp
 * @brief Implementation of ProcessCustomPage handling custom data processing and visualization.
 * 
 * This class provides UI elements to process custom measurement data,
 * displaying images in a carousel and offering controls to generate data sheets,
 * reset views, and navigate back. The layout splits into a main processing panel
 * and a scrollable image menu.
 * 
 * @author Aleksandar Demic
 */
#include "ProcessCustomPage.h"
#include <QStandardPaths>
#include "ui/dialogs/MessageBox.h"
#include "ui/components/containers/HeaderPage.h"
#include "ui/components/wizard/wizardpages/WizardSetupParamsPage.h"
#include "ui/components/text/Text.h"
#include "core/graceplots/LIVGracePlot.h"
#include "core/graceplots/SpectraGracePlot.h"
#include "core/graceplots/IthGracePlot.h"
#include "core/dataprocessing/LIVDataProcessor.h"
#include "core/dataprocessing/SpectraDataProcessor.h"
#include "core/dataprocessing/IthDataProcessor.h"
#include "core/fileconversion/FileConverter.h"
#include "core/datasheetgenerator/DataSheetGenerator.h"

/**
 * @brief Constructs the ProcessCustomPage with the specified title and optional parent.
 * 
 * Initializes UI components including widgets for no data display,
 * image generation controls, an image carousel, and navigation buttons.
 * Sets up layouts for left and right panels, with signals connected for interaction.
 * 
 * @param title The title of the wizard page.
 * @param parent The optional parent widget.
 */
ProcessCustomPage::ProcessCustomPage(const QString &title, QWidget *parent)
    : WizardPage(title, parent)
    , nothingToShowWidget(new Widget())
    , generateImagesControlWidget(new Widget())
    , imageCarousel(new ImageCarousel())
    , imageMenu(new ButtonGroup(ButtonGroup::VLayout))
    , resetButton(new PushButton("Reset", "contained", this))
    , backButton(new PushButton("Back", "outlined", this))
{
    // === Main horizontal layout ===
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);

    // === Left panel layout ===
    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    vBoxLayout->setContentsMargins(hBoxLayout->contentsMargins());
    vBoxLayout->setSpacing(20);

    // === Init widgets ===
    initNothingToShowWidget();
    initGenerateImagesControlWidget();

    // Add widgets individually with stretch factors
    vBoxLayout->addWidget(generateImagesControlWidget, 1);  // small stretch
    vBoxLayout->addWidget(nothingToShowWidget, 1);          // small stretch for "nothing to show"
    vBoxLayout->addWidget(imageCarousel, 8);                 // big stretch for carousel

    // Proper sizing of Generate Data Sheet button
    generateDataSheetButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    generateDataSheetButton->setMinimumWidth(140);
    vBoxLayout->addWidget(generateDataSheetButton, 0);      // no stretch, fixed size

    // === Bottom buttons layout ===
    QHBoxLayout *bottomButtonsLayout = new QHBoxLayout();
    bottomButtonsLayout->setContentsMargins(0, 0, 0, 0);
    bottomButtonsLayout->setSpacing(10);
    bottomButtonsLayout->addWidget(backButton, 0, Qt::AlignLeft);
    bottomButtonsLayout->addStretch();
    bottomButtonsLayout->addWidget(resetButton, 0, Qt::AlignRight);

    vBoxLayout->addLayout(bottomButtonsLayout);

    // === Separator line ===
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    // === Right panel: scrollable image menu ===
    QScrollArea *imageScrollArea = new QScrollArea();
    imageScrollArea->setWidget(imageMenu);
    imageScrollArea->setWidgetResizable(true);
    imageScrollArea->setMinimumWidth(350);
    imageScrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // === Combine layouts ===
    hBoxLayout->addLayout(vBoxLayout);
    hBoxLayout->addWidget(line);
    hBoxLayout->addWidget(imageScrollArea);

    // === Connections ===
    connect(resetButton, &PushButton::clicked, this, &ProcessCustomPage::resetView);
    connect(backButton, &PushButton::clicked, this, &ProcessCustomPage::onBackClicked);
    connect(imageMenu, &ButtonGroup::buttonClickedId, imageCarousel, &ImageCarousel::setImageStackIndex);

}

/**
 * @brief Initializes the "Nothing to Show" widget UI components.
 * 
 * This method sets up the layout and widgets for the placeholder UI displayed
 * when no data is currently available. It includes:
 * - A centered title instructing the user to select a data directory.
 * - A button to open a directory selection dialog for choosing the data folder.
 * - A button to trigger generation of Grace images, initially disabled and enabled only after
 *   a valid directory is selected.
 * 
 * The method also connects the buttons to their respective slots:
 * - The directory selection button opens a folder picker dialog and stores the selected path.
 * - The generate images button calls the image generation routine if a directory has been selected.
 * 
 * The widget size policies and layout alignments are configured to ensure consistent appearance.
 */
void ProcessCustomPage::initNothingToShowWidget()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);
    nothingToShowWidget->setLayout(layout);

    // Set size policy and max height to limit expansion
    nothingToShowWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    nothingToShowWidget->setMaximumHeight(200);  // Adjust this value as needed

    nothingToShowWidget->setObjectName("NothingToShowWidget");

    // Title text changed to "Select Data Directory"
    Text *text = new Text("Select Data Directory", "h3");
    text->setAlignment(Qt::AlignCenter);
    text->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);  // Fix vertical size
    layout->addWidget(text);

    // Select Data Directory button
    PushButton *selectDataDirButton = new PushButton("Select Data Directory", "contained");
    selectDataDirButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    layout->addWidget(selectDataDirButton);

    // Generate Grace Images button (initially disabled)
    PushButton *generateGraceImagesButton = new PushButton("Generate Grace Images", "contained");
    generateGraceImagesButton->setObjectName("generateGraceImagesButton");  // optional, useful for findChild
    generateGraceImagesButton->setEnabled(false);
    generateGraceImagesButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    layout->addWidget(generateGraceImagesButton);

    // Connect Select Data Directory button
    connect(selectDataDirButton, &QPushButton::clicked, this, [=]() 
    {
        QString dirPath = QFileDialog::getExistingDirectory(this, "Select Data Directory", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!dirPath.isEmpty()) 
        {
            outputDir = dirPath;
            qDebug() << "Selected data directory:" << outputDir;
            generateGraceImagesButton->setEnabled(true);
        }
    });

    connect(generateGraceImagesButton, &QPushButton::clicked, this, [=]() {
        if (!outputDir.isEmpty()) {
            generateGraceImages();
        }
    });
}

/**
 * @brief Initializes the widget controlling image generation actions.
 *
 * Sets up the layout and adds the "Generate Data Sheet" button, enabling it by default.
 * Connects the button's clicked signal to the slot responsible for generating the data sheet.
 */
void ProcessCustomPage::initGenerateImagesControlWidget()
{
    QVBoxLayout *layout = new QVBoxLayout();
    generateImagesControlWidget->setLayout(layout);
    generateImagesControlWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // Correct place to initialize it
    generateDataSheetButton = new PushButton("Generate Data Sheet", "contained", this);
    generateDataSheetButton->setEnabled(true);
    layout->addWidget(generateDataSheetButton);

    connect(generateDataSheetButton, &QPushButton::clicked, this, &ProcessCustomPage::generateDataSheet);
}

/**
 * @brief Adds an image to the image menu and carousel.
 *
 * Creates a new button in the image menu linked to the image carousel and adds the
 * image at the specified path to the carousel display.
 *
 * @param imagePath The file path to the image to be added.
 */
void ProcessCustomPage::addImage(const QString &imagePath)
{
    imageMenu->addImageButton(imageCarousel, SIGNAL(currentChanged(int)), imagePath);
    imageCarousel->addImage(imagePath);
	imageMenu->update();
    imageCarousel->update();
}

/**
 * @brief Resets the view to the initial state.
 *
 * Clears the selected output directory path, shows the placeholder widgets,
 * clears all images from the carousel and menu, re-enables the "Generate Data Sheet"
 * button, and disables the "Generate Grace Images" button.
 */
void ProcessCustomPage::resetView()
{
    // Clear stored output directory path
    outputDir.clear();

    // Show "Nothing to Show" widget and hide others
    nothingToShowWidget->show();
    generateImagesControlWidget->show();
    imageCarousel->show();

    // Clear images and menu
    imageCarousel->clear();
    imageMenu->clear();
    imageCarousel->update();

    // Disable "Generate Data Sheet" button and reset style
    generateDataSheetButton->setEnabled(true);
    generateDataSheetButton->setStyleSheet("");

    // Disable "Generate Grace Images" button inside nothingToShowWidget
    PushButton *generateGraceImagesButton = nothingToShowWidget->findChild<PushButton *>("generateGraceImagesButton");
    if (generateGraceImagesButton) {
        generateGraceImagesButton->setEnabled(false);
    }
}

/**
 * @brief Loads all generated PNG images from the "Figures" subdirectory within the output directory.
 *
 * Checks if the "Figures" directory exists under the current output directory.
 * If it exists, it scans for all PNG files and adds each one to the image carousel.
 * Logs a warning if the directory is missing.
 */
void ProcessCustomPage::loadGeneratedImagesFromFigures()
{
    QString figuresDirPath = outputDir + "/Figures";
    QDir figuresDir(figuresDirPath);

    if (!figuresDir.exists()) {
        qWarning() << "Figures directory not found:" << figuresDirPath;
        return;
    }

	QStringList pngFiles = figuresDir.entryList(QStringList() << "*.png", QDir::Files);
	for (const QString &pngFile : pngFiles)
	{
		QString fullPath = figuresDir.absoluteFilePath(pngFile);
		addImage(fullPath);
	}
}

/**
 * @brief Prompts the user to select a LaTeX (.tex) file and compiles it into a PDF data sheet.
 *
 * Opens a file dialog for the user to select a LaTeX source file.
 * Verifies the presence of the pdflatex executable.
 * Runs pdflatex asynchronously to compile the selected LaTeX file.
 * Displays a progress dialog during compilation.
 * Shows a message box upon success or failure of the PDF generation.
 */
void ProcessCustomPage::generateDataSheet()
{
    // Ask user to select a LaTeX file every time
    QString latexFilePath = QFileDialog::getOpenFileName(this, 
        "Select LaTeX File to Compile", QString(), "TeX Files (*.tex);;All Files (*)");

    if (latexFilePath.isEmpty()) {
        QMessageBox::warning(this, "No File Selected", "Please select a LaTeX (.tex) file to generate the data sheet.");
        return;
    }

    QFileInfo texFileInfo(latexFilePath);
    QString workDir = texFileInfo.absolutePath();
    QString texFileName = texFileInfo.fileName();

    QString pdflatexPath = QCoreApplication::applicationDirPath() + "/miktex-portable/texmfs/install/miktex/bin/x64/pdflatex.exe";
    if (!QFile::exists(pdflatexPath)) {
        QMessageBox::critical(this, "Error", "pdflatex executable not found:\n" + pdflatexPath);
        return;
    }

    QProgressDialog* progressDialog = new QProgressDialog("Compiling PDF...", QString(), 0, 0, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setCancelButton(nullptr);
    progressDialog->setMinimumDuration(0);
    progressDialog->show();

    QProcess* latexProcess = new QProcess(this);
    latexProcess->setWorkingDirectory(workDir);
    QStringList arguments = { "-interaction=nonstopmode", texFileName };

    connect(latexProcess, &QProcess::readyReadStandardOutput, this, [latexProcess]() {
        qDebug() << latexProcess->readAllStandardOutput();
    });

    connect(latexProcess, &QProcess::readyReadStandardError, this, [latexProcess]() {
        qWarning() << latexProcess->readAllStandardError();
    });

    connect(latexProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, latexProcess, latexFilePath, progressDialog](int exitCode, QProcess::ExitStatus) {
        progressDialog->close();

        QString pdfPath = latexFilePath;
        pdfPath.replace(".tex", ".pdf");

        if (exitCode == 0 && QFile::exists(pdfPath)) {
            QMessageBox::information(this, "Success", "PDF generated successfully:\n" + pdfPath);
        } else {
            QMessageBox::warning(this, "Failure", "PDF generation failed.");
        }

        latexProcess->deleteLater();
        progressDialog->deleteLater();
    });

    latexProcess->start(pdflatexPath, arguments);
}

/**
 * @brief Generates Grace plot images from .agr files located in the GraceFigures directory.
 *
 * Ensures the "GraceFigures" directory exists under the current output directory, creating it if necessary.
 * Checks for the presence of .agr files in that directory.
 * If no .agr files are found, displays a warning message.
 * Initiates the conversion of .agr files to PostScript and PDF formats using FileConverter.
 * After conversion, loads the generated images from the Figures directory.
 */
void ProcessCustomPage::generateGraceImages()
{
    QString graceFiguresDir = outputDir + "/GraceFigures";
    QDir dir(graceFiguresDir);

    // Ensure directory exists
    if (!dir.exists() && !dir.mkpath(graceFiguresDir)) {
        QMessageBox::critical(this, "Error", "Failed to create directory:\n" + graceFiguresDir);
        return;
    }

    // Check for .agr files
    QStringList agrFiles = dir.entryList(QStringList() << "*.agr", QDir::Files);
    if (agrFiles.isEmpty()) {
        QMessageBox::warning(this, "No Files", "No .agr files found in:\n" + graceFiguresDir);
        return;
    }

    // Start conversion
    FileConverter *converter = new FileConverter(this);
    connect(converter, &FileConverter::conversionFinished, this, [this]() 
    {
        loadGeneratedImagesFromFigures();
    });

    converter->processAgrFilesToPsAndPdf(graceFiguresDir);
}

/**
 * @brief Handles the "Back" button click event.
 *
 * Emits a signal indicating that the wizard or process should return to the welcome page.
 * Assumes the welcome page corresponds to index 0 in a QStackedWidget.
 */
void ProcessCustomPage::onBackClicked()
{
    emit finished(0);  // Assuming 0 is the index for welcome page in QStackedWidget
}