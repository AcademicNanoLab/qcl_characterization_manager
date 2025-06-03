/**
 * \file        FileConverter.cpp
 * \brief       Provides functionality to convert Grace .agr files to PostScript (.ps) format.
 * \author      Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include <QtConcurrent>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>
#include <QProcess>
#include <QImage>
#include <QTransform>
#include "FileConverter.h"

/**
 * \brief   Constructs a FileConverter object.
 * \param   parent - Optional parent QObject.
 */
FileConverter::FileConverter(QObject *parent)
    : QObject(parent)
{}

/**
 * \brief Converts a Grace .agr file to a PostScript (.ps) file.
 * 
 * \param agrFilePath The file path to the input Grace .agr file.
 * 
 * \return The file path to the generated PostScript (.ps) file on success,
 *         or an empty string if the conversion fails.
 * 
 * This function uses the external executable qtgrace.exe located in the
 * application's XMGrace/bin directory to perform the conversion.
 * It runs qtgrace.exe with the appropriate arguments to generate a PostScript
 * file from the provided .agr file. The process is allowed to run up to 60 seconds.
 * 
 * If the qtgrace executable is not found, the process times out, or the
 * conversion fails (non-zero exit code), an empty string is returned.
 * 
 * Warning and debug information is logged to assist troubleshooting.
 */
QString FileConverter::generatePostScript(const QString& agrFilePath)
{
    QString graceExePath = QCoreApplication::applicationDirPath() + "/XMGrace/bin/qtgrace.exe";

    if (!QFile::exists(graceExePath)) {
        qWarning() << "qtgrace.exe not found at:" << graceExePath;
        return "";
    }

    QString psFilePath = QFileInfo(agrFilePath).absolutePath() + "/" + QFileInfo(agrFilePath).baseName() + ".ps";

    QStringList arguments = {
        "-nosafe",
        "-hdevice", "PostScript",
        "-noask",
        "-hardcopy",
        "-printfile", psFilePath,
        agrFilePath
    };

    QProcess process;
    process.setProgram(graceExePath);
    process.setArguments(arguments);
    process.setProcessChannelMode(QProcess::MergedChannels);

    process.start();
    if (!process.waitForFinished(60000)) {  // Wait max 60 seconds
        qWarning() << "qtgrace.exe process timed out or failed to finish";
        return "";
    }

    int exitCode = process.exitCode();
    if (exitCode != 0) {
        qWarning() << "qtgrace.exe failed with exit code:" << exitCode;
        qWarning() << process.readAllStandardOutput();
        return "";
    }

    qDebug() << "PostScript file generated successfully:" << psFilePath;
    return psFilePath;
}

/**
 * \brief Converts a PostScript (.ps) file to PDF and PNG formats using Ghostscript.
 * 
 * \param psFilePath The file path of the input PostScript (.ps) file.
 * 
 * This function performs the following steps:
 *  - Checks for the existence of the Ghostscript executable (gswin64c.exe).
 *  - Converts the given .ps file to a PDF file using Ghostscript.
 *  - Converts the generated PDF file to a high-quality PNG (600 DPI).
 *  - Creates (if needed) and moves the generated PDF and PNG files into a 'Figures' directory,
 *    located one level above the directory containing the input .ps file.
 *  - Deletes the original .ps file after successful conversion and relocation.
 * 
 * If any step fails (missing executable, process errors, directory creation failures, or file operations),
 * appropriate warnings are logged and the function returns without throwing exceptions.
 */
void FileConverter::convertPsToPdf(const QString& psFilePath)
{
    QString gsExePath = QCoreApplication::applicationDirPath() + "/Ghostscript/App/bin/gswin64c.exe";

    if (!QFile::exists(gsExePath)) {
        qWarning() << "Ghostscript executable not found at:" << gsExePath;
        return;
    }

    QString pdfFilePath = psFilePath;
    pdfFilePath.replace(".ps", ".pdf");

    QStringList pdfArgs = {
        "-sDEVICE=pdfwrite",
        "-o", pdfFilePath,
        psFilePath
    };

    QProcess gsProcess;
    gsProcess.setProgram(gsExePath);
    gsProcess.setArguments(pdfArgs);
    gsProcess.setProcessChannelMode(QProcess::MergedChannels);

    gsProcess.start();
    if (!gsProcess.waitForFinished(60000)) {
        qWarning() << "Ghostscript PDF generation timed out or failed";
        return;
    }
    if (gsProcess.exitCode() != 0) {
        qWarning() << "Ghostscript failed with exit code:" << gsProcess.exitCode();
        qWarning() << gsProcess.readAllStandardOutput();
        return;
    }

    // Generate PNG from PDF with high DPI (600)
    QString pngFilePath = pdfFilePath;
    pngFilePath.replace(".pdf", ".png");

    QStringList pngArgs = {
        "-sDEVICE=png16m",
        "-r600",                // 600 DPI for high quality
        "-o", pngFilePath,
        pdfFilePath
    };

    QProcess gsPngProcess;
    gsPngProcess.setProgram(gsExePath);
    gsPngProcess.setArguments(pngArgs);
    gsPngProcess.setProcessChannelMode(QProcess::MergedChannels);

    gsPngProcess.start();
    if (!gsPngProcess.waitForFinished(60000)) {
        qWarning() << "Ghostscript PNG generation timed out or failed";
        return;
    }
    if (gsPngProcess.exitCode() != 0) {
        qWarning() << "Ghostscript PNG generation failed with exit code:" << gsPngProcess.exitCode();
        qWarning() << gsPngProcess.readAllStandardOutput();
        return;
    }

    QFileInfo psFileInfo(psFilePath);
    QDir psDir = psFileInfo.absoluteDir();
    if (!psDir.cdUp()) {
        qWarning() << "Failed to go one directory up from:" << psDir.absolutePath();
        return;
    }

    QString figuresDirPath = psDir.filePath("Figures");
    QDir figuresDir(figuresDirPath);

    // Correctly create the Figures directory by passing full path, NOT "."
    if (!figuresDir.exists()) {
        if (!QDir().mkpath(figuresDirPath)) {
            qWarning() << "Failed to create Figures directory:" << figuresDirPath;
            return;
        }
    }

    QString newPdfPath = figuresDirPath + "/" + psFileInfo.baseName() + ".pdf";
    QString newPngPath = figuresDirPath + "/" + psFileInfo.baseName() + ".png";

    // Overwrite PDF if it exists
    if (QFile::exists(newPdfPath)) {
        QFile::remove(newPdfPath);
    }
    if (!QFile::rename(pdfFilePath, newPdfPath)) {
        qWarning() << "Failed to move PDF to:" << newPdfPath;
    }

    // Overwrite PNG if it exists
    if (QFile::exists(newPngPath)) {
        QFile::remove(newPngPath);
    }
    if (!QFile::rename(pngFilePath, newPngPath)) {
        qWarning() << "Failed to move PNG to:" << newPngPath;
    }

    if (!QFile::remove(psFilePath))
        qWarning() << "Failed to delete .ps file:" << psFilePath;
}

/**
 * \brief Processes all Grace .agr files in the specified directory by converting them
 *        to PostScript (.ps), then to PDF and PNG formats asynchronously.
 * 
 * \param directory The directory path containing the .agr files to process.
 * 
 * This function scans the provided directory for all files with a .agr extension,
 * then performs the following for each file in a background thread:
 *  - Converts the .agr file to a PostScript (.ps) file.
 *  - Converts the generated .ps file to PDF and PNG formats.
 * 
 * Once all files have been processed, a \c conversionFinished signal is emitted on the main thread.
 * 
 * Note: Uses QtConcurrent for asynchronous execution and QPointer to safely reference \c this.
 */
void FileConverter::processAgrFilesToPsAndPdf(const QString& directory)
{
    QDir agrDir(directory);
    QStringList agrFiles = agrDir.entryList(QStringList() << "*.agr", QDir::Files);

    // Use QPointer for safe references
    QPointer<FileConverter> self(this);

    // Run conversion in a background thread
    QtConcurrent::run([directory, agrFiles, self]() {
        QDir localDir(directory);
        for (const QString& agrFile : agrFiles) {
            QString agrFilePath = localDir.absoluteFilePath(agrFile);

            if (!self)
                break;

            // Convert .agr to .ps
            QString psFilePath = self->generatePostScript(agrFilePath);

            // Convert .ps to .pdf and .png
            if (!psFilePath.isEmpty()) {
                self->convertPsToPdf(psFilePath);
            }
        }

        // Emit conversionFinished signal safely on main thread
        if (self) {
            QMetaObject::invokeMethod(self, "conversionFinished", Qt::QueuedConnection);
        }

        qDebug() << "Finished processing all .agr files to .pdf.";
    });
}