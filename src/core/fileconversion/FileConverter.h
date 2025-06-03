/**
 * @file FileConverter.h
 * @brief Declaration of FileConverter for converting Grace .agr files to PS and PDF.
 * 
 * Converts Grace (.agr) files to PostScript (.ps) and PDF formats.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#pragma once
#ifndef FILECONVERTER_H
	#define FILECONVERTER_H

	#if HAVE_CONFIG_H
		#include "config.h"
	#endif

	#include <QString>
	#include <QObject>

	/**
	 * @class FileConverter
	 * @brief Converts Grace (.agr) files to PostScript and PDF formats.
	 */
	class FileConverter : public QObject
	{
			Q_OBJECT

		public:
			explicit FileConverter(QObject *parent = nullptr); ///< Constructor

			void processAgrFilesToPsAndPdf(const QString& directory); ///< Convert all .agr files in directory to PS and PDF
			void convertPsToPdf(const QString& psFilePath); ///< Convert a single PS file to PDF
			QString generatePostScript(const QString& agrFilePath); ///< Generate PostScript content from an .agr file

		signals:
			void conversionFinished(); ///< Signal emitted when conversion is finished
	};
#endif // FILECONVERTER_H