/**
* \file		LIVGracePlot.cpp
* \brief 	GracePlot class - contains methods for plotting data
* \author 	Aleksandar Demic <A.Demic@leeds.ac.uk>
*/
#include <vector>
#include <limits>
#include <algorithm>  // For std::sort
#include <sstream>
#include <iomanip>
#include <QtConcurrent>
#include "LIVGracePlot.h"
#include <cmath>
#include <QVector>
#include <iostream>
#include <QVector>
#include <cmath>

/**
 * @brief Generates and writes LIV (Light-Current-Voltage) plots to a file.
 * 
 * This function processes multiple LIV traces, calculating axis ranges and nice tick steps for
 * current, voltage, current density, and light output power. It sets up two graphs:
 * one for current-voltage (I-V) and another for current density-light output (J-L).
 * It then writes experimental data and subgraph legends indicating temperature values.
 *
 * @param filename The name of the output file to write the plot data.
 * @param data Pointer to an LIVDataProcessor containing the LIV data.
 * @param w The device width in micrometers.
 * @param l The device length in millimeters.
 */
void LIVGracePlot::plot_liv(std::string filename, LIVDataProcessor *data, const double &w, const double &l)
{
    size_t numberOfTraces = data->getValueList().size();
    double currDensityScale = 100000.0 / (w * l);  // [µm × mm → A/cm²]

    // Step 1: Determine I and V axis bounds
    double Imin = std::numeric_limits<double>::max();
    double Imax = std::numeric_limits<double>::lowest();
    double Vmin = std::numeric_limits<double>::max();
    double Vmax = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < numberOfTraces; i++) {
        const QVector<double>& I = data->getXList()[i];
        const QVector<double>& V = data->getY1List()[i];

        if (!I.isEmpty()) {
            Imin = std::min(Imin, *std::min_element(I.begin(), I.end()));
            Imax = std::max(Imax, *std::max_element(I.begin(), I.end()));
        }
        if (!V.isEmpty()) {
            Vmin = std::min(Vmin, *std::min_element(V.begin(), V.end()));
            Vmax = std::max(Vmax, *std::max_element(V.begin(), V.end()));
        }
    }

    // Step 2: Compute nice ticks and adjusted ranges for IV plot
    double I_step = chooseNiceStep(Imin, Imax);
    double V_step = chooseNiceStep(Vmin, Vmax);
    double Imin_final = std::floor(Imin / I_step) * I_step;
    double Imax_final = std::ceil(Imax / I_step) * I_step;
    double Vmin_final = std::floor(Vmin / V_step) * V_step;
    double Vmax_final = std::ceil(Vmax / V_step) * V_step;

	std::string IWorld = makeWorldString(Imin_final == 0.0 ? 0.0001 : Imin_final * 1.0001, Vmin_final == 0.0 ? 0.0001 : Vmin_final * 1.0001, Imax_final, Vmax_final);

    // Step 3: Compute nice ticks and adjusted ranges for IL plot
    double Jmin = Imin * currDensityScale;
    double Jmax = Imax * currDensityScale;
    double J_step = chooseNiceStep(Jmin, Jmax);
    double Jmin_final = std::floor(Jmin / J_step) * J_step;
    double Jmax_final = std::ceil(Jmax / J_step) * J_step;

    double Lmin = 0.0001;	// Trick to avoid both axes have 0 0, this way 0 won't print on any axis
    double Lmax = 1.75*data->getScaleFactor(); // Trick to allow IL curves to be beneat IV curves
	std::string LWorld = makeWorldString(Jmin_final == 0.0 ? 0.0001 : Jmin_final * 1.0001, Lmin, Jmax_final, Lmax);

    std::fstream outfile(filename, std::ios::out);
    setColors(outfile);

    // Graph g0: I-V plot
    setGraph(outfile, "g0", IWorld, " 0.150000, 0.150000, 1.130000, 0.880000", "");
    setAxis(outfile, "x", "\\qI\\Q [A]", I_step, 1.5, "normal", true);
    setAxis(outfile, "y", "\\qV\\Q [V]", V_step, 1.5, "normal", true);

    for (unsigned int i = 0; i < numberOfTraces; i++) {
        std::string subgraphID = "s" + std::to_string(i);
        std::string subgraphColor = (i <= 14) ? std::to_string(i + 1) : std::to_string(15 - i + 1);
        setSubgraph(outfile, subgraphID, 7, "1", subgraphColor, "");
    }
	
	double L_step = chooseNiceStep(Lmin, Lmax, 7);  
	QString yLabel = (data->getScaleFactor() == 100.0) ? "\\qL\\Q [a.u]" : "\\qL\\Q [mW]";
	setGraph(outfile, "g1", LWorld, "0.150000, 0.150000, 1.130000, 0.880000", "");
	setAxis(outfile, "x", "\\qJ\\Q [A cm\\S-2\\N]", J_step, 1.5, "opposite");
	setAxis(outfile, "y", yLabel.toStdString(), L_step, 1.5, "opposite");

    // Subgraphs with temperature legends
    for (unsigned int i = 0; i < numberOfTraces; i++) {
        std::string subgraphID = "s" + std::to_string(i);
        std::string subgraphColor = (i <= 14) ? std::to_string(i + 1) : std::to_string(15 - i + 1);

        double value = data->getValueList()[i].toDouble();
        std::ostringstream oss;
        oss << std::fixed << std::setprecision((std::fmod(value, 1.0) == 0.0) ? 0 : 1);
        oss << value;
        std::string legend = "\"" + oss.str() + " [K]\"";
        setSubgraph(outfile, subgraphID, 7, "1", subgraphColor, legend);
    }

    // Plot IV data
    for (unsigned int i = 0; i < numberOfTraces; i++) {
        const QVector<double>& I = data->getXList()[i];
        const QVector<double>& V = data->getY1List()[i];
        printData(outfile, I, V, "g0", "s" + std::to_string(i));
    }

    // Plot IL data
    for (unsigned int i = 0; i < numberOfTraces; i++) {
        QVector<double> I_scaled = data->getXList()[i];
        for (double& val : I_scaled)
            val *= currDensityScale;

        const QVector<double>& L = data->getY2List()[i];
        printData(outfile, I_scaled, L, "g1", "s" + std::to_string(i));
    }

    outfile.close();
}

/**
 * @brief Calculates a "nice" step size for axis ticks given a value range.
 *
 * This function selects a step size from a set of candidate steps scaled by
 * the order of magnitude of the desired step. The step size ensures that the
 * number of ticks does not exceed the specified maximum.
 *
 * @param minVal The minimum value in the range.
 * @param maxVal The maximum value in the range.
 * @param maxTicks The maximum desired number of ticks on the axis.
 * @return A suitable step size for axis ticks.
 */
double LIVGracePlot::chooseNiceStep(double minVal, double maxVal, int maxTicks) 
{
	double range = maxVal - minVal;
	if (range <= 0) return 1.0;

	// Candidate steps (scaled to powers of 10 later)
	const std::vector<double> steps = {1, 2, 2.5, 5, 10};

	double roughStep = range / maxTicks;
	double magnitude = std::pow(10.0, std::floor(std::log10(roughStep)));

	for (double base : steps) {
		double step = base * magnitude;
		if ((maxVal - minVal) / step <= maxTicks)
			return step;
	}

	// fallback: just return largest
	return 10 * magnitude;
}