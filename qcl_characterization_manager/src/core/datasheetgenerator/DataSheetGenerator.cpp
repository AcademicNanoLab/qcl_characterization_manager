/**
 * \file        DataSheetGenerator.cpp
 * \brief       Generates LaTeX datasheet documents from measurement metadata and figures.
 * \author      Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "DataSheetGenerator.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>
#include <QDate>
#include <QDir>
#include <QRegularExpression>

/**
 * \brief Constructs a DataSheetGenerator instance.
 * \param outputPath The path to the output LaTeX file.
 * \param params A map of parameters (e.g., author, device name) used in the datasheet.
 */
DataSheetGenerator::DataSheetGenerator(const QString& outputPath, const QMap<QString, QVariant>& params)
    : outputPath(outputPath), params(params) {}

/**
 * \brief Sets the measurement metadata for pulsed LIV and continuous wave (CW) data.
 * \param pulsedKeys Metadata key-value pairs for pulsed LIV measurements (keys have "pulsed_" prefix).
 * \param cwKeys Metadata key-value pairs for CW measurements.
 */
void DataSheetGenerator::setMeasurementMetadata(const QMap<QString, QString>& pulsedKeys,
                                                const QMap<QString, QString>& cwKeys) 
{
    pulsedMetadata = pulsedKeys;	// Store pulsed LIV metadata as is (keys with "pulsed_" prefix)
    cwMetadata = cwKeys;			// Store pulsedSpectra metadata as is, no trimming
}

/**
 * \brief Assigns figure file names grouped by their respective sections.
 * \param figuresBySection Map where keys are section names and values are vectors of figure filenames.
 */
void DataSheetGenerator::setFigures(const QMap<QString, QVector<QString>>& figuresBySection) 
{
    figuresMap = figuresBySection;
}

/**
 * \brief Generates the complete LaTeX datasheet file.
 * 
 * Scans the Figures directory for available PDF files, opens the output
 * file, and writes the document by sequentially calling the header,
 * performance summary, pulsed and CW sections, and footer generation functions.
 * 
 * If the output file cannot be opened, logs a warning and aborts.
 */
void DataSheetGenerator::generate() 
{
	QDir figuresDir(QFileInfo(outputPath).absolutePath() + "/Figures");
	QStringList figureFiles = figuresDir.entryList(QStringList() << "*.pdf", QDir::Files);

	availableFigures.clear();
	for (const QString& file : figureFiles)
		availableFigures.insert(file);

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
	{
        qWarning("Failed to open LaTeX file for writing.");
        return;
    }

    QTextStream out(&file);
    out << generateHeader();
	out << generatePerformanceSummary();
    out << generatePulsedSection();
	out << generateCWSection();  
    out << generateFooter();
    file.close();
}

/**
 * \brief Generates the LaTeX document header with title, author, and date.
 * 
 * Uses parameters such as "Author", "Date", "Device Name", and "Sample Name"
 * to construct the title and metadata for the document.
 * 
 * \return A QString containing the LaTeX header code.
 */
QString DataSheetGenerator::generateHeader() const 
{
    QString author = params.value("Author", "Unknown Author").toString();
    QString date = params.value("Date", QDate::currentDate().toString("dd-MM-yyyy")).toString();
    QString deviceName = params.value("Device Name", "Unnamed Device").toString();
    QString sampleName = params.value("Sample Name", "Unnamed Sample").toString();
    QString title = QString("Datasheet: device %2 -- %1").arg(deviceName, sampleName);

    return QString(R"(
        \documentclass[12pt]{article}
        \usepackage[utf8]{inputenc}
        \usepackage{graphicx}
        \usepackage{geometry}
        \usepackage{tabularx}
        \usepackage{subcaption}
        \usepackage{caption}
        \usepackage{amsmath}
		\newcolumntype{Y}{>{\raggedright\arraybackslash}p{0.6\textwidth}}  
        \geometry{margin=2cm}

        \begin{document}
        \thispagestyle{empty}

        \vspace*{4cm}

        \begin{center}
            {\Huge \textbf{%1}} \\[1ex]
            {\small \textit{Characterised by: %2}} \\[2ex]
            {\large %3}
        \end{center}

        \vspace{5cm}  

    )").arg(escapeLatex(title), escapeLatex(author), escapeLatex(date));
}

/**
 * \brief Generates the LaTeX document footer.
 * 
 * \return A QString containing the closing command for the LaTeX document.
 */
QString DataSheetGenerator::generateFooter() const 
{
    return "\n\\end{document}\n";
}

/**
 * \brief Generates the performance summary section of the LaTeX datasheet.
 * 
 * Extracts and formats metadata parameters such as author, date, device dimensions,
 * threshold current densities, peak output powers, and maximum operating temperatures
 * for both pulsed and continuous wave (c.w.) measurements.
 * 
 * Reads formula data from Grace (.agr) files to parse threshold current density.
 * Formats the data into a LaTeX tabularx environment suitable for inclusion in the report.
 * 
 * \return A QString containing the formatted LaTeX code for the performance summary section.
 */
QString DataSheetGenerator::generatePerformanceSummary() const
{
    // Debug prints for all metadata maps
    qDebug() << "--- Params ---";
    for (auto it = params.begin(); it != params.end(); ++it) {
        qDebug() << it.key() << ":" << it.value().toString();
    }

    qDebug() << "--- CW Metadata ---";
    for (auto it = cwMetadata.begin(); it != cwMetadata.end(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }

    qDebug() << "--- Pulsed Metadata ---";
    for (auto it = pulsedMetadata.begin(); it != pulsedMetadata.end(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }

    auto getParam = [&](const QString& key, const QString& def = "N/A") -> QString {
        QString val = params.value(key).toString().trimmed();
        return val.isEmpty() ? def : escapeLatex(val);
    };

    auto getCW = [&](const QString& key, const QString& def = "N/A") -> QString {
        QString val = cwMetadata.value(key).trimmed();
        return val.isEmpty() ? def : escapeLatex(val);
    };

    auto getPulsed = [&](const QString& key, const QString& def = "N/A") -> QString {
        QString val = pulsedMetadata.value(key).trimmed();
        if (val.isEmpty()) {
            if (key == "pulsed_duty_cycle_liv") return "5";  // default duty cycle 5%
            return def;
        }
        return escapeLatex(val);
    };

    QString author = getParam("Author", "Unknown Author");
    QString date = getParam("Date", QDate::currentDate().toString("dd-MM-yyyy"));

    // Extract dimensions from params
    QVariantMap dimensions = params.value("Dimensions").toMap();
    double length = dimensions.value("length").toDouble();
    double width = dimensions.value("width").toDouble();
    double height = dimensions.value("height").toDouble();

    // Format dimensions string with units wrapped in math mode
    QString ridgeDimensions = QString("$%1~\\mathrm{mm}~\\times~%2~\\mathrm{\\mu m}~\\times~%3~\\mathrm{\\mu m}$")
                                .arg(length)
                                .arg(width)
                                .arg(height);

    QString emissionRange = "";  // intentionally left blank

    // Determine base directory (latex root) from outputPath
    QString latexRootDir = QFileInfo(outputPath).absolutePath();
    QDir graceFiguresDir(latexRootDir + "/GraceFigures");

    // Parse threshold current density formulas
    QString pulsedFormulaPath = graceFiguresDir.filePath("Ith_vs_T_pulsed_liv.agr");
    QString pulsedFormula = parseIthFormulaFromAgrFile(pulsedFormulaPath);
    QString pulsedJth;
    if (!pulsedFormula.isEmpty()) {
        QString jthFormula = pulsedFormula.section(',', 1).trimmed();
        QString jthRaw = jthFormula.section('+', 0, 0).trimmed();  // extract A parameter
        pulsedJth = jthRaw + "~\\mathrm{A/cm^2}~(20~\\mathrm{K})";
    }

    QString cwFormulaPath = graceFiguresDir.filePath("Ith_vs_T_cw_liv.agr");
    QString cwFormula = parseIthFormulaFromAgrFile(cwFormulaPath);
    QString cwJth;
    if (!cwFormula.isEmpty()) {
        QString jthFormula = cwFormula.section(',', 1).trimmed();
        QString jthRaw = jthFormula.section('+', 0, 0).trimmed();
        cwJth = jthRaw + "~\\mathrm{A/cm^2}~(20~\\mathrm{K})";
    }

    QString pulsedPower = getPulsed("pulsed_power_scale_liv");
    QString cwPower = getCW("cw_power_scale_liv");
    QString pulsedDuty = getPulsed("pulsed_duty_cycle_liv");  // default 5% handled above
	QString pulsedFreqRange = params.value("pulsed_ftir_fixed_temp_freq_range").toString().trimmed();
    QString pulsedTmax = getPulsed("pulsed_tmax_liv");
	QString cwFreqRange = params.value("cw_ftir_fixed_temp_freq_range").toString().trimmed();
    QString cwTmax = getCW("cw_tmax_liv");
	// Get frequency ranges if available
	

    QString section = "\\section*{Performance Summary}\n";
	section += "\\begin{tabularx}{\\textwidth}{|Y|X|}\n\\hline\n";

    section += "\\textbf{Characterised by:} & " + author + " \\\\\n\\hline\n";
    section += "\\textbf{Date of completion:} & " + date + " \\\\\n\\hline\n";

    if (!ridgeDimensions.isEmpty())
        section += "\\textbf{Ridge dimensions:} & " + ridgeDimensions + " \\\\\n\\hline\n";

    if (!emissionRange.isEmpty())
        section += "\\textbf{Emission frequency range:} & " + emissionRange + " \\\\\n\\hline\n";

    if (!pulsedJth.isEmpty())
        section += "\\textbf{Threshold current density (pulsed):} & $" + pulsedJth + "$ \\\\\n\\hline\n";
	
    if (!pulsedPower.isEmpty())
        section += "\\textbf{Peak output power (pulsed):} & $" + pulsedPower +
                   "~\\mathrm{mW}~(" + pulsedDuty + "\\%~\\mathrm{d.c.},~20~\\mathrm{K})$ \\\\\n\\hline\n";
	if (!pulsedFreqRange.isEmpty()) 
		section += "\\textbf{Emission frequency range (pulsed):} & " + pulsedFreqRange + " \\\\\n\\hline\n";

    if (!pulsedTmax.isEmpty())
        section += "\\textbf{Maximum operating temperature (pulsed):} & $" + pulsedTmax +
                   "~\\mathrm{K}~(" + pulsedDuty + "\\%~\\mathrm{d.c.})$ \\\\\n\\hline\n";

    if (!cwJth.isEmpty())
        section += "\\textbf{Threshold current density (c.w.):} & $" + cwJth + "$ \\\\\n\\hline\n";

    if (!cwPower.isEmpty())
        section += "\\textbf{Peak output power (c.w.):} & $" + cwPower +
                   "~\\mathrm{mW}~(20~\\mathrm{K})$ \\\\\n\\hline\n";

	if (!cwFreqRange.isEmpty()) 
		section += "\\textbf{Emission frequency range (c.w.):} & " + cwFreqRange + " \\\\\n\\hline\n";
	
    if (!cwTmax.isEmpty())
        section += "\\textbf{Maximum operating temperature (c.w.):} & $" + cwTmax +
                   "~\\mathrm{K}$ \\\\\n\\hline\n";

    section += "\\end{tabularx}\n\\clearpage\n";

    return section;
}

/**
 * \brief Generates the full pulsed characteristics section of the datasheet.
 * 
 * Combines the L-I-V characteristics subsection and the pulsed spectra subsection,
 * only including them if corresponding data exists.
 * 
 * \return LaTeX formatted QString for the pulsed characteristics section, or
 *         an empty string if no pulsed data is available.
 */
QString DataSheetGenerator::generatePulsedSection() const 
{
    QString liv = generatePulsedLIVSubsection();
    QString spectra = generatePulsedSpectraSubsection();

    if (liv.isEmpty() && spectra.isEmpty()) {
        return "";  // No pulsed data, so no section header or content
    }

    QString section = "\\section*{Pulsed Characteristics}\n";

    if (!liv.isEmpty())
        section += liv;

    if (!spectra.isEmpty())
        section += spectra;

    return section;
}

/**
 * \brief Generates the L-I-V characteristics subsection for the pulsed data.
 * 
 * Checks availability of required figures and data, constructs a subsection including
 * the L-I-V data table, combined figures, and any experimental notes.
 * 
 * Adds a LaTeX clearpage command at the end to ensure proper page breaks.
 * 
 * \return LaTeX formatted QString containing the L-I-V characteristics subsection,
 *         or an empty string if no relevant data is found.
 */
QString DataSheetGenerator::generatePulsedLIVSubsection() const 
{
    bool hasLIV = availableFigures.contains("pulsed_liv.pdf");
    bool hasIth = availableFigures.contains("Ith_vs_T_pulsed_liv.pdf");

    if (!hasLIV && !hasIth)
        return "";

    QString result = "\\subsection*{L-I-V Characteristics}\n";
    result += generatePulsedLIVTable();

    if (hasLIV || hasIth)
        result += generateCombinedLIVFigures(hasLIV, hasIth);

    // Fetch experimental notes from pulsedMetadata map
    QString notes = pulsedMetadata.value("pulsed_liv_experimental_notes").trimmed();

    if (!notes.isEmpty()) {
        result += generateExperimentalNotes(notes);
    }
    result += "\\clearpage\n";  // <-- add clearpage here

    return result;
}

/**
 * \brief Generates the pulsed spectra characteristics subsection.
 * 
 * Checks for availability of pulsed FTIR vs current and temperature figures,
 * constructs a subsection including the spectra table, combined FTIR figures,
 * and experimental notes if present.
 * 
 * Adds a LaTeX clearpage command at the end for proper page layout.
 * 
 * \return LaTeX formatted QString of the pulsed spectra subsection,
 *         or an empty string if no relevant data is available.
 */
QString DataSheetGenerator::generatePulsedSpectraSubsection() const 
{
    bool hasVsI = availableFigures.contains("pulsed_ftir_vs_I.pdf");
    bool hasVsT = availableFigures.contains("pulsed_ftir_vs_T.pdf");

    if (!hasVsI && !hasVsT)
        return "";

    QString result = "\\subsection*{Spectra Characteristics}\n";
    result += generatePulsedSpectraTable();

    if (hasVsI || hasVsT)
        result += generateCombinedFTIRFigures(hasVsI, hasVsT);

    // Fetch notes from pulsedMetadata map
    QString notesT = pulsedMetadata.value("pulsed_spectra_t_experimental_notes").trimmed();
    QString notesI = pulsedMetadata.value("pulsed_spectra_i_experimental_notes").trimmed();

    QString combinedNotes;
	if (!notesT.isEmpty() && !notesI.isEmpty()) {
		combinedNotes = "a) " + notesT + "\n\n\\noindent b) " + notesI;  // <-- add \noindent here
	} else {
		if (!notesT.isEmpty())
			combinedNotes = notesT;
		if (!notesI.isEmpty()) {
			if (!combinedNotes.isEmpty())
				combinedNotes += "\n\n";
			combinedNotes += notesI;
		}
	}


    if (!combinedNotes.isEmpty()) {
        result += generateExperimentalNotes(combinedNotes);
    }
	
    result += "\\clearpage\n";  // <-- add clearpage here

    return result;
}

/**
 * \brief Generates the pulsed L-I-V characteristics data table in LaTeX format.
 * 
 * Extracts key pulsed measurement metadata such as cryostat, detector, power supply,
 * drive frequency, duty cycle, gate frequency, power scale, and maximum temperature.
 * Missing values are replaced with defaults or "N/A".
 * 
 * \return LaTeX formatted QString representing a tabularx table of pulsed L-I-V parameters.
 */
QString DataSheetGenerator::generatePulsedLIVTable() const 
{
    auto get = [&](const QString& key, const QString& def = "N/A") -> QString {
        QString val = pulsedMetadata.value(key).trimmed();
        return val.isEmpty() ? def : escapeLatex(val);
    };

    QString maxTemp = get("pulsed_tmax_liv", "N/A");

    QString table = QString(R"(
\begin{tabularx}{\textwidth}{|X|X|}
\hline
\textbf{Cryostat:} & %1 \\
\hline
\textbf{Detector:} & %2 \\
\hline
\textbf{Power Supply:} & %3 \\
\hline
\textbf{Drive Frequency:} & %4 kHz \\
\hline
\textbf{Duty Cycle:} & %5 \\
\hline
\textbf{Gate Frequency:} & %6 Hz \\
\hline
\textbf{Power Scale:} & %7 mW \\
\hline
)").arg(
        get("pulsed_cryostat_liv"),
        get("pulsed_detector_liv"),
        get("pulsed_ps_liv"),
        get("pulsed_drive_freq_liv", "10"),
        get("pulsed_duty_cycle_liv", "5"),
        get("pulsed_gate_freq_liv", "167"),
        get("pulsed_power_scale_liv", "100")
    );

    // Append Max Temperature row only if valid
    if (maxTemp != "N/A" && !maxTemp.isEmpty()) {
        table += QString("\\textbf{Max Temperature:} & %1 K \\\\ \n\\hline\n").arg(maxTemp);
    }

    table += "\\end{tabularx}\n\\vspace{0.5cm}\n";

    return table;
}

/**
 * \brief Generates the pulsed spectra metadata table in LaTeX format.
 * 
 * Extracts pulsed measurement metadata related to spectra: cryostat, detector,
 * spectrometer, power supply, drive frequency, duty cycle, and gate frequency.
 * Missing values are replaced with defaults or "N/A".
 * 
 * \return LaTeX formatted QString representing a tabularx table of pulsed spectra parameters.
 */
QString DataSheetGenerator::generatePulsedSpectraTable() const 
{
    auto get = [&](const QString& key, const QString& def = "N/A") -> QString {
        QString val = pulsedMetadata.value(key).trimmed();
        return val.isEmpty() ? def : escapeLatex(val);
    };

    QString table = QString(R"(
\begin{tabularx}{\textwidth}{|X|X|}
\hline
\textbf{Cryostat:} & %1 \\
\hline
\textbf{Detector:} & %2 \\
\hline
\textbf{Spectrometer:} & %3 \\
\hline
\textbf{Power Supply:} & %4 \\
\hline
\textbf{Drive Frequency:} & %5 kHz \\
\hline
\textbf{Duty Cycle:} & %6 \\
\hline
\textbf{Gate Frequency:} & %7 Hz \\
\hline
\end{tabularx}
\vspace{0.5cm}
)").arg(
        get("pulsed_cryostat_spectra"),
        get("pulsed_detector_spectra"),
        get("pulsed_spectrometer_spectra"),
        get("pulsed_ps_spectra"),
        get("pulsed_drive_freq_spectra", "10"),
        get("pulsed_duty_cycle_spectra", "5"),
        get("pulsed_gate_freq_spectra", "167")
    );

    return table;
}

/**
 * \brief Generates combined L-I-V and threshold current figures in LaTeX format.
 * 
 * Depending on the availability of pulsed L-I-V and threshold current figures,
 * generates LaTeX figure environments to include these images side-by-side or individually.
 * Captions are dynamically constructed from metadata including drive frequency, duty cycle,
 * gate frequency, and threshold current fit formulas.
 * 
 * \param hasLIV Indicates if pulsed L-I-V figure is available.
 * \param hasIth Indicates if pulsed threshold current figure is available.
 * 
 * \return LaTeX formatted QString containing figure environments with appropriate captions,
 *         or an empty string if no figures are available.
 */
QString DataSheetGenerator::generateCombinedLIVFigures(bool hasLIV, bool hasIth) const
{
    auto getOrDefault = [&](const QString& key, const QString& def) -> QString {
        QString val = pulsedMetadata.value(key).trimmed();
        return val.isEmpty() ? def : val;
    };

    QString freq = trimTrailingBackslashes(escapeLatex(getOrDefault("pulsed_drive_freq_liv", "10")));
    QString duty = trimTrailingBackslashes(escapeLatex(getOrDefault("pulsed_duty_cycle_liv", "5")));
    QString gate = trimTrailingBackslashes(escapeLatex(getOrDefault("pulsed_gate_freq_liv", "167")));

    QString livCaption = QString(
        "Pulsed L-I-V characteristics driven by %1\\,kHz, %2\\% duty cycle pulses gated by a %3\\,Hz square-wave."
    ).arg(freq, duty, gate);

    QString latexRootDir = QFileInfo(outputPath).absolutePath();
    QString formulaPath = QDir(latexRootDir + "/GraceFigures").filePath("Ith_vs_T_pulsed_liv.agr");
    QString formula = parseIthFormulaFromAgrFile(formulaPath);

    auto buildIthCaption = [&](const QString& formulaText) -> QString {
        if (!formulaText.isEmpty()) {
            QString ith = formulaText.section(',', 0, 0).trimmed();
            QString jth = formulaText.section(',', 1).trimmed();
            return QString(
                "Threshold current vs. temperature, fitted to "
                "\\(I_{\\mathrm{th}}(T) = %1\\), corresponding to current density "
                "\\(J_{\\mathrm{th}}(T) = %2\\)."
            ).arg(ith, jth);
        } else {
            return "Threshold current vs. temperature.";
        }
    };

    if (hasLIV && hasIth) {
        // Combine detailed livCaption + Ith caption for main figure caption
        QString combinedCaption = QString("%1 (b) %2").arg(livCaption, buildIthCaption(formula));
        return QString(
            "\\begin{figure}[h!]\n"
            "\\centering\n"
            "\\begin{subfigure}{0.48\\textwidth}\n"
            "    \\includegraphics[width=\\linewidth]{Figures/pulsed_liv.pdf}\n"
            "    \\caption{\\small Pulsed LIV characteristics}\n"
            "\\end{subfigure}\n"
            "\\hfill\n"
            "\\begin{subfigure}{0.48\\textwidth}\n"
            "    \\includegraphics[width=\\linewidth]{Figures/Ith_vs_T_pulsed_liv.pdf}\n"
            "    \\caption{\\small Pulsed threshold current}\n"
            "\\end{subfigure}\n"
            "\\caption{\\small %1}\n"
            "\\end{figure}\n"
        ).arg(combinedCaption);
    }

    if (hasLIV) {
        return QString(
            "\\begin{figure}[h!]\n"
            "\\centering\n"
            "\\includegraphics[width=0.7\\textwidth]{Figures/pulsed_liv.pdf}\n"
            "\\caption{\\small %1}\n"
            "\\end{figure}\n"
        ).arg(livCaption);
    }

    if (hasIth) {
        QString ithCaption = buildIthCaption(formula);
        return QString(
            "\\begin{figure}[h!]\n"
            "\\centering\n"
            "\\includegraphics[width=0.7\\textwidth]{Figures/Ith_vs_T_pulsed_liv.pdf}\n"
            "\\caption{\\small %1}\n"
            "\\end{figure}\n"
        ).arg(ithCaption);
    }

    return "";
}

/**
 * @brief Generates LaTeX code for the combined FTIR emission spectra figures.
 *
 * Creates figures showing pulsed FTIR emission spectra driven by specific pulse parameters.
 * Supports showing spectra vs. current, spectra vs. temperature, or both side-by-side.
 *
 * @param hasVsI Indicates if spectra versus current data is available.
 * @param hasVsT Indicates if spectra versus temperature data is available.
 * @return QString LaTeX formatted figure environment(s) including captions and subfigures.
 */
QString DataSheetGenerator::generateCombinedFTIRFigures(bool hasVsI, bool hasVsT) const {
    auto getOrDefault = [&](const QString& key, const QString& def) -> QString {
        QString val = pulsedMetadata.value(key).trimmed();
        return val.isEmpty() ? def : val;
    };

    QString tfix = pulsedMetadata.value("tfix_spectra", "20").trimmed();
    QString ifix = pulsedMetadata.value("ifix_spectra", "").trimmed();

    QString freq = trimTrailingBackslashes(escapeLatex(getOrDefault("pulsed_drive_freq_spectra", "10")));
    QString duty = trimTrailingBackslashes(escapeLatex(getOrDefault("pulsed_duty_cycle_spectra", "5")));
    QString gate = trimTrailingBackslashes(escapeLatex(getOrDefault("pulsed_gate_freq_spectra", "167")));

    QString mainCaption = QString(
        "Pulsed FTIR emission spectra driven by %1\\,kHz, %2\\% duty cycle pulses gated by a %3\\,Hz square-wave."
    ).arg(freq, duty, gate);

    QString spectraVsTCaption = ifix.isEmpty() 
        ? "Spectra at different temperatures." 
        : QString("Spectra at different temperatures (at I = %1 mA).").arg(ifix);

    if (hasVsI && hasVsT) {
        return QString(R"(
\begin{figure}[h!]
\centering
\begin{subfigure}{0.48\textwidth}
    \includegraphics[width=\linewidth]{Figures/pulsed_ftir_vs_I.pdf}
    \caption{\small Spectra at different currents (at T = %1 K).}
\end{subfigure}
\hfill
\begin{subfigure}{0.48\textwidth}
    \includegraphics[width=\linewidth]{Figures/pulsed_ftir_vs_T.pdf}
    \caption{\small %2}
\end{subfigure}
\caption{\small %3}
\end{figure}
)").arg(tfix, spectraVsTCaption, mainCaption);
    } 
    else if (hasVsI) {
        return QString(R"(
\begin{figure}[h!]
\centering
\includegraphics[width=0.7\textwidth]{Figures/pulsed_ftir_vs_I.pdf}
\caption{\small %1 Spectra at different currents (at T = %2 K).}
\end{figure}
)").arg(mainCaption, tfix);
    } 
    else if (hasVsT) {
        return QString(R"(
\begin{figure}[h!]
\centering
\includegraphics[width=0.7\textwidth]{Figures/pulsed_ftir_vs_T.pdf}
\caption{\small %1 %2}
\end{figure}
)").arg(mainCaption, spectraVsTCaption);
    }

    return "";
}
 
 /**
 * @brief Generates the Continuous Wave (CW) Characteristics section in LaTeX.
 *
 * Combines subsections for L-I-V characteristics and spectral data if available.
 * Returns an empty string if no CW data exists.
 *
 * @return QString LaTeX formatted CW section content including section header and subsections.
 */
QString DataSheetGenerator::generateCWSection() const {
    QString liv = generateCWLIVSubsection();
    QString spectra = generateCWSpectraSubsection();

    if (liv.isEmpty() && spectra.isEmpty()) {
        return "";  // No CW data, so no section header or content
    }

    QString section = "\\section*{CW Characteristics}\n";

    if (!liv.isEmpty())
        section += liv + "\\clearpage\n";

    if (!spectra.isEmpty())
        section += spectra;

    return section;
}

/**
 * @brief Generates the L-I-V characteristics subsection for CW operation.
 *
 * Checks availability of CW LIV and threshold current figures, generates tables,
 * figures, and appends any experimental notes.
 *
 * @return QString LaTeX formatted subsection content including tables, figures, and notes.
 * Returns empty string if no relevant figures are found.
 */
QString DataSheetGenerator::generateCWLIVSubsection() const {
    bool hasLIV = availableFigures.contains("cw_liv.pdf");
    bool hasIth = availableFigures.contains("Ith_vs_T_cw_liv.pdf");

    if (!hasLIV && !hasIth)
        return "";

    QString result = "\\subsection*{L-I-V Characteristics}\n";
    result += generateCWLIVTable();

    if (hasLIV || hasIth)
        result += generateCombinedCWLIVFigures(hasLIV, hasIth); // Reuse same function if it's general

    QString notes = cwMetadata.value("cw_liv_experimental_notes").trimmed();

    if (!notes.isEmpty()) {
        result += generateExperimentalNotes(notes);
    }
    result += "\\clearpage\n";  // <-- add clearpage here

    return result;
}

/**
 * @brief Generates the Spectra Characteristics subsection for CW operation.
 *
 * Checks for the availability of CW FTIR spectra figures versus current and temperature.
 * If data is available, generates a spectra table and combines FTIR figures.
 * Also appends any relevant experimental notes with formatting.
 *
 * @return QString LaTeX formatted subsection content including table, figures, notes, and page break.
 * Returns an empty string if no spectra figures are found.
 */
QString DataSheetGenerator::generateCWSpectraSubsection() const {
    bool hasVsI = availableFigures.contains("cw_ftir_vs_I.pdf");
    bool hasVsT = availableFigures.contains("cw_ftir_vs_T.pdf");

    if (!hasVsI && !hasVsT)
        return "";

    QString result = "\\subsection*{Spectra Characteristics}\n";
    result += generateCWSpectraTable();

    if (hasVsI || hasVsT)
        result += generateCombinedCWFTIRFigures(hasVsI, hasVsT); // Reuse if it works for CW

    // Fetch notes from cwMetadata map
    QString notesT = cwMetadata.value("cw_spectra_t_experimental_notes").trimmed();
    QString notesI = cwMetadata.value("cw_spectra_i_experimental_notes").trimmed();

	QString combinedNotes;
	if (!notesT.isEmpty() && !notesI.isEmpty()) {
		combinedNotes = "a) " + notesT + "\n\n\\noindent b) " + notesI;  // <-- add \noindent here
	} else {
		if (!notesT.isEmpty())
			combinedNotes = notesT;
		if (!notesI.isEmpty()) {
			if (!combinedNotes.isEmpty())
				combinedNotes += "\n\n";
			combinedNotes += notesI;
		}
	}

    if (!combinedNotes.isEmpty()) {
        result += generateExperimentalNotes(combinedNotes);
    }
    result += "\\clearpage\n";  

    return result;
}

/**
 * @brief Generates a LaTeX formatted table summarizing the CW L-I-V experimental setup.
 *
 * Retrieves metadata such as cryostat, detector, power supply, and power scale,
 * and includes maximum temperature if available.
 *
 * @return QString LaTeX table code representing CW L-I-V experimental parameters.
 */
QString DataSheetGenerator::generateCWLIVTable() const {
    auto get = [&](const QString& key, const QString& def = "N/A") -> QString {
        QString val = cwMetadata.value(key).trimmed();
        return val.isEmpty() ? def : escapeLatex(val);
    };

    QString maxTemp = get("cw_tmax_liv", "N/A");

    QString table = QString(R"(
\begin{tabularx}{\textwidth}{|X|X|}
\hline
\textbf{Cryostat:} & %1 \\
\hline
\textbf{Detector:} & %2 \\
\hline
\textbf{Power Supply:} & %3 \\
\hline
\textbf{Power Scale:} & %4 mW \\
\hline
)").arg(
        get("cw_cryostat_liv"),
        get("cw_detector_liv"),
        get("cw_ps_liv"),
        get("cw_power_scale_liv", "100")
    );

    if (maxTemp != "N/A" && !maxTemp.isEmpty()) {
        table += QString("\\textbf{Max Temperature:} & %1 K \\\\ \n\\hline\n").arg(maxTemp);
    }

    table += "\\end{tabularx}\n\\vspace{0.5cm}\n";

    return table;
}

/**
 * @brief Generates a LaTeX formatted table summarizing the CW spectral measurement setup.
 * 
 * Retrieves and formats metadata such as cryostat, detector, spectrometer, and power supply
 * used during CW spectral measurements.
 * 
 * @return QString LaTeX table code representing the CW spectra experimental parameters.
 */
QString DataSheetGenerator::generateCWSpectraTable() const {
    auto get = [&](const QString& key, const QString& def = "N/A") -> QString {
        QString val = cwMetadata.value(key).trimmed();
        return val.isEmpty() ? def : escapeLatex(val);
    };

    QString table = QString(R"(
\begin{tabularx}{\textwidth}{|X|X|}
\hline
\textbf{Cryostat:} & %1 \\
\hline
\textbf{Detector:} & %2 \\
\hline
\textbf{Spectrometer:} & %3 \\
\hline
\textbf{Power Supply:} & %4 \\
\hline
\end{tabularx}
\vspace{0.5cm}
)").arg(
        get("cw_cryostat_spectra"),
        get("cw_detector_spectra"),
        get("cw_spectrometer_spectra"),
        get("cw_ps_spectra")
    );

    return table;
}

/**
 * @brief Generates LaTeX figure environments combining CW L-I-V characteristics and threshold current plots.
 * 
 * If both L-I-V and Ith figures exist, places them side-by-side with a combined caption.
 * If only one exists, generates a single figure with an appropriate caption.
 * Threshold current caption includes formula fitting if available from a .agr file.
 * 
 * @param hasLIV Indicates presence of CW L-I-V figure.
 * @param hasIth Indicates presence of CW threshold current figure.
 * 
 * @return QString LaTeX formatted figure code combining CW L-I-V and Ith plots as appropriate.
 */
QString DataSheetGenerator::generateCombinedCWLIVFigures(bool hasLIV, bool hasIth) const
{
    // Default LIV caption for CW (no freq, duty cycle, or gate freq mentioned)
    QString livCaption = "CW L-I-V characteristics.";

    // Get Ith formula text from file (same as before)
    QString latexRootDir = QFileInfo(outputPath).absolutePath();
    QString formulaPath = QDir(latexRootDir + "/GraceFigures").filePath("Ith_vs_T_cw_liv.agr");
    QString formula = parseIthFormulaFromAgrFile(formulaPath);

    QString ithCaption;
    if (!formula.isEmpty()) {
        QString ith = formula.section(',', 0, 0).trimmed();
        QString jth = formula.section(',', 1).trimmed();
        ithCaption = QString(
            "Threshold current vs. temperature, fitted to "
            "\\(I_{\\mathrm{th}}(T) = %1\\), corresponding to current density "
            "\\(J_{\\mathrm{th}}(T) = %2\\)."
        ).arg(ith, jth);
    } else {
        ithCaption = "Threshold current vs. temperature.";
    }

    if (hasLIV && hasIth) {
        QString combinedCaption = QString("%1 (b) %2").arg(livCaption, ithCaption);
        return QString(
            "\\begin{figure}[h!]\n"
            "\\centering\n"
            "\\begin{subfigure}{0.48\\textwidth}\n"
            "    \\includegraphics[width=\\linewidth]{Figures/cw_liv.pdf}\n"
            "    \\caption{\\small CW LIV characteristics}\n"
            "\\end{subfigure}\n"
            "\\hfill\n"
            "\\begin{subfigure}{0.48\\textwidth}\n"
            "    \\includegraphics[width=\\linewidth]{Figures/Ith_vs_T_cw_liv.pdf}\n"
            "    \\caption{\\small CW threshold current}\n"
            "\\end{subfigure}\n"
            "\\caption{\\small %1}\n"
            "\\end{figure}\n"
        ).arg(combinedCaption);
    }

    if (hasLIV) {
        return QString(
            "\\begin{figure}[h!]\n"
            "\\centering\n"
            "\\includegraphics[width=0.7\\textwidth]{Figures/cw_liv.pdf}\n"
            "\\caption{\\small %1}\n"
            "\\end{figure}\n"
        ).arg(livCaption);
    }

    if (hasIth) {
        return QString(
            "\\begin{figure}[h!]\n"
            "\\centering\n"
            "\\includegraphics[width=0.7\\textwidth]{Figures/Ith_vs_T_cw_liv.pdf}\n"
            "\\caption{\\small %1}\n"
            "\\end{figure}\n"
        ).arg(ithCaption);
    }

    return "";
}

QString DataSheetGenerator::generateCombinedCWFTIRFigures(bool hasVsI, bool hasVsT) const {
    QString tfix = cwMetadata.value("tfix_spectra", "20").trimmed();
    QString ifix = cwMetadata.value("ifix_spectra", "").trimmed();

    QString mainCaption = "CW FTIR emission spectra.";

    QString spectraVsTCaption = ifix.isEmpty() 
        ? "Spectra at different temperatures." 
        : QString("Spectra at different temperatures (at I = %1 mA).").arg(ifix);

    if (hasVsI && hasVsT) {
        return QString(R"(
\begin{figure}[h!]
\centering
\begin{subfigure}{0.48\textwidth}
    \includegraphics[width=\linewidth]{Figures/cw_ftir_vs_I.pdf}
    \caption{\small Spectra at different currents (at T = %1 K).}
\end{subfigure}
\hfill
\begin{subfigure}{0.48\textwidth}
    \includegraphics[width=\linewidth]{Figures/cw_ftir_vs_T.pdf}
    \caption{\small %2}
\end{subfigure}
\caption{\small %3}
\end{figure}
)").arg(tfix, spectraVsTCaption, mainCaption);
    } 
    else if (hasVsI) {
        return QString(R"(
\begin{figure}[h!]
\centering
\includegraphics[width=0.7\textwidth]{Figures/cw_ftir_vs_I.pdf}
\caption{\small %1 Spectra at different currents (at T = %2 K).}
\end{figure}
)").arg(mainCaption, tfix);
    } 
    else if (hasVsT) {
        return QString(R"(
\begin{figure}[h!]
\centering
\includegraphics[width=0.7\textwidth]{Figures/cw_ftir_vs_T.pdf}
\caption{\small %1 %2}
\end{figure}
)").arg(mainCaption, spectraVsTCaption);
    }

    return "";
}

/**
 * @brief Generates a LaTeX figure environment for embedding an image with a caption.
 * 
 * Constructs a figure block with the specified image file located under the "Figures" directory,
 * setting the image width to 85% of the text width and applying LaTeX escaping to the caption.
 * 
 * @param fileName Name of the image file (e.g., "example.pdf").
 * @param caption Caption text for the figure.
 * 
 * @return QString LaTeX code for the figure block.
 */
QString DataSheetGenerator::generateFigureBlock(const QString& fileName, const QString& caption) const {
    QString figurePath = "Figures/" + fileName;
    return QString(R"(
\begin{figure}[h!]
\centering
\includegraphics[width=0.85\textwidth]{%1}
\caption{%2}
\end{figure}
)").arg(figurePath, escapeLatex(caption));
}

/**
 * @brief Generates a LaTeX formatted subsection for experimental notes.
 * 
 * If the provided notes string is empty or whitespace, returns an empty string.
 * Otherwise, formats the notes as a subsection titled "Experimental Notes" with vertical spacing.
 * 
 * @param notes The experimental notes text to include.
 * 
 * @return QString LaTeX code for the experimental notes subsection or empty string if notes are empty.
 */
QString DataSheetGenerator::generateExperimentalNotes(const QString &notes) const {
    if (notes.trimmed().isEmpty())
        return "";

    return QString(R"(
\vspace{0.5cm}
\subsubsection*{Experimental Notes}
%1
)").arg(notes);
}

/**
 * @brief Parses threshold current (Ith) and current density (Jth) formulas from a Grace .agr file.
 * 
 * Opens the specified .agr file and scans for the legend line associated with dataset "s1" containing
 * threshold current formulas. Extracts the formulas, cleans Grace-specific formatting, converts them
 * to LaTeX-friendly syntax, and wraps units in \mathrm{} for proper LaTeX rendering.
 * 
 * The method expects at least two formulas separated by two newlines in the legend text: one for Ith,
 * one for Jth. It extracts the right-hand side of the equations after '=' and returns both as a
 * comma-separated string.
 * 
 * @param filePath Absolute or relative path to the .agr file to parse.
 * 
 * @return QString A LaTeX-ready string containing Ith and Jth formulas separated by a comma,
 *                or an empty string if the file cannot be opened or no valid legend is found.
 */
QString DataSheetGenerator::parseIthFormulaFromAgrFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filePath;
        return "";
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        qDebug() << "Reading line:" << line;

        if (line.startsWith("@") && line.contains("s1") && line.contains("legend")) {
            qDebug() << "Found legend line";

            int firstQuote = line.indexOf('"');
            int lastQuote = line.lastIndexOf('"');
            if (firstQuote == -1 || lastQuote == -1 || firstQuote >= lastQuote) {
                qDebug() << "Quote parsing error in line";
                continue;
            }

            QString legendText = line.mid(firstQuote + 1, lastQuote - firstQuote - 1);
            qDebug() << "Extracted legend text:" << legendText;

            // Remove Grace formatting tags \q \Q and \N but keep other meaningful markup
            legendText.replace("\\q", "");
            legendText.replace("\\Q", "");
            legendText.replace("\\N", "");

            // Replace superscript -2 notation with LaTeX superscript
            legendText.replace("\\S-2", "^{-2}");
            // Remove other \S (like \S-1, \S-3 if any) - if you expect others, handle accordingly
            legendText.replace("\\S", "");

            // Replace literal \n with newline
            legendText.replace("\\n", "\n");

            qDebug() << "Cleaned legend text:" << legendText;

            // Split formulas by two newlines, each formula contains the full expression including units
            QStringList formulas = legendText.split("\n\n", Qt::SkipEmptyParts);
            qDebug() << "Split into formulas count:" << formulas.size();

            if (formulas.size() < 2) {
                qDebug() << "Not enough formulas found";
                return "";
            }

            // Extract formula RHS (right side of =) but keep units inside []
            QString ithFormula = formulas[0].section('=', 1).trimmed();
            QString jthFormula = formulas[1].section('=', 1).trimmed();

            qDebug() << "Ith formula raw:" << ithFormula;
            qDebug() << "Jth formula raw:" << jthFormula;

			// Replace "exp" with "\exp" (single backslash)
			ithFormula.replace(QRegularExpression("\\bexp\\b"), "\\exp");
			jthFormula.replace(QRegularExpression("\\bexp\\b"), "\\exp");


            // Wrap units in \mathrm{...}
            // Find units inside [ ... ] and replace brackets with \mathrm{...}
            auto wrapUnits = [](QString& formula) {
                QRegularExpression unitRx("\\[(.*?)\\]");
                QRegularExpressionMatchIterator i = unitRx.globalMatch(formula);
                while (i.hasNext()) {
                    QRegularExpressionMatch match = i.next();
                    QString unitText = match.captured(1);
                    // Replace [unitText] with \mathrm{unitText}
                    formula.replace(match.captured(0), "~\\mathrm{" + unitText + "}");
                }
            };

            wrapUnits(ithFormula);
            wrapUnits(jthFormula);

            qDebug() << "Ith formula after LaTeX formatting:" << ithFormula;
            qDebug() << "Jth formula after LaTeX formatting:" << jthFormula;

            // Return both formulas separated by comma for your caption builder
            return ithFormula + ", " + jthFormula;
        }
    }

    qDebug() << "No legend line found in file";
    return "";
}

QString DataSheetGenerator::escapeLatex(const QString& str) const 
{
    QString escaped = str;
    escaped.replace("\\", "\\textbackslash{}");
    escaped.replace("&", "\\&");
    escaped.replace("%", "\\%");
    escaped.replace("$", "\\$");
    escaped.replace("#", "\\#");
    escaped.replace("_", "\\_");
    escaped.replace("{", "\\{");
    escaped.replace("}", "\\}");
    escaped.replace("~", "\\textasciitilde{}");
    escaped.replace("^", "\\textasciicircum{}");
    escaped.replace("\n", " ");
    escaped.replace("\r", " ");
    return escaped.simplified();
}

/**
 * @brief Removes trailing backslash characters from the given string.
 * 
 * This helper function trims any trailing backslashes '\\' from the end of the input string.
 * It is useful for cleaning up strings intended for LaTeX output to prevent runaway commands.
 * 
 * @param str The input QString potentially containing trailing backslashes.
 * @return QString The input string with all trailing backslashes removed.
 */
QString DataSheetGenerator::trimTrailingBackslashes(const QString& str) 
{
    QString s = str;
    while (s.endsWith('\\')) {
        s.chop(1);  // remove last char
    }
    return s;
}