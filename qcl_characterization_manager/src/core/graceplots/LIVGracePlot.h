/**
 * \file   	LIVGracePlot.h
 * \brief 	LIVGracePlot class - specializes GracePlot for LIV data visualization
 * \author 	Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#pragma once
#ifndef LIVGRACEPLOT_H
	#define LIVGRACEPLOT_H

	#include "GracePlot.h"
	#include "core/dataprocessing/LIVDataProcessor.h"
	#include <string>

	class LIVGracePlot : public GracePlot
	{
		double chooseNiceStep(double minVal, double maxVal, int maxTicks = 8);

		public:
			/**
			 * \brief Plot LIV data in Grace format
			 * \param filename - Name of the file to output the plot
			 * \param data - LIVDataProcessor object containing the data to plot
			 * \param w - Some parameter for width, used in the plot
			 * \param l - Some parameter for length, used in the plot
			 */
			void plot_liv(std::string filename, LIVDataProcessor *data, const double &w, const double &l);
	};
#endif // LIVGRACEPLOT_H