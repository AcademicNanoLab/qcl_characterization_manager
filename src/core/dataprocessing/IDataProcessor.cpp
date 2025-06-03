/**
 * \file        IDataProcessor.cpp
 * \brief       Implements the base class for processing LIV and Spectra data files.
 * \author      Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "IDataProcessor.h"

/**
 * \brief Constructs an IDataProcessor object with file metadata and parent object.
 *
 * This constructor initializes common fields used for both LIV and Spectra data processing,
 * including the file name, unit, and trace variable. It also initializes a regular expression
 * used to split strings based on whitespace.
 *
 * \param fileName         The name of the file to be processed.
 * \param unit             The measurement unit used in the data.
 * \param traceVariable    The variable used to trace or label the data.
 * \param parent           The QObject parent, if any.
 */
IDataProcessor::IDataProcessor(const QString &fileName, 
                                 const QString &unit, 
                                 const QString &traceVariable, 
                                 QObject *parent)
    : QObject(parent), fileName(fileName), unit(unit), traceVariable(traceVariable)
{
    // Initialize the regular expression for whitespace separation
    whiteSpaceRx = new QRegularExpression("\\s+");
}