/**
 * @file DataSheetGenerator.h
 * @brief Declaration of DataSheetGenerator for creating LaTeX data sheets.
 * 
 * Generates structured LaTeX data sheets with pulsed LIV and Spectra characteristics.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#ifndef DATASHEETGENERATOR_H
	#define DATASHEETGENERATOR_H

	#include <QString>
	#include <QMap>
	#include <QVector>
	#include <QSet>

	/**
	 * @class DataSheetGenerator
	 * @brief Generates LaTeX data sheets from measurement data and figures.
	 */
	class DataSheetGenerator
	{
		public:
			DataSheetGenerator(const QString& outputPath, const QMap<QString, QVariant>& params); ///< Constructor

			void setMeasurementMetadata(const QMap<QString, QString>& pulsedLIV,
										const QMap<QString, QString>& cw); ///< Set measurement metadata
			void setFigures(const QMap<QString, QVector<QString>>& figuresBySection); ///< Set figures grouped by section
			void generate(); ///< Generate the data sheet

		private:
			QString outputPath; ///< Output file path
			QMap<QString, QVariant> params; ///< Parameters for generation
			QSet<QString> availableFigures; ///< Set of available figure names

			QMap<QString, QString> pulsedMetadata; ///< Pulsed measurement metadata
			QMap<QString, QString> cwMetadata; ///< CW measurement metadata (ignored for now)
			QMap<QString, QVector<QString>> figuresMap; ///< Figures organized by section

			QString generateHeader() const; ///< Generate LaTeX header
			QString generateFooter() const; ///< Generate LaTeX footer
			QString generatePerformanceSummary() const; ///< Generate performance summary section

			QString generatePulsedSection() const; ///< Generate pulsed section
			QString generatePulsedLIVSubsection() const; ///< Generate pulsed LIV subsection
			QString generatePulsedSpectraSubsection() const; ///< Generate pulsed spectra subsection
			QString generatePulsedLIVTable() const; ///< Generate pulsed LIV data table
			QString generatePulsedSpectraTable() const; ///< Generate pulsed spectra data table
			QString generateCombinedLIVFigures(bool hasLIV, bool hasIth) const; ///< Generate combined LIV figures
			QString generateCombinedFTIRFigures(bool hasVsI, bool hasVsT) const; ///< Generate combined FTIR figures
			QString generateCWSection() const; ///< Generate CW section
			QString generateCWLIVSubsection() const; ///< Generate CW LIV subsection
			QString generateCWSpectraSubsection() const; ///< Generate CW spectra subsection
			QString generateCWLIVTable() const; ///< Generate CW LIV table
			QString generateCWSpectraTable() const; ///< Generate CW spectra table
			QString generateCombinedCWLIVFigures(bool hasLIV, bool hasIth) const; ///< Generate combined CW LIV figures
			QString generateCombinedCWFTIRFigures(bool hasVsI, bool hasVsT) const; ///< Generate combined CW FTIR figures

			QString generateFigureBlock(const QString& fileName, const QString& caption) const; ///< Generate figure block in LaTeX
			QString generateExperimentalNotes(const QString &notes) const; ///< Generate experimental notes section
			QString parseIthFormulaFromAgrFile(const QString& filePath) const; ///< Parse Ith formula from .agr file

			QString escapeLatex(const QString& str) const; ///< Escape LaTeX special characters
			static QString trimTrailingBackslashes(const QString& str); ///< Trim trailing backslashes from string
	};
#endif // DATASHEETGENERATOR_H