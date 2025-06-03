/**
* \file		IthGracePlot.cpp
* \brief 	GracePlot class - contains methods for plotting data
* \author 	Aleksandar Demic <A.Demic@leeds.ac.uk>
*/
#include <vector>
#include <limits>
#include <algorithm>  
#include <sstream>
#include <iomanip>
#include <QtConcurrent>
#include "IthGracePlot.h"
#include <cmath>
#include <QVector>
#include <iostream>
#include <fstream>
#include <QDebug>

/**
 * \brief Generates a Grace plot file for threshold current (Ith) versus temperature (T).
 * 
 * This function writes a Grace-formatted output file plotting experimental Ith data against temperature,
 * including an optional exponential fit curve. It automatically scales current units (A or mA) depending
 * on the data range and configures axis labels, colors, and graph layout accordingly.
 * 
 * \param filename The path to the output Grace plot file.
 * \param data Pointer to an IthDataProcessor object providing experimental data and fit results.
 * \param w The device width in micrometers (used for current density calculation).
 * \param l The device length in millimeters (used for current density calculation).
 */
void IthGracePlot::plot_Ith_vs_T(std::string filename, IthDataProcessor *data, const double &w, const double &l)
{
    std::fstream outfile(filename, std::ios::out);
    if (!outfile.is_open()) {
        qWarning() << "Failed to open file:" << QString::fromStdString(filename);
        return;
    }

    QVector<double> T_expt = data->getTemperatures();
    QVector<double> Ith_expt = data->getThresholdCurrents();

    double T_min = *std::min_element(T_expt.begin(), T_expt.end());
    double T_max = *std::max_element(T_expt.begin(), T_expt.end());
    double Ith_min = *std::min_element(Ith_expt.begin(), Ith_expt.end());
    double Ith_max = *std::max_element(Ith_expt.begin(), Ith_expt.end())*1.05;

    std::string ylabel = "\\qI\\Q\\sth\\N [A]";
    std::string jlabel = "J\\s\\qth\\Q\\N [A/cm\\S2\\N]";
    bool convertToMilli = false;

    if (Ith_max < 9.9) {
        convertToMilli = true;
        ylabel = "\\qI\\Q\\sth\\N [mA]";
        for (double &val : Ith_expt) val *= 1000.0;
        Ith_min *= 1000.0;
        Ith_max *= 1000.0;
    }

    // --- Nice X-axis (Temperature) ---
	double T_step = chooseNiceStep(T_min, T_max);
    double T0_min = std::floor(T_min / T_step) * T_step;
    double T0_max = std::ceil(T_max / T_step) * T_step;
    double T_margin = (T0_max - T0_min) * 0.1;
    double T_min_final = T0_min;
    double T_max_final = T0_max + T_margin;

    // --- Nice Y-axis (Ith) ---
    double Ith_step = chooseNiceStep(Ith_min, Ith_max);

    double Ymin = std::floor(Ith_min / Ith_step) * Ith_step;
    double Ymax = std::ceil(Ith_max / Ith_step) * Ith_step;

    // --- Graph 0 (Ith) ---
    std::string world0 = makeWorldString(T_min_final, Ymin, T_max_final, Ymax);

    setColors(outfile);
    setGraph(outfile, "g0", world0, "0.150000, 0.150000, 1.130000, 0.880000", "Threshold Current vs Temperature");
    setAxis(outfile, "x", "\\qT\\Q [K]", T_step, 1.5, "normal", true);
    setAxis(outfile, "y", ylabel, Ith_step, 1.5, "normal", true);

    setSubgraph(outfile, "s0", 7, "1", "3", "\" Experimental data \"", true);
    printData(outfile, T_expt, Ith_expt, "g0", "s0");

    int numFitPoints = 50;
    auto [T_fit, Ith_fit] = data->applyExponentialFit(numFitPoints);

    if (!Ith_fit.isEmpty()) {
        double A, B, C0;
        data->getExponentialFitParams(A, B, C0);

        if (convertToMilli) {
            A *= 1000.0;
            C0 *= 1000.0;
            for (double &val : Ith_fit) val *= 1000.0;
        }

        double T0 = 1.0 / B;
        std::ostringstream oss;
		oss << std::fixed << std::setprecision(1);
		oss << "\"\\qI\\Q\\sth\\N(T) = " << C0 << " + " << A << " exp(\\qT\\Q / " << T0 << ") [mA]\\n\\n";

		double area_cm2 = (w * 1e-4) * (l * 0.1); // Âµm Ã— mm â†’ cmÂ²
		double J_A = (convertToMilli ? A / 1000.0 : A) / area_cm2;
		double J_C0 = (convertToMilli ? C0 / 1000.0 : C0) / area_cm2;

		oss << "\\qJ\\Q\\sth\\N(T) = " << J_C0 << " + " << J_A << " exp(  \\qT\\Q/ " << T0 << ") [A cm\\S-2\\N]\"";


        setSubgraph(outfile, "s1", 7, "1", "9", oss.str());
        printData(outfile, T_fit, Ith_fit, "g0", "s1");
    }

    // --- Graph 1 (for Jth axis only, no data) ---
    double area_cm2 = (w * 1e-4) * (l * 0.1); // Âµm Ã— mm â†’ cmÂ²
    double JYmin = (convertToMilli ? Ymin / 1000.0 : Ymin) / area_cm2;
    double JYmax = (convertToMilli ? Ymax / 1000.0 : Ymax) / area_cm2;
    double J_step = chooseNiceStep(JYmin, JYmax);

    std::string world1 = makeWorldString(T_min_final, JYmin, T_max_final, JYmax);

    setGraph(outfile, "g1", world1, "0.150000, 0.150000, 1.130000, 0.880000", "");
    setAxis(outfile, "x", "\\qT\\Q [K]", T_step, 1.5, "normal", true);
    setAxis(outfile, "y", jlabel, J_step, 1.5, "opposite", false);

    outfile.close();
}

/**
 * @brief Generates a Grace plot script for Dynamic Range versus Temperature.
 * 
 * This function writes a plot configuration and data to the specified file,
 * including experimental data points and a polynomial fit curve. It sets up axes,
 * graph properties, and legends for visualization in Grace.
 * 
 * @param filename The output filename for the Grace plot script.
 * @param data Pointer to IthDataProcessor containing temperature and dynamic range data.
 * @param w Width parameter (used for area calculation in cm²).
 * @param l Length parameter (used for area calculation in cm²).
 */
void IthGracePlot::plot_DR_vs_T(std::string filename, IthDataProcessor *data, const double &w, const double &l)
{
    std::fstream outfile(filename, std::ios::out);
    if (!outfile.is_open()) {
        qWarning() << "Failed to open file:" << QString::fromStdString(filename);
        return;
    }

    QVector<double> T_expt = data->getTemperatures();
    QVector<double> DR_expt = data->getDynamicRanges();

    double T_min = *std::min_element(T_expt.begin(), T_expt.end());
    double T_max = *std::max_element(T_expt.begin(), T_expt.end());
    double DR_min = *std::min_element(DR_expt.begin(), DR_expt.end());
    double DR_max = *std::max_element(DR_expt.begin(), DR_expt.end());

    // Calculate nice steps
    double T_step = chooseNiceStep(T_min, T_max);
    double DR_step = chooseNiceStep(DR_min, DR_max);

    // Rounded and padded bounds
    double T0_min = std::floor(T_min / T_step) * T_step;
    double T0_max = std::ceil(T_max / T_step) * T_step;
    double T_margin = (T0_max - T0_min) * 0.1;
    double T_min_final = T0_min;
    double T_max_final = T0_max + T_margin;

    double Ymin = std::floor(DR_min / DR_step) * DR_step;
    double Ymax = std::ceil(DR_max / DR_step) * DR_step;

    std::string TWorld_DR = makeWorldString(T_min_final, Ymin, T_max_final, Ymax);

    // Setup graph
    setColors(outfile);
    setGraph(outfile, "g0", TWorld_DR, "0.150000, 0.150000, 1.130000, 0.880000", "Dynamic Range vs Temperature");
    setAxis(outfile, "x", "\\qT\\Q [K]", T_step, 1.5, "normal", true);
    setAxis(outfile, "y", "\\q\\xD\\f{}I\\Q [mA]", DR_step, 1.5, "normal", true);

    std::string legend = "\" Experimental data \"";
    setSubgraph(outfile, "s0", 7, "1", "12", legend, true);
    printData(outfile, T_expt, DR_expt, "g0", "s0");

    // Polynomial fit
    int polynomial_order = 3;
    std::pair<QVector<double>, QVector<double>> polyFit = data->applyPolynomialFit(50, polynomial_order);
    QVector<double> T_DR_fit = polyFit.first;
    QVector<double> DR_fit = polyFit.second;

    if (!DR_fit.isEmpty()) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "\"\\qD\\Q\\sR\\N = ";

        QVector<double> coefficients;
        data->getPolynomialCoefficients(coefficients);

        for (int i = polynomial_order; i >= 0; i--) {
            double coef = coefficients[i];
            if (coef != 0) {
                if (i != polynomial_order) oss << (coef > 0 ? " + " : " - ");
                if (std::abs(coef) != 1 || i == 0) oss << std::abs(coef);
                if (i > 0) oss << "x";
                if (i > 1) oss << "^" << i;
            }
        }

        oss << "\"";

        setSubgraph(outfile, "s1", 7, "1", "14", oss.str());
        printData(outfile, T_DR_fit, DR_fit, "g0", "s1");
    }

    outfile.close();
}

/**
 * @brief Chooses a "nice" step size for axis ticks based on the given range and maximum number of ticks.
 *
 * The function calculates a step size that divides the range between minVal and maxVal into a suitable number
 * of intervals not exceeding maxTicks, choosing from a predefined set of step candidates scaled by powers of ten.
 *
 * @param minVal The minimum value of the range.
 * @param maxVal The maximum value of the range.
 * @param maxTicks The maximum number of desired ticks on the axis (default is 10).
 * @return A double representing the calculated step size.
 */
double IthGracePlot::chooseNiceStep(double minVal, double maxVal, int maxTicks) 
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