/**
* \file		graceplot.cpp
* \brief 	GracePlot class - contains methods for plotting data
* \author 	Aleksandar Demic <A.Demic@leeds.ac.uk>
*/
#include <vector>
#include <limits>
#include <algorithm>  /
#include <sstream>
#include <iomanip>
#include <QtConcurrent>
#include "GracePlot.h"
#include <cmath>
#include <QVector>
#include <iostream>
#include <QVector>
#include <cmath>

/**
 * \brief Writes predefined color mappings to the given file stream for Grace plot colors.
 * 
 * \param file Reference to an open std::fstream to write the color setup commands.
 */
void GracePlot::setColors(std::fstream &file) const
{
	std::string colorSetup ="@version 50123 \n"
							"@map color 0 to (255, 255, 255), \"white\" \n"
							"@map color 1 to (0, 0, 0), \"black\" \n"
							"@map color 2 to (255, 0, 0), \"red\" \n"
							"@map color 3 to (0, 0, 255), \"blue\" \n"
							"@map color 4 to (0, 139, 0), \"green4\" \n"
							"@map color 5 to (255, 165, 0), \"orange\" \n"
							"@map color 6 to (188, 143, 143), \"brown\" \n"
							"@map color 7 to (103, 7, 72), \"maroon\" \n"
							"@map color 8 to (0, 255, 0), \"green\" \n"
							"@map color 9 to (0, 127, 255), \"azure\" \n"
							"@map color 10 to (184, 115, 51), \"copper\" \n"
							"@map color 11 to (255, 215, 0), \"gold\" \n"
							"@map color 12 to (255, 0, 255), \"magenta\" \n"
							"@map color 13 to (128, 128, 128), \"gray\" \n"
							"@map color 14 to (114, 33, 188), \"indigo\" \n"
							"@map color 15 to (64, 224, 208), \"turquoise\" \n";
	if (file.is_open())
		file.write(colorSetup.data(), colorSetup.size());			
};

/**
 * \brief Configures axis properties for a Grace plot axis and writes settings to the file stream.
 * 
 * \param file Reference to an open std::fstream to write axis configuration commands.
 * \param xy The axis identifier (e.g., "x" or "y").
 * \param label The label text for the axis.
 * \param tick The major tick interval for the axis.
 * \param labelSize The font size for the axis label and tick labels.
 * \param labelPlace Positioning for the label and ticks (e.g., "default", "opposite").
 * \param grid Boolean flag indicating whether to enable grid lines for major ticks.
 */
void GracePlot::setAxis(std::fstream &file, const std::string xy, const std::string label, double tick, double labelSize, std::string labelPlace, bool grid) const
{
	std::string axis="\n @ " + xy+"axis";
	std::string axisSetup = axis + " on";
	axisSetup += axis + " label \"" + label + "\"";
	if (grid)
	{
		axisSetup+=axis + " tick major linestyle 2";
		axisSetup+= axis + " tick major linewidth 1.1";
		axisSetup+=axis + " tick major grid on";
	}
	axisSetup+= axis + " tick major " + std::to_string(tick);
	axisSetup+= axis + " tick minor ticks " + std::to_string(1);
	axisSetup+= axis + " label char size " + std::to_string(labelSize);
	axisSetup+= axis + " ticklabel on ";
	axisSetup+= axis + " ticklabel char size " + std::to_string(labelSize);
	axisSetup+= axis + " label place " + labelPlace;
	axisSetup+= axis + " ticklabel place " + labelPlace;
	axisSetup+= axis + " tick place " + labelPlace;
	if (file.is_open())
		file.write(axisSetup.data(), axisSetup.size());		
};

/**
 * \brief Sets up graph parameters such as axis ranges, view window, subtitle, and legend for a Grace plot.
 * 
 * \param file Reference to an open std::fstream to write the graph configuration.
 * \param graphID The identifier for the graph (e.g., "graph0").
 * \param world The world coordinate range for the graph axes (e.g., "0, 10, 0, 100").
 * \param view The viewport settings for the graph (e.g., "0.15, 0.95, 0.15, 0.85").
 * \param subtitle Optional subtitle text for the graph; empty string disables subtitle.
 */
void GracePlot::setGraph(std::fstream &file, std::string graphID, std::string world, std::string view, std::string subtitle) const
{
	std::string graphSetup = "\n @ " + graphID + " on\n";
	graphSetup+= "@ with " + graphID + "\n"; 
	graphSetup+="@ world " + world+"\n";
	if (subtitle != "")
	{
		graphSetup+="@ subtitle " + subtitle + "\n";
		graphSetup+="@ subtitle size " + std::to_string(1.1) + "\n";
	}
	graphSetup+="@ legend on \n"
				"@ legend 0.18, 0.86 \n"
				"@ legend char size 1.100000\n";
	graphSetup+="@ view "+view;
	if (file.is_open())
		file.write(graphSetup.data(), graphSetup.size());		
};

/**
 * \brief Configures the appearance and properties of a subgraph (data series) in a Grace plot.
 * 
 * \param file Reference to an open std::fstream to write subgraph configuration commands.
 * \param subgraphID Identifier for the subgraph (e.g., "s0", "s1").
 * \param linewidth Line width for the subgraph's line or symbol outline.
 * \param linestyle Line style identifier (e.g., "0" for solid, "1" for dashed).
 * \param color Color name or index for the line and symbols.
 * \param legend Legend text associated with the subgraph.
 * \param marker If true, the subgraph is drawn with markers (symbols) instead of lines.
 */
void GracePlot::setSubgraph(std::fstream &file, std::string subgraphID, double linewidth, std::string linestyle, std::string color, std::string legend, bool marker) const
{
	std::string subgraphSetup= "\n @ "+subgraphID + " line linewidth " + std::to_string(linewidth) + "\n";
	subgraphSetup+= "@ "+subgraphID + " line color " + color + "\n";
	subgraphSetup+= "@ "+subgraphID + " legend " + legend + "\n";
	if (marker)
	{
		subgraphSetup+= "@ "+subgraphID + " line linestyle 0 \n";
		subgraphSetup+= "@ "+subgraphID + " symbol 1 \n";
		subgraphSetup+= "@ "+subgraphID + " symbol size 1.00000 \n";
		subgraphSetup+= "@ "+subgraphID + " symbol color " + color + "\n";
		subgraphSetup+= "@ "+subgraphID + " symbol pattern 1 \n";
		subgraphSetup+= "@ "+subgraphID + " symbol fill color " + color + "\n";
		subgraphSetup+= "@ "+subgraphID + " symbol fill pattern 1 \n";
		subgraphSetup+= "@ "+subgraphID + " symbol linewidth " + std::to_string(linewidth) + "\n";
		subgraphSetup+= "@ "+subgraphID + " symbol char 65 \n";
		
	}	
	else
		subgraphSetup += "@ " + subgraphID + " line linestyle " + linestyle + "\n";

	if (file.is_open())
		file.write(subgraphSetup.data(), subgraphSetup.size());		
};

/**
 * \brief Writes commands to enable and configure ellipses and lines annotations in the Grace plot.
 * 
 * The function sets properties such as ellipse position, line styles, colors, arrow styles, and visibility.
 * 
 * \param file Reference to an open std::fstream to write ellipse and line annotation commands.
 */
void GracePlot::setEllipses(std::fstream &file) const
{	std::string ellipseSetup = "@ with ellipse \n"
			"@ ellipse on \n"
			"@ ellipse loctype view \n"
			"@ ellipse 0.95, 0.2, 1.05, 0.25 \n"
			"@ ellipse linestyle 1 \n"
			"@ ellipse linewidth 2.5 \n"
			"@ ellipse color 1 \n"
			"@ ellipse fill color 1 \n"
			"@ ellipse fill pattern 0 \n"
		"@ ellipse def \n"
		"@ with ellipse \n"
			"@ ellipse on \n"
			"@ ellipse loctype view \n"
			"@ ellipse 0.25, 0.2, 0.35, 0.25 \n"
			"@ ellipse linestyle 1 \n"
			"@ ellipse linewidth 2.5 \n"
			"@ ellipse color 2 \n"
			"@ ellipse fill color 1 \n"
			"@ ellipse fill pattern 0\n "
		"@ ellipse def \n"
		"@ with line \n"
			"@ line on \n"
			"@ line loctype view \n"
			"@ line 1, 0.25, 1.09, 0.25 \n"
			"@ line linewidth 2.5 \n"
			"@ line linestyle 1 \n"
			"@ line color 1 \n"
			"@ line arrow 2 \n"
			"@ line arrow type 0 \n"
			"@ line arrow length 1.000000 \n"
			"@ line arrow layout 1.000000, 1.000000 \n"
		"@ line def \n"
		"@ with line \n"
			"@ line on \n"
			"@ line loctype view \n"
			"@ line 0.21, 0.25, 0.3, 0.25 \n"
			"@ line linewidth 2.5 \n"
			"@ line linestyle 1 \n"
			"@ line color 2 \n"
			"@ line arrow 1 \n"
			"@ line arrow type 0 \n"
			"@ line arrow length 1.000000 \n"
			"@ line arrow layout 1.000000, 1.000000 \n"
			"@ line def \n";
	if (file.is_open())
		file.write(ellipseSetup.data(), ellipseSetup.size());	
};

/**
 * \brief Writes XY data points to the Grace plot file for a specific graph and subgraph.
 * 
 * Writes data points in the format expected by Grace, associating the data with the given graph and subgraph IDs.
 * 
 * \param file Reference to an open std::fstream for writing data.
 * \param x QVector of x-coordinates.
 * \param y QVector of y-coordinates.
 * \param graphID Identifier of the target graph (e.g., "g0").
 * \param subgraphID Identifier of the subgraph within the graph (e.g., "s0").
 */
void GracePlot::printData(std::fstream &file, const QVector<double> &x, const QVector<double> &y, 
					  std::string graphID, std::string subgraphID) const
{
	std::string DataTarget;
	if ((graphID == "g0") && (subgraphID == "s0"))
	{
		DataTarget = "@ target g0.s0 \n"
					 "@type xy \n";
	}
	else
	{
		DataTarget = "&\n";
		DataTarget += "@ target " + graphID + "." + subgraphID + "\n";
		DataTarget += "@type xy \n";
	}

	if (file.is_open())
	{
		file.write(DataTarget.data(), DataTarget.size());
		for (unsigned int i = 0; i < x.size(); i++)
		{
			std::string line = std::to_string(x.at(i)) + " " + std::to_string(y.at(i)) + "\n";
			file.write(line.data(), line.size());
		}
	}
}

/**
 * \brief Constructs a world coordinate string for defining the plot ranges in Grace.
 * 
 * Formats the minimum and maximum x and y values into a comma-separated string with fixed decimal precision.
 * 
 * \param xmin Minimum x-coordinate.
 * \param ymin Minimum y-coordinate.
 * \param xmax Maximum x-coordinate.
 * \param ymax Maximum y-coordinate.
 * \return A formatted string representing the world coordinates: "xmin, ymin, xmax, ymax".
 */
std::string GracePlot::makeWorldString(double xmin, double ymin, double xmax, double ymax) const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(6)
		<< xmin << ", "
		<< ymin << ", "
		<< xmax << ", "
		<< ymax;
	return oss.str();
}