/**
 * @file WizardGracePage.cpp
 * @brief Implementation of WizardGracePage managing visualization and controls for Grace plots.
 * 
 * This class extends WizardPage and provides widgets to display Grace plots,
 * controls to generate images, and an image carousel with a side menu.
 * It also includes a reset button to reset the view.
 * 
 * Dependencies include various plot classes, data processors, file converters, and datasheet generators.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "WizardGracePage.h"
#include <QStandardPaths>
#include "ui/components/containers/HeaderPage.h"
#include "ui/components/text/Text.h"
#include "ui/dialogs/MessageBox.h"
#include "core/graceplots/LIVGracePlot.h"
#include "core/graceplots/SpectraGracePlot.h"
#include "core/graceplots/IthGracePlot.h"
#include "core/dataprocessing/LIVDataProcessor.h"
#include "core/dataprocessing/SpectraDataProcessor.h"
#include "core/dataprocessing/IthDataProcessor.h"
#include "core/fileconversion/FileConverter.h"
#include "core/datasheetgenerator/DataSheetGenerator.h"


/**
 * @brief Constructs a WizardGracePage with UI setup for Grace plot visualization and control.
 * 
 * @param title The page title.
 * @param parent Optional parent widget.
 * 
 * Initializes widgets for:
 * - A "nothing to show" placeholder.
 * - Image generation controls.
 * - An image carousel to display plots.
 * - A vertical button group as an image menu.
 * - A reset button to reset the displayed view.
 * 
 * Layout:
 * - Left vertical layout with header page containing controls and carousel.
 * - Vertical separator line.
 * - Scrollable side menu for images.
 * 
 * Connects signals from the image menu buttons to update the carousel view.
 * Hides the default page title.
 */
WizardGracePage::WizardGracePage(const QString &title, QWidget *parent)
    : WizardPage(title, parent)
    , nothingToShowWidget(new Widget())
    , generateImagesControlWidget(new Widget())
    , imageCarousel(new ImageCarousel())
    , imageMenu(new ButtonGroup(ButtonGroup::VLayout))
    , generateDataSheetButton(nullptr)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    QVBoxLayout *vBoxLayout = new QVBoxLayout();

    vBoxLayout->setContentsMargins(hBoxLayout->contentsMargins());
    vBoxLayout->setSpacing(20);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    initNothingToShowWidget();
    initGenerateImagesControlWidget();
	
	// Reset button
	resetButton = new PushButton("Reset", "contained");
	connect(resetButton, &QPushButton::clicked, this, &WizardGracePage::resetView);
	
    HeaderPage *page = new HeaderPage("Grace figures");
    page->addWidget(generateImagesControlWidget);
    page->addWidget(nothingToShowWidget);
    page->addWidget(imageCarousel);
	page->addWidget(resetButton);  // Added Reset Button here

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    QScrollArea *imageScrollArea = new QScrollArea();
    imageScrollArea->setWidget(imageMenu);
    imageScrollArea->setWidgetResizable(true);
    imageScrollArea->setMinimumWidth(350);
    imageScrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    vBoxLayout->addWidget(page);

    hBoxLayout->addLayout(vBoxLayout);
    hBoxLayout->addWidget(line);
    hBoxLayout->addWidget(imageScrollArea);

    connect(imageMenu, &ButtonGroup::buttonClickedId, imageCarousel, &ImageCarousel::setImageStackIndex);

    this->hideTitle();
}

/**
 * @brief Initializes the "Nothing to Show" widget.
 * 
 * This widget displays a centered message indicating that there is no content to show,
 * along with a button to trigger the generation of Grace images.
 * 
 * The button is connected to the generateGraceImages() slot.
 */
void WizardGracePage::initNothingToShowWidget()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);
    nothingToShowWidget->setLayout(layout);
    nothingToShowWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    nothingToShowWidget->setObjectName("NothingToShowWidget");

    Text *text = new Text("Nothing to show", "h3");
    text->setAlignment(Qt::AlignCenter);
    layout->addWidget(text);

    PushButton *generateGraceImagesButton = new PushButton("Generate Grace Images", "contained");
    layout->addWidget(generateGraceImagesButton);
    connect(generateGraceImagesButton, &QPushButton::clicked, this, &WizardGracePage::generateGraceImages);
}

/**
 * @brief Initializes the widget containing controls for image generation.
 * 
 * Currently includes a "Generate Data Sheet" button which is disabled by default.
 * 
 * The button is connected to the generateDataSheet() slot.
 */
void WizardGracePage::initGenerateImagesControlWidget()
{
    QVBoxLayout *layout = new QVBoxLayout();
    generateImagesControlWidget->setLayout(layout);
    generateImagesControlWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // Initialize the "Generate Data Sheet" button and disable it initially
    generateDataSheetButton = new PushButton("Generate Data Sheet", "contained");
    generateDataSheetButton->setEnabled(false);  // Initially disabled
    layout->addWidget(generateDataSheetButton);

    connect(generateDataSheetButton, &QPushButton::clicked, this, &WizardGracePage::generateDataSheet);
}

/**
 * @brief Adds an image to the image menu and carousel.
 * 
 * @param imagePath The filesystem path of the image to add.
 * 
 * Adds a button to the image menu linked to the image carousel,
 * and adds the image itself to the carousel display.
 * Updates both widgets after adding.
 */
void WizardGracePage::addImage(const QString &imagePath)
{
    imageMenu->addImageButton(imageCarousel, SIGNAL(currentChanged(int)), imagePath);
    imageCarousel->addImage(imagePath);
	imageMenu->update();
    imageCarousel->update();
}

/**
 * @brief Adds an image to the image menu and carousel.
 * 
 * @param imagePath The filesystem path of the image to add.
 * 
 * Adds a button to the image menu linked to the image carousel,
 * and adds the image itself to the carousel display.
 * Updates both widgets after adding.
 */
void WizardGracePage::resetView()
{
    // Show the "Nothing to Show" widget
    nothingToShowWidget->show();

    // Clear images from the carousel (implement clear() in ImageCarousel)
    imageCarousel->clear();
	imageMenu->clear();
    imageCarousel->update();

    // Disable and reset the "Generate Data Sheet" button
    generateDataSheetButton->setEnabled(false);
    generateDataSheetButton->setStyleSheet("");  // reset style to default
}

/**
 * @brief Loads all generated PNG images from the "Figures" directory in the output path.
 * 
 * @note Assumes that the outputDir member variable is set.
 * 
 * Scans the "Figures" directory for PNG files, adding each to the image menu and carousel.
 * Logs a warning if the directory does not exist.
 */
void WizardGracePage::loadGeneratedImagesFromFigures()
{
    QString figuresDirPath = outputDir + "/Figures";
    QDir figuresDir(figuresDirPath);

    if (!figuresDir.exists()) {
        qWarning() << "Figures directory not found:" << figuresDirPath;
        return;
    }

	QStringList pngFiles = figuresDir.entryList(QStringList() << "*.png", QDir::Files);
	for (const QString &pngFile : pngFiles) {
		QString fullPath = figuresDir.absoluteFilePath(pngFile);
		addImage(fullPath);
	}
}

/**
 * @brief Sets the internal data fields and output directory for the page.
 * 
 * @param map A QVariantMap containing key-value pairs of collected data.
 * @param dir A QString specifying the output directory path.
 * 
 * Stores the provided data map and output directory for later use.
 * Also outputs debug information about the keys received and the directory.
 */
void WizardGracePage::setFields(const QVariantMap &map, const QString &dir)
{
	qDebug() << "setFields called. Keys:" << map.keys();  
    qDebug() << "Output directory:" << dir;                
    
    collectedData = map;
    outputDir = dir;
}

/**
 * @brief Generates a LaTeX data sheet PDF based on collected measurement data and figures.
 * 
 * This function processes the stored data and metadata, gathers relevant figures from the
 * output directory, generates a LaTeX file using the DataSheetGenerator class, and compiles
 * it to a PDF using the pdflatex executable. It provides user feedback via progress dialogs
 * and message boxes for success or failure notifications.
 * 
 * The function expects `collectedData` and `outputDir` to be set prior to calling.
 * It filters data keys to separate pulsed and continuous-wave (CW) metadata and looks
 * for relevant figure files (.pdf) in the "Figures" subdirectory.
 * 
 * If data is missing or pdflatex executable is not found, the process is aborted with warnings.
 * 
 * On successful PDF generation, the user is notified and the "Generate Data Sheet" button
 * is enabled for further use.
 */
void WizardGracePage::generateDataSheet() 
{
    if (collectedData.isEmpty()) {
        QMessageBox::warning(this, "Error", "No data available to generate data sheet.");
        return;
    }

    QString outputTexPath = outputDir + "/LaserDataSheet.tex";  // Define output tex path

    // Extract author and date from collectedData
    QString author = collectedData.value("author").toString();
    QString date = collectedData.value("date").toString();

    if (author.isEmpty()) {
        author = "Default Author";
    }
    if (date.isEmpty()) {
        date = QDate::currentDate().toString("dd-MM-yyyy");
    }

    // Construct metadata maps
    QMap<QString, QString> pulsedMetadata;
    QMap<QString, QString> cwMetadata;

    for (auto it = collectedData.begin(); it != collectedData.end(); ++it) 
    {
        QString key = it.key();
        QString value = it.value().toString();

        // Pulsed LIV keys
        if (key.contains("pulsed_") && key.endsWith("_liv")) {
            pulsedMetadata[key] = value;
        } 
        // Pulsed Spectra keys
        else if (key.contains("pulsed_") && key.endsWith("_spectra")) {
            pulsedMetadata[key] = value;
        }
        // Pulsed experimental notes
        else if (key.startsWith("pulsed_") && key.endsWith("_experimental_notes")) {
            pulsedMetadata[key] = value;
        }
        // CW experimental notes
        else if (key.startsWith("cw_") && key.endsWith("_experimental_notes")) {
            cwMetadata[key] = value;
        }
        // CW LIV and spectra keys (optional, depending on use case)
        else if (key.contains("cw_") && (key.endsWith("_liv") || key.endsWith("_spectra"))) {
            cwMetadata[key] = value;
        }
    }

    // Collect figures from the figures directory
    QMap<QString, QVector<QString>> figuresMap;
    QString figuresDir = outputDir + "/Figures";
    QDir dir(figuresDir);
    if (dir.exists()) {
        QStringList files = dir.entryList(QStringList() << "*.pdf", QDir::Files);
        for (const QString &file : files) {
            QString lower = file.toLower();
            QString fullPath = dir.absoluteFilePath(file);
            if (lower.contains("pulsed") && lower.contains("liv")) {
                figuresMap["pulsed_liv"].append(fullPath);
            } else if (lower.contains("pulsed") && lower.contains("temp")) {
                figuresMap["pulsed_spectra_temp"].append(fullPath);
            } else if (lower.contains("pulsed") && lower.contains("current")) {
                figuresMap["pulsed_spectra_current"].append(fullPath);
            } else if (lower.contains("cw") && lower.contains("liv")) {
                figuresMap["cw_liv"].append(fullPath);
            } else if (lower.contains("cw") && lower.contains("temp")) {
                figuresMap["cw_spectra_temp"].append(fullPath);
            } else if (lower.contains("cw") && lower.contains("current")) {
                figuresMap["cw_spectra_current"].append(fullPath);
            }
        }
    } else {
        qWarning() << "Figures directory does not exist:" << figuresDir;
    }

    // Generate LaTeX data sheet
    DataSheetGenerator generator(outputTexPath, collectedData);
    generator.setMeasurementMetadata(pulsedMetadata, cwMetadata);
    generator.setFigures(figuresMap);
    generator.generate();

    qDebug() << "Laser Data Sheet LaTeX written to:" << outputTexPath;

    // Progress dialog for PDF compilation
    QProgressDialog* progressDialog = new QProgressDialog("Compiling PDF...", QString(), 0, 0, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setCancelButton(nullptr);
    progressDialog->setMinimumDuration(0);
    progressDialog->show();

    // Path to pdflatex executable
    QString pdflatexPath = QCoreApplication::applicationDirPath() + "/miktex-portable/texmfs/install/miktex/bin/x64/pdflatex.exe";
    if (!QFile::exists(pdflatexPath)) {
        qWarning() << "pdflatex not found at:" << pdflatexPath;
        progressDialog->close();
        return;
    }

    QFileInfo texFileInfo(outputTexPath);
    QString workDir = texFileInfo.absolutePath();
    QString texFileName = texFileInfo.fileName();

    QProcess* latexProcess = new QProcess(this);
    latexProcess->setWorkingDirectory(workDir);
    QStringList arguments;
    arguments << "-interaction=nonstopmode" << texFileName;

    connect(latexProcess, &QProcess::readyReadStandardOutput, this, [=]() {
        qDebug() << latexProcess->readAllStandardOutput();
    });

    connect(latexProcess, &QProcess::readyReadStandardError, this, [=]() {
        qWarning() << latexProcess->readAllStandardError();
    });

    connect(latexProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus /*status*/) {
        progressDialog->close();
        generateDataSheetButton->setEnabled(true);

        QString pdfPath = outputTexPath;
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
 * @brief Generates Grace plot images from the collected measurement data.
 * 
 * This function processes measurement data stored in collectedData and generates
 * various plots (LIV, Ith vs Temperature, FTIR spectra) using dedicated plotter classes.
 * Generated plots are saved as .agr files in a "GraceFigures" subdirectory under the output directory.
 * 
 * It handles both pulsed and continuous-wave (CW) measurement types, including:
 * - Pulsed LIV and Ith plots
 * - Pulsed FTIR at fixed temperature and fixed current
 * - CW LIV and Ith plots
 * - CW FTIR at fixed temperature and fixed current
 * 
 * The function creates necessary directories, instantiates data processors and plotters,
 * and updates collectedData with fit parameters from Ith plots.
 * 
 * After plots are generated, the .agr files are converted to PDFs asynchronously using FileConverter.
 * Upon conversion completion, the UI is updated to show generated images and enable the data sheet generation button.
 * 
 * Emits:
 * - dataProcessed(const QVariantMap &) when Ith plot parameters are updated.
 * 
 * Handles errors such as missing directories or insufficient data gracefully.
 */
void WizardGracePage::generateGraceImages()
{
    nothingToShowWidget->hide();  // Hide the "Nothing to Show" widget after generating Grace images
   
    if (collectedData.isEmpty()) {
        return; // No data, exit early
    }

    // Get dimensions
    QVariantMap dimensions = collectedData.value("Dimensions").toMap();
    double w = dimensions.value("width").toDouble();
    double l = dimensions.value("length").toDouble();

    QString graceFiguresDir = outputDir + "/GraceFigures";
    QDir dir(graceFiguresDir);

    // Create directory if it doesn't exist
    if (!dir.exists() && !dir.mkpath(graceFiguresDir)) {
        qWarning() << "Failed to create directory:" << graceFiguresDir;
        return; // Handle failure appropriately
    }

    // Create instances of the plotters
    LIVGracePlot livPlot;
    SpectraGracePlot spectraPlot;
    IthGracePlot ithPlot;

    // Check for and handle data for "Pulsed LIV"
    if (collectedData.contains("Pulsed LIV")) {
        QString field = "Pulsed LIV";
		LIVDataProcessor *livData = new LIVDataProcessor(field, collectedData[field].toMap(), "temperature", collectedData.value("pulsed_power_scale_liv", 100.0).toDouble());
        
        // Generate the LIV plot
        QString livOutputPath = graceFiguresDir + "/pulsed_liv.agr";
        livPlot.plot_liv(livOutputPath.toStdString(), livData, w, l);

        // Create IthDataProcessor for Ith vs T plot (based on LIV data)
        IthDataProcessor *ithData = new IthDataProcessor(livData, 3.0, this);  // Passing LIV data and threshold
        QString ithOutputPath = graceFiguresDir + "/Ith_vs_T_pulsed_liv.agr";
        
		if (ithData->canPlot()) 
		{
			ithPlot.plot_Ith_vs_T(ithOutputPath.toStdString(), ithData, w, l);

			double A, B, C0;
			ithData->getExponentialFitParams(A, B, C0);

			collectedData["pulsed_I_exp_A"] = QString::number(A, 'f', 2);
			collectedData["pulsed_I_exp_B"] = QString::number(B, 'f', 2);
			collectedData["pulsed_I_exp_C0"] = QString::number(C0, 'f', 2);

			double scale = 1e5 / (w * l);

			collectedData["pulsed_J_exp_A"] = QString::number(A * scale, 'f', 2);
			collectedData["pulsed_J_exp_B"] = QString::number(B * scale, 'f', 2);
			collectedData["pulsed_J_exp_C0"] = QString::number(C0 * scale, 'f', 2);

			// Emit the signal with the updated collectedData
			emit dataProcessed(collectedData);
		}
		else 
			qDebug() << "Skipping Ith plot: insufficient valid traces";
    }

	// Pulsed FTIR - fixed temperature
	if (collectedData.contains("Pulsed FTIR - fixed temperature")) {
		QString field = "Pulsed FTIR - fixed temperature";
		SpectraDataProcessor* spectraData = new SpectraDataProcessor(
			field,
			collectedData[field].toMap(),
			"current",
			collectedData.value("pulsed_fmin_spectra", 0.0).toDouble(),
			collectedData.value("pulsed_fmax_spectra", 0.0).toDouble()
		);        
		QString spectraOutputPath = graceFiguresDir + "/pulsed_ftir_vs_I.agr";
		spectraPlot.plot_spectra_waterfall(spectraOutputPath.toStdString(), spectraData);

		// Add frequency range string
		QString freqRange = spectraData->getGlobalFrequencyRangeString();
		if (!freqRange.isEmpty()) {
			collectedData["pulsed_ftir_fixed_temp_freq_range"] = freqRange;
		}
	}

    // Check for and handle data for "Pulsed FTIR - fixed current"
    if (collectedData.contains("Pulsed FTIR - fixed current")) {
        QString field = "Pulsed FTIR - fixed current";
		SpectraDataProcessor* spectraData = new SpectraDataProcessor(
			field,
			collectedData[field].toMap(),
			"temperature",
			collectedData.value("pulsed_fmin_spectra", 0.0).toDouble(),
			collectedData.value("pulsed_fmax_spectra", 0.0).toDouble()
		);        
        // Generate the Spectra plot
        QString spectraOutputPath = graceFiguresDir + "/pulsed_ftir_vs_T.agr";
        spectraPlot.plot_spectra_waterfall(spectraOutputPath.toStdString(), spectraData);
    }

    // Check for and handle data for "CW LIV"
    if (collectedData.contains("CW LIV")) {
        QString field = "CW LIV";
        LIVDataProcessor *livData = new LIVDataProcessor(field, collectedData[field].toMap(), "temperature", collectedData.value("pulsed_power_scale_liv", 100.0).toDouble());
        
        // Generate the LIV plot
        QString livOutputPath = graceFiguresDir + "/cw_liv.agr";
        livPlot.plot_liv(livOutputPath.toStdString(), livData, w, l);
		 // Create IthDataProcessor for Ith vs T plot (based on LIV data)
        IthDataProcessor *ithData = new IthDataProcessor(livData, 3.0, this);  // Passing LIV data and threshold
        QString ithOutputPath = graceFiguresDir + "/Ith_vs_T_cw_liv.agr";
        
        // Generate the Ith plot (Ith vs T)
		if (ithData->canPlot()) 
		{
			ithPlot.plot_Ith_vs_T(ithOutputPath.toStdString(), ithData, w, l);

			double A, B, C0;
			ithData->getExponentialFitParams(A, B, C0);

			collectedData["cw_I_exp_A"] = QString::number(A, 'f', 2);
			collectedData["cw_I_exp_B"] = QString::number(B, 'f', 2);
			collectedData["cw_I_exp_C0"] = QString::number(C0, 'f', 2);

			double scale = 1e5 / (w * l);

			collectedData["cw_J_exp_A"] = QString::number(A * scale, 'f', 2);
			collectedData["cw_J_exp_B"] = QString::number(B * scale, 'f', 2);
			collectedData["cw_J_exp_C0"] = QString::number(C0 * scale, 'f', 2);

			// Emit the signal with the updated collectedData
			emit dataProcessed(collectedData);
		}
		else 
			qDebug() << "Skipping Ith plot: insufficient valid traces";
		}

    // CW FTIR - fixed temperature
	if (collectedData.contains("CW FTIR - fixed temperature")) {
		QString field = "CW FTIR - fixed temperature";
		SpectraDataProcessor* spectraData = new SpectraDataProcessor(
			field,
			collectedData[field].toMap(),
			"current",
			collectedData.value("cw_fmin_spectra", 0.0).toDouble(),
			collectedData.value("cw_fmax_spectra", 0.0).toDouble()
		);        
		QString spectraOutputPath = graceFiguresDir + "/cw_ftir_vs_I.agr";
		spectraPlot.plot_spectra_waterfall(spectraOutputPath.toStdString(), spectraData);

		// Add frequency range string
		QString freqRange = spectraData->getGlobalFrequencyRangeString();
		if (!freqRange.isEmpty()) {
			collectedData["cw_ftir_fixed_temp_freq_range"] = freqRange;
		}
	}

    // Check for and handle data for "CW FTIR - fixed current"
    if (collectedData.contains("CW FTIR - fixed current")) {
        QString field = "CW FTIR - fixed current";
		SpectraDataProcessor* spectraData = new SpectraDataProcessor(
			field,
			collectedData[field].toMap(),
			"temperature",
			collectedData.value("cw_fmin_spectra", 0.0).toDouble(),
			collectedData.value("cw_fmax_spectra", 0.0).toDouble()
		);        
        // Generate the Spectra plot
        QString spectraOutputPath = graceFiguresDir + "/cw_ftir_vs_T.agr";
        spectraPlot.plot_spectra_waterfall(spectraOutputPath.toStdString(), spectraData);
    }
	
	// Convert all generated .agr plots to PDF
	FileConverter *converter = new FileConverter(this);
	connect(converter, &FileConverter::conversionFinished, this, [this]() 
	{
		loadGeneratedImagesFromFigures();
		generateDataSheetButton->setEnabled(true);
		generateDataSheetButton->setStyleSheet("background-color: #007AFF;");
	});

	converter->processAgrFilesToPsAndPdf(graceFiguresDir);
}

/**
void WizardGracePage::generateGraceImages()
{
	nothingToShowWidget->hide();  // Hide the "Nothing to Show" widget after generating Grace images
    generateDataSheetButton->setEnabled(true);  // Enable the "Generate Data Sheet" button
    generateDataSheetButton->setStyleSheet("background-color: #007AFF;");  // Reset the style when enabled

    if (collectedData.isEmpty())
        return;

    QVariantMap dimensions = collectedData.value("Dimensions").toMap();
    double w = dimensions.value("width").toDouble();
    double l = dimensions.value("length").toDouble();

    QString graceFiguresDir = outputDir + "/GraceFigures";
    QDir dir(graceFiguresDir);
    if (!dir.exists() && !dir.mkpath(graceFiguresDir)) {
        qWarning() << "Failed to create directory:" << graceFiguresDir;
        return;
    }

    LIVGracePlot livPlot;
    SpectraGracePlot spectraPlot;
    IthGracePlot ithPlot;

    auto tryAdd = [&](const QString &key, const QString &filename, const QString &xAxisType, bool isLIV) {
        if (!collectedData.contains(key))
            return;

        QString path = graceFiguresDir + "/" + filename;

        if (isLIV) {
            auto *data = new LIVDataProcessor(key, collectedData[key].toMap(), xAxisType, this);
            livPlot.plot_liv(path.toStdString(), data, w, l);

            auto *ithData = new IthDataProcessor(data, 3.0, this);
            QString ithPath = graceFiguresDir + "/Jth_vs_T_" + key.replace(" ", "_").toLower() + ".agr";
            ithPlot.plot_Ith_vs_T(ithPath.toStdString(), ithData, w, l);
        } else {
            auto *data = new SpectraDataProcessor(key, collectedData[key].toMap(), xAxisType, this);
            spectraPlot.plot_spectra_waterfall(path.toStdString(), data);
        }
    };

    tryAdd("Pulsed LIV",                     "pulsed_liv.agr",        "temperature", true);
    tryAdd("Pulsed FTIR - fixed temperature","pulsed_ftir_vs_I.agr",  "current",     false);
    tryAdd("Pulsed FTIR - fixed current",    "pulsed_ftir_vs_T.agr",  "temperature", false);
    tryAdd("CW LIV",                         "cw_liv.agr",            "temperature", true);
    tryAdd("CW FTIR - fixed temperature",    "cw_ftir_vs_I.agr",      "current",     false);
    tryAdd("CW FTIR - fixed current",        "cw_ftir_vs_T.agr",      "temperature", false);
	
	// Convert all generated .agr plots to PDF and JPEG
    FileConverter converter;
    converter.processAgrFilesToPsAndPdf(graceFiguresDir);
	loadGeneratedImagesFromFigures();
}
*/