/**
 * @file WizardStack.cpp
 * @brief Implementation of WizardStack managing the sequence of wizard pages for QCL characterization.
 * 
 * This class initializes and organizes various wizard pages for pulsed and continuous wave (CW) modes,
 * including measurement setup, file inputs, and visualization via the Grace page.
 * It manages page transitions and coordinates data flow between pages.
 * 
 * @author Aleksandar Demic
 */
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
#include "wizardstack.h"
#include "ui/dialogs/MessageBox.h"
#include "ui/components/wizard/WizardPage.h"
#include "ui/components/wizard/wizardpages/WizardFilePage.h"

/**
 * @brief Constructs the WizardStack, initializing and adding all wizard pages.
 * 
 * Sets up the sequence of pages including setup parameters, pulsed and CW measurement setups,
 * file input pages for LIV and FTIR data, and the Grace visualization page.
 * Connects signals and slots for page changes and data communication between pages.
 * 
 * @param parent The parent QWidget, defaulting to nullptr.
 */
WizardStack::WizardStack(QWidget *parent)
    : Wizard{parent}
{
    // SetupParamsPage
    setupParamsPage = new WizardSetupParamsPage();
    addPage(setupParamsPage);

    // Pulsed pages
    measurementPagePulsed = new WizardMeasurementSetupPage(true);
    addPage(measurementPagePulsed);

    auto *pulsedLIV = new WizardFilePage("Pulsed LIV", "Temperature (K)");
    pulsedLIV->setKeyBase("pulsed_liv");
    addPage(pulsedLIV);

    auto *pulsedFTIRTemp = new WizardFilePage("Pulsed FTIR - fixed temperature", "Current (mA)");
    pulsedFTIRTemp->setKeyBase("pulsed_spectra_t");
    addPage(pulsedFTIRTemp);

    auto *pulsedFTIRCurr = new WizardFilePage("Pulsed FTIR - fixed current", "Temperature (K)");
    pulsedFTIRCurr->setKeyBase("pulsed_spectra_i");
    addPage(pulsedFTIRCurr);

    // CW pages
    measurementPageCW = new WizardMeasurementSetupPage(false);
    addPage(measurementPageCW);

    auto *cwLIV = new WizardFilePage("CW LIV", "Temperature (K)");
    cwLIV->setKeyBase("cw_liv");
    addPage(cwLIV);

    auto *cwFTIRTemp = new WizardFilePage("CW FTIR - fixed temperature", "Current (mA)");
    cwFTIRTemp->setKeyBase("cw_spectra_t");
    addPage(cwFTIRTemp);

    auto *cwFTIRCurr = new WizardFilePage("CW FTIR - fixed current", "Temperature (K)");
    cwFTIRCurr->setKeyBase("cw_spectra_i");
    addPage(cwFTIRCurr);

    // Grace page
    gracePage = new WizardGracePage("Grace Figures");
    addPage(gracePage);

    connect(wizardPages, &QStackedWidget::currentChanged, this, &WizardStack::currentChangedSlot);

    // Connect to sendFields signal to setFields slot in GracePage
    connect(this, &WizardStack::sendFields, gracePage, &WizardGracePage::setFields);

    // Connect GracePage dataProcessed signal to WizardStack slot to receive updated data
    connect(gracePage, &WizardGracePage::dataProcessed,
            this, &WizardStack::updateCollectedDataFromGracePage);
}

/**
 * @brief Slot triggered when the current wizard page changes.
 * 
 * Flushes any active UI edits, collects data from various pages into a map,
 * validates the output directory, and emits the collected data for further processing.
 * Specifically handles the transition to the WizardGracePage by aggregating
 * parameters and file page data.
 * 
 * @param id The index of the newly selected page in the wizard stack.
 */
void WizardStack::currentChangedSlot(int id)
{
    qDebug() << "currentChangedSlot triggered for page id:" << id;

    // Flush edits in UI
    if (QWidget *focused = QApplication::focusWidget()) {
        focused->clearFocus();
    }
    qApp->processEvents();

    if (qobject_cast<WizardGracePage *>(wizardPages->widget(id))) {
        QVariantMap map;

        if (setupParamsPage) setupParamsPage->addToMap(map);
        if (measurementPagePulsed) measurementPagePulsed->addToMap(map);
        if (measurementPageCW) measurementPageCW->addToMap(map);

        // Add WizardFilePage data (already properly keyed)
        QList<WizardFilePage *> filePages = wizardPages->findChildren<WizardFilePage *>();
        for (WizardFilePage *filePage : filePages) {
            filePage->addToMap(map);  // Let the page define its own keys like "pulsed_liv_experimental_notes"
        }

        QString outputDir = setupParamsPage->getOutputDirectory();
        if (outputDir.isEmpty()) {
            QMessageBox::warning(this, "Error", "Output directory is not set.");
            return;
        }

        QDir dir(outputDir);
        if (!dir.exists()) {
            QMessageBox::warning(this, "Error", "Output directory does not exist.");
            return;
        }

        qDebug() << "setFields called with data keys:" << map.keys();
        qDebug() << "Output dir:" << dir.absolutePath();
		
		for (auto it = map.constBegin(); it != map.constEnd(); ++it) 
			collectedData[it.key()] = it.value();

        emit sendFields(map, outputDir);
    }
}

/**
 * @brief Finalizes the wizard process by collecting and saving all relevant data.
 * 
 * This function gathers data from various wizard pages, including setup parameters,
 * pulsed and continuous-wave measurement pages, and file-related pages.
 * It merges the collected data with any previously stored data, ensuring completeness.
 * 
 * Then, it constructs a sanitized filename based on the sample name, defaulting to "results" if none is provided.
 * The output directory is validated for existence before proceeding.
 * 
 * If a file with the intended name already exists in the output directory, the user is prompted
 * to confirm overwriting it.
 * 
 * The collected data is then asynchronously written to a text file with neatly formatted entries.
 * Special handling is provided for structured data such as dimensions and file path entries,
 * aligning output for readability.
 * 
 * Upon successful file write, a signal is emitted with the collected data and output directory.
 * If file writing fails, an error handler is invoked to display an appropriate message.
 * 
 * @note This operation runs asynchronously to avoid blocking the UI thread.
 */
void WizardStack::finishWizardAction()
{
    QVariantMap map;

    // Collect data from all pages as before
    if (setupParamsPage) setupParamsPage->addToMap(map);
    if (measurementPagePulsed) measurementPagePulsed->addToMap(map);
    if (measurementPageCW) measurementPageCW->addToMap(map);

    // Collect file page data (paths)
    QList<WizardFilePage *> filePages = wizardPages->findChildren<WizardFilePage *>();
    QVariantMap filePageMap;
    for (WizardFilePage *page : filePages) 
    {
        QVariantMap tmpMap;
        page->addToMap(tmpMap);
        for (auto it = tmpMap.constBegin(); it != tmpMap.constEnd(); ++it) 
            filePageMap[it.key()] = it.value();
    }

    // Merge collectedData
    for (auto it = collectedData.constBegin(); it != collectedData.constEnd(); ++it) {
        map[it.key()] = it.value();
    }

    // Prepare filename
    QString sampleName = map.value("Sample Name").toString();
    if (sampleName.isEmpty()) {
        sampleName = "results";
    } else {
        sampleName = sampleName.replace(QRegularExpression("[^a-zA-Z0-9_]"), "_");
    }
    QString defaultFileName = sampleName + "_DataMap.txt";

    QString outputDir = setupParamsPage ? setupParamsPage->getOutputDirectory() : "";

    if (outputDir.isEmpty()) {
        QMessageBox::warning(this, "Error", "Output directory is not set.");
        return;
    }

    QDir dir(outputDir);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Error", "Output directory does not exist.");
        return;
    }

    QString filePath = dir.filePath(defaultFileName);

    if (QFile::exists(filePath)) {
        int ret = QMessageBox::warning(this, "File Exists",
                                       "The file already exists. Do you want to overwrite it?",
                                       QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::No) return;
    }

	QtConcurrent::run([this, filePath, map, outputDir]() 
	{
		QFile file(filePath);
		if (file.open(QIODevice::WriteOnly)) {
			QTextStream out(&file);

			out << "\n=== Collected Data ===\n";

			// Compute max key length for alignment
			int maxKeyWidth = 0;
			for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
				if (it.key().length() > maxKeyWidth)
					maxKeyWidth = it.key().length();
			}

			// Column where paths start for special keys
			const int pathColumn = maxKeyWidth + 5;

			for (auto it = map.constBegin(); it != map.constEnd(); ++it) 
			{
				const QString &key = it.key();
				const QVariant &value = it.value();

				qDebug() << "KEY:" << key << "TYPE:" << value.typeName() << "VALUE:" << value;

				if (key == "Dimensions") 
				{
					QVariantMap dimensions = value.toMap();
					double w = dimensions.value("width").toDouble();
					double l = dimensions.value("length").toDouble();
					double h = dimensions.value("height").toDouble();

					out << QString("%1 = %2\n").arg("Width", maxKeyWidth).arg(w);
					out << QString("%1 = %2\n").arg("Length", maxKeyWidth).arg(l);
					out << QString("%1 = %2\n").arg("Height", maxKeyWidth).arg(h);

				} 
				else if (key == "Pulsed LIV" 
					  || key == "CW LIV" 
					  || key == "Pulsed FTIR - fixed temperature" 
					  || key == "Pulsed FTIR - fixed current"
					  || key == "CW FTIR - fixed temperature"
					  || key == "CW FTIR - fixed current") 
				{
					// Print key with '=' on its own line
					out << QString("%1 = \n").arg(key, maxKeyWidth);

					QVariantMap fileMap = value.toMap();

					// Find longest path for neat alignment
					int maxPathLen = 0;
					for (auto mit = fileMap.constBegin(); mit != fileMap.constEnd(); ++mit) 
						maxPathLen = std::max(maxPathLen, int(mit.key().length()));
					

					// Output each path and value aligned
					for (auto mit = fileMap.constBegin(); mit != fileMap.constEnd(); ++mit) {
						QString spacesBeforePath = QString(pathColumn, ' ');
						// Pad path to maxPathLen to align values
						QString pathPadded = mit.key().leftJustified(maxPathLen, ' ');
						out << spacesBeforePath << pathPadded << " " << mit.value().toString() << "\n";
					}
				} 
				else 
				{
					out << QString("%1 = %2\n").arg(key, maxKeyWidth).arg(value.toString());
				}
			}

			file.close();

			QMetaObject::invokeMethod(this, "sendFields", Qt::QueuedConnection,
									  Q_ARG(QVariantMap, map),
									  Q_ARG(QString, outputDir));
		} 
		else 
		{
			QMetaObject::invokeMethod(this, "handleFileError", Qt::QueuedConnection,
									  Q_ARG(QString, "Failed to open file for writing."));
		}
	});
}

/**
 * @brief Shows a warning message box displaying a file-related error.
 * 
 * @param errorMessage The error message text to display.
 */
void WizardStack::handleFileError(const QString &errorMessage)
{
    QMessageBox::warning(this, "File Error", errorMessage);
}

/**
 * @brief Updates the internal collected data map with new data.
 * 
 * Merges the entries from updatedData into the existing collectedData,
 * replacing existing keys or adding new ones.
 * 
 * @param updatedData Map containing updated key-value pairs to merge.
 */
void WizardStack::updateCollectedDataFromGracePage(const QVariantMap &updatedData)
{
    // Merge the updated data into WizardStack's collectedData map
    for (auto it = updatedData.constBegin(); it != updatedData.constEnd(); ++it) {
        collectedData[it.key()] = it.value();
    }
}
