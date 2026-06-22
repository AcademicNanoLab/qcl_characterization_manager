/**
 * @file SpectraGracePlot.cpp
 * @brief Implements the SpectraGracePlot class for generating waterfall plots of spectra data.
 * 
 * This file contains the implementation of the method to generate and output
 * a spectra waterfall plot, where multiple spectra traces are stacked.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "SpectraGracePlot.h"
#include "core/dataprocessing/SpectraDataProcessor.h"
#include <QVector>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * @brief Generates a waterfall plot of spectra data and writes it to a file.
 *
 * This function creates a multi-trace spectra waterfall plot, where each trace
 * is vertically offset to visualize overlapping spectra. The X-axis represents
 * frequency in THz, and the Y-axis shows arbitrary units. Legends are automatically
 * generated for each trace.
 *
 * @param filename The output file name where the plot data will be written.
 * @param data Pointer to the SpectraDataProcessor containing spectra data and metadata.
 */
void SpectraGracePlot::plot_spectra_waterfall(std::string filename, SpectraDataProcessor *data)
{
    std::fstream outfile(filename, std::ios::out);
    if (!outfile.is_open()) {
        qWarning() << "Failed to open file:" << QString::fromStdString(filename);
        return;
    }

    // Retrieve the spectra data
    QList<QVector<double>>& xList = data->getXList();
    QList<QVector<double>>& y1List = data->getY1List();
    const QList<QString>& valueList = data->getValueList();

    // Use SpectraDataProcessor to get the global X-min and X-max based on center and side modes
    double x_min = data->getXmin();
    double x_max = data->getXmax();

    // Create the world string with the updated X-min and X-max values
    std::string worldString = makeWorldString(x_min, -0.07, x_max, (y1List.size() * 2.0));

    // Set colors, graph, and axis definitions
    setColors(outfile);
    setGraph(outfile, "g0", worldString, "0.150000, 0.150000, 1.130000, 0.880000", "Spectra Waterfall Plot");

    // X axis: italic f, units in THz
    setAxis(outfile, "x", "\\qf\\Q [THz]", (x_max - x_min) / 6.0, 1.5, "normal", true);

    // Y axis: arbitrary units
    setAxis(outfile, "y", "a.u.", 1.0, 1.5, "normal", true);

    // Loop to configure subgraphs and assign colors with full legends
    unsigned int numberOfTraces = y1List.size();
    for (unsigned int i = 0; i < numberOfTraces; ++i) {
        std::string subgraphID = "s" + std::to_string(i);
        std::string subgraphColor = (i <= 14) ? std::to_string(i + 1) : std::to_string(15 - i + 1);

        // Prefix legend with valueList (e.g., current, voltage)
        std::ostringstream legend;
        legend << "\"" << data->generateLegendForTrace(i).toStdString() << "\"";

        // Configure subgraph
        setSubgraph(outfile, subgraphID, 7, "1", subgraphColor, legend.str());
    }

    // Plot data with Y offset for waterfall stacking
    double y_offset = 0.0;
    for (int i = 0; i < y1List.size(); ++i) {
        QVector<double> y_offset_data = y1List[i];
        for (double& y : y_offset_data) {
            y += y_offset;
        }

        printData(outfile, xList[i], y_offset_data, "g0", "s" + std::to_string(i));
        y_offset += 1.1;
    }

    outfile.close();
}
