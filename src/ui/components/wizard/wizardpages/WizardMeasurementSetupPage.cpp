/**
 * @file WizardMeasurementSetupPage.cpp
 * @brief Implementation of WizardMeasurementSetupPage for setting up LIV and Spectra measurements.
 * 
 * This class constructs the UI page for measurement setup in pulsed or CW mode.
 * It provides validated input fields and dropdowns for configuring measurement parameters,
 * including hardware selections and graph options.
 * 
 * The page is split into two columns: LIV Setup and Spectra Setup, each with relevant controls.
 * Input validation ensures proper ranges and formats.
 * 
 * @author Aleksandar Demic
 */
#include "WizardMeasurementSetupPage.h"
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QFrame>
#include <QMessageBox>

/**
 * @brief Constructs a WizardMeasurementSetupPage for configuring LIV and Spectra measurement setups.
 * 
 * Initializes the UI layout for either pulsed or continuous-wave (CW) measurement modes.
 * The page contains two main sections: LIV Setup and Spectra Setup, each with relevant input fields.
 * 
 * Features:
 * - Conditional display and validation of fields based on pulsed or CW mode.
 * - Validated line edits for numerical inputs with range and decimal precision constraints.
 * - Dropdown fields populated from data files for selecting hardware components (cryostat, detectors, power supply, etc.).
 * - Graph options fields for controlling plot parameters such as power scale and frequency ranges.
 * - UI layout organized in a two-column grid with spacing, margins, and separator lines.
 * - Automatic setting of default values after widget creation.
 * 
 * @param pulsedMode Boolean flag indicating if the setup is for pulsed (true) or CW (false) mode.
 * @param parent Optional parent widget.
 */
WizardMeasurementSetupPage::WizardMeasurementSetupPage(bool pulsedMode, QWidget *parent)
    : WizardPage(pulsedMode ? "Measurement Setup (pulsed)" : "Measurement Setup (CW)", parent), pulsed(pulsedMode)
{
    keyPrefix = pulsed ? "pulsed_" : "cw_";

    QGridLayout *layout = new QGridLayout();
    layout->setSpacing(12);

    // Add top margin to push everything down so it doesn't overlap page title
    layout->setContentsMargins(0, 100, 0, 0);  // 40 px top margin

    layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed), 0, 0, 1, 5);

    // Titles for two columns
    QLabel *livTitle = new QLabel("LIV Setup");
    QLabel *spectraTitle = new QLabel("Spectra Setup");

    QFont titleFont = livTitle->font();
    titleFont.setBold(true);
    titleFont.setPointSize(20);
    livTitle->setFont(titleFont);
    spectraTitle->setFont(titleFont);

    livTitle->setStyleSheet("font-size: 25px; font-weight: bold;");
    spectraTitle->setStyleSheet("font-size: 25px; font-weight: bold;");

    layout->addWidget(livTitle, 0, 0, 1, 2, Qt::AlignLeft);
    layout->addWidget(spectraTitle, 0, 3, 1, 2, Qt::AlignLeft);

    // Helper to add validated line edits with validator and duty cycle clamp
    auto addValidatedLineEditField = [&](const QString &key, const QString &label, const QString &placeholder,
                                         int row, int col,
                                         double min, double max, int decimals)
    {
        addLineEditField(layout, key, label, placeholder, row, col);
        auto edit = qobject_cast<QLineEdit*>(fieldWidgets[key]);
        if (edit) {
            auto val = new QDoubleValidator(min, max, decimals, this);
            val->setNotation(QDoubleValidator::StandardNotation);
            edit->setValidator(val);

            if (key.contains("duty_cycle")) {
                QObject::connect(edit, &QLineEdit::textChanged, this, [edit]() {
                    bool ok = false;
                    double v = edit->text().toDouble(&ok);
                    if (ok && v > 100.0) {
                        edit->setText("100.0");
                    }
                });
            }
        }
    };

    // ----- Left Column (LIV) -----
    int livRow = 1;
    if (pulsed) {
        addValidatedLineEditField(keyPrefix + "duty_cycle_liv", "Duty Cycle", "Enter duty cycle (0.0 - 100.0 %)", livRow++, 0, 0.0, 100.0, 2);
    }
    addDropdownField(layout, keyPrefix + "cryostat_liv", "Cryostat", "cryostats.dat", livRow++, 0);
    addDropdownField(layout, keyPrefix + "detector_liv", "Detector", "livdetectors.dat", livRow++, 0);
    addDropdownField(layout, keyPrefix + "ps_liv", "Power Supply", "powersupply.dat", livRow++, 0);

    if (pulsed) {
        addValidatedLineEditField(keyPrefix + "drive_freq_liv", "Drive Freq", "Current sampling frequency [kHz]", livRow++, 0, 0.0, 1e6, 2);
        addValidatedLineEditField(keyPrefix + "gate_freq_liv", "Gate Freq", "Square wave frequency [Hz] (167 default)", livRow++, 0, 0.0, 1e6, 2);
    }
    addValidatedLineEditField(keyPrefix + "tmax_liv", "Tmax", "Maximum temperature [K], if empty, will not show in data sheet", livRow++, 0, 0.0, 1e3, 2);

    // Horizontal line separating LIV main fields and graph options
    QFrame *hLineLiv = new QFrame();
    hLineLiv->setFrameShape(QFrame::HLine);
    hLineLiv->setFrameShadow(QFrame::Sunken);
    layout->addWidget(hLineLiv, livRow++, 0, 1, 2);

    // Graph Options label immediately below line
    QLabel *graphOptionsLiv = new QLabel("Graph Options");
    QFont sectionFont = graphOptionsLiv->font();
    sectionFont.setBold(true);
    graphOptionsLiv->setFont(sectionFont);
    layout->addWidget(graphOptionsLiv, livRow++, 0, 1, 2);

    // Graph options fields
    addValidatedLineEditField(keyPrefix + "power_scale_liv", "Power Scale", "Highest measured power [mW], if empty 100 a.u. will be used in LIVs", livRow++, 0, 0.0, 10000, 3);

    // ----- Vertical separator line -----
    QFrame *vLine = new QFrame();
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFrameShadow(QFrame::Sunken);
    // Make vLine span from title row to bottom of left column (livRow)
    layout->addWidget(vLine, 1, 2, livRow - 1, 1);

    // ----- Right Column (Spectra) -----
    int spectraRow = 1;
    if (pulsed) {
        addValidatedLineEditField(keyPrefix + "duty_cycle_spectra", "Duty Cycle", "Enter duty cycle (0.0 - 100.0 %)", spectraRow++, 3, 0.0, 100.0, 2);
    }
    addDropdownField(layout, keyPrefix + "cryostat_spectra", "Cryostat", "cryostats.dat", spectraRow++, 3);
    addDropdownField(layout, keyPrefix + "spectrometer_spectra", "Spectrometer", "spectrometers.dat", spectraRow++, 3);
    addDropdownField(layout, keyPrefix + "detector_spectra", "Detector", "spectradetectors.dat", spectraRow++, 3);
    addDropdownField(layout, keyPrefix + "ps_spectra", "Power Supply", "powersupply.dat", spectraRow++, 3);

    if (pulsed) {
        addValidatedLineEditField(keyPrefix + "gate_freq_spectra", "Gate Freq", "Square wave frequency [Hz]", spectraRow++, 3, 0.0, 1e6, 2);
    }

    // Horizontal line separating Spectra main fields and graph options
    QFrame *hLineSpectra = new QFrame();
    hLineSpectra->setFrameShape(QFrame::HLine);
    hLineSpectra->setFrameShadow(QFrame::Sunken);
    layout->addWidget(hLineSpectra, spectraRow++, 3, 1, 2);

    // Graph Options label immediately below line
    QLabel *graphOptionsSpectra = new QLabel("Graph Options");
    graphOptionsSpectra->setFont(sectionFont);
    layout->addWidget(graphOptionsSpectra, spectraRow++, 3, 1, 2);

    // Graph options fields
    addValidatedLineEditField(keyPrefix + "fmin_spectra", "Fmin", "Minimum frequency [THz]", spectraRow++, 3, 0.0, 300, 2);
    addValidatedLineEditField(keyPrefix + "fmax_spectra", "Fmax", "Maximum frequency [THz]", spectraRow++, 3, 0.0, 300, 2);

    addLineEditField(layout, keyPrefix + "tfix_spectra", "Tfix", "Fixed temperature [K] (20 default), value for spectra measured at different I levels and Tfix", spectraRow++, 3);
    {
        auto edit = qobject_cast<QLineEdit*>(fieldWidgets[keyPrefix + "tfix_spectra"]);
        if (edit) {
            auto val = new QDoubleValidator(-273.15, 1e3, 2, this);
            val->setNotation(QDoubleValidator::StandardNotation);
            edit->setValidator(val);
        }
    }

    addLineEditField(layout, keyPrefix + "ifix_spectra", "Ifix", "Fixed current [mA], leave empty if spectra not measured at different T levels and Ifix", spectraRow++, 3);
    {
        auto edit = qobject_cast<QLineEdit*>(fieldWidgets[keyPrefix + "ifix_spectra"]);
        if (edit) {
            auto val = new QDoubleValidator(0.0, 1e6, 3, this);
            val->setNotation(QDoubleValidator::StandardNotation);
            edit->setValidator(val);
        }
    }

    // Add vertical stretch at bottom so layout expands nicely when window grows
    layout->setRowStretch(std::max(livRow, spectraRow), 10);

    setLayout(layout);
	setDefaults();  // <-- call after all widgets are created
}

/**
 * @brief Adds a labeled QLineEdit input field to the specified grid layout.
 * 
 * Creates a QLabel and QLineEdit pair with the given label text and placeholder,
 * places them in the layout at the specified row and column,
 * and stores the QLineEdit widget in a map for later access.
 * 
 * @param layout Pointer to the QGridLayout where the widgets will be added.
 * @param key A unique key to associate with the input widget.
 * @param label Text for the QLabel describing the input.
 * @param placeholder Placeholder text shown inside the QLineEdit.
 * @param row Row number in the grid layout.
 * @param colPrefix Starting column number in the grid layout for the label.
 */
void WizardMeasurementSetupPage::addLineEditField(QGridLayout *layout, const QString &key,
                                           const QString &label, const QString &placeholder,
                                           int row, int colPrefix)
{
    QLabel *lbl = new QLabel(label);
    QLineEdit *edit = new QLineEdit();
    edit->setPlaceholderText(placeholder);
    layout->addWidget(lbl, row, colPrefix);
    layout->addWidget(edit, row, colPrefix + 1);
    fieldWidgets[key] = edit;
}

/**
 * @brief Validates input fields in the measurement setup page.
 * 
 * Checks that specified line edits contain valid numeric values within defined ranges.
 * Shows warning dialogs and sets focus to invalid fields.
 * Supports some fields being optionally empty.
 * 
 * @return true if all validations pass; false otherwise.
 */
bool WizardMeasurementSetupPage::validatePage()
{
    auto checkField = [&](const QString &key, const QString &errorMsg, double minVal = 0.0, double maxVal = 1e9, bool allowEmpty = false) -> bool {
        auto edit = qobject_cast<QLineEdit*>(fieldWidgets[key]);
        if (!edit) return true;

        QString text = edit->text().trimmed();

        if (text.isEmpty()) {
            if (allowEmpty) return true;
            QMessageBox::warning(this, "Input Error", errorMsg + " cannot be empty.");
            edit->setFocus();
            return false;
        }

        bool ok = false;
        double val = text.toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Input Error", errorMsg + " is not a valid number.");
            edit->setFocus();
            return false;
        }

        if (val < minVal || val > maxVal) {
            QMessageBox::warning(this, "Input Error", errorMsg + QString(" must be between %1 and %2.").arg(minVal).arg(maxVal));
            edit->setFocus();
            return false;
        }

        return true;
    };

    // Now explicitly set min and max for duty cycle fields
    if (!checkField(keyPrefix + "duty_cycle_liv", "Duty Cycle (LIV)", 0.0, 100.0)) return false;
    if (!checkField(keyPrefix + "drive_freq_liv", "Drive Frequency (LIV)", 0.0, 1e6)) return false;
    if (!checkField(keyPrefix + "gate_freq_liv", "Gate Frequency (LIV)", 0.0, 1e6)) return false;
    if (!checkField(keyPrefix + "tmax_liv", "Tmax (LIV)", 0.0, 1e3)) return false;
    if (!checkField(keyPrefix + "power_scale_liv", "Power Scale (LIV)", 0.0, 10000)) return false;

    if (!checkField(keyPrefix + "duty_cycle_spectra", "Duty Cycle (Spectra)", 0.0, 100.0)) return false;
    if (!checkField(keyPrefix + "gate_freq_spectra", "Gate Frequency (Spectra)", 0.0, 1e6)) return false;
    if (!checkField(keyPrefix + "fmin_spectra", "Fmin (Spectra)", 0.0, 300)) return false;
    if (!checkField(keyPrefix + "fmax_spectra", "Fmax (Spectra)", 0.0, 300)) return false;

    // Optional fields:
    if (!checkField(keyPrefix + "tfix_spectra", "Tfix (Spectra)", -273.15, 1e3, true)) return false;
    if (!checkField(keyPrefix + "ifix_spectra", "Ifix (Spectra)", 0.0, 1e6, true)) return false;

    return true;
}

/**
 * @brief Loads a list of resource strings from a text file.
 * 
 * Reads the specified file line-by-line from the equipment resources directory,
 * trims whitespace, and returns a list of non-empty lines.
 * 
 * @param filename Name of the resource file to load.
 * @return QStringList List of items loaded from the file.
 */
QStringList WizardMeasurementSetupPage::loadResourceList(const QString &filename)
{
    // Use Qt resource path, matching .qrc entries
    QString resourcePath = ":/src/resources/equipment/" + filename;
    QFile file(resourcePath);
    QStringList items;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty())
                items.append(line);
        }
        file.close();
    } else {
        qWarning() << "Could not open embedded resource file:" << resourcePath;
    }

    return items;
}

/**
 * @brief Adds a labeled dropdown (QComboBox) field to the specified grid layout.
 * 
 * Loads dropdown options from a resource file and adds both a QLabel and QComboBox
 * to the layout. Stores the combo box widget in the fieldWidgets map.
 * 
 * @param layout Pointer to the QGridLayout where widgets will be added.
 * @param key Unique key to associate with this dropdown widget.
 * @param label Text label for the dropdown.
 * @param resourceFile File name to load dropdown items from.
 * @param row Row number in the grid layout.
 * @param colPrefix Starting column number for the label.
 */
void WizardMeasurementSetupPage::addDropdownField(QGridLayout *layout, const QString &key, const QString &label, const QString &resourceFile, int row, int colPrefix)
{
    QLabel *lbl = new QLabel(label);
    QComboBox *combo = new QComboBox();
    combo->addItems(loadResourceList(resourceFile));
    layout->addWidget(lbl, row, colPrefix);
    layout->addWidget(combo, row, colPrefix + 1);
    fieldWidgets[key] = combo;
}

/**
 * @brief Populates the given QVariantMap with current widget values.
 * 
 * Copies values from line edits and combo boxes into the map,
 * applying default values for empty fields depending on mode (pulsed or CW).
 * 
 * @param map Reference to the QVariantMap to populate with key-value pairs.
 */
void WizardMeasurementSetupPage::addToMap(QVariantMap &map)
{
    // Define default values per mode, excluding tmax_liv as requested
    static const QMap<QString, QVariant> pulsedDefaults = {
        { "duty_cycle_liv", "5" },
        { "drive_freq_liv", "10" },
        { "gate_freq_liv", "167" },
        { "power_scale_liv", "100" },
		{ "duty_cycle_spectra", "5" },
        { "tfix_spectra", "20" },
        { "fmin_spectra", "0" },
        { "fmax_spectra", "0" }
    };

    static const QMap<QString, QVariant> cwDefaults = {
        { "power_scale_liv", "100" },
        { "tfix_spectra", "20" },
        { "fmin_spectra", "0" },
        { "fmax_spectra", "0" }
    };

    const auto &defaults = pulsed ? pulsedDefaults : cwDefaults;

    for (auto it = fieldWidgets.begin(); it != fieldWidgets.end(); ++it) {
        const QString &fullKey = it.key();

        // Strip prefix to compare with defaults keys
        QString baseKey = fullKey;
        if (fullKey.startsWith(keyPrefix)) {
            baseKey = fullKey.mid(keyPrefix.length());
        }

        if (auto *edit = qobject_cast<QLineEdit *>(it.value())) {
            QString text = edit->text().trimmed();

            if (text.isEmpty() && defaults.contains(baseKey)) {
                map[fullKey] = defaults[baseKey];
            } else {
                map[fullKey] = text;
            }
        } else if (auto *combo = qobject_cast<QComboBox *>(it.value())) {
            map[fullKey] = combo->currentText();
        }
    }
}

/**
 * @brief Sets placeholder text for default values on input fields.
 * 
 * Applies different default placeholder texts depending on whether
 * the page is in pulsed or CW mode.
 */
void WizardMeasurementSetupPage::setDefaults()
{
    if (!pulsed) {
        QLineEdit *pscale = qobject_cast<QLineEdit *>(fieldWidgets[keyPrefix + "power_scale_liv"]);
        if (pscale) pscale->setPlaceholderText("Enter max measured power in mW or default is 100 a.u.");

        QLineEdit *tmax = qobject_cast<QLineEdit *>(fieldWidgets[keyPrefix + "tmax_liv"]);
        if (tmax) tmax->setPlaceholderText("0 (if not set, will not print in the data sheet)");

    } else {
        QLineEdit *duty = qobject_cast<QLineEdit *>(fieldWidgets[keyPrefix + "duty_cycle_liv"]);
        if (duty) duty->setPlaceholderText("5 % (default)");

        QLineEdit *dutyspectra = qobject_cast<QLineEdit *>(fieldWidgets[keyPrefix + "duty_cycle_spectra"]);
        if (dutyspectra) dutyspectra->setPlaceholderText("5 % (default)");

        QLineEdit *pscale = qobject_cast<QLineEdit *>(fieldWidgets[keyPrefix + "power_scale_liv"]);
        if (pscale) pscale->setPlaceholderText("Enter max measured power in mW or default is 100 a.u.");

        QLineEdit *tmax = qobject_cast<QLineEdit *>(fieldWidgets[keyPrefix + "tmax_liv"]);
        if (tmax) tmax->setPlaceholderText("0 (if not set, will not print in the data sheet)");
    }
}