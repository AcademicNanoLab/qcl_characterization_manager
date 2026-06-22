/**
 * \file    IthGracePlot.h
 * \brief   IthGracePlot class - specializes GracePlot for plotting Jth vs T and DR vs T
 * \author  Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#pragma once
#ifndef ITHGRACEPLOT_H
	#define ITHGRACEPLOT_H

	#include "GracePlot.h"
	#include "core/dataprocessing/IthDataProcessor.h"
	#include <string>

	/**
	 * \class IthGracePlot
	 * \brief Inherits from GracePlot, handles plotting of threshold current and dynamic range vs. temperature
	 */
	class IthGracePlot : public GracePlot
	{
			double chooseNiceStep(double minVal, double maxVal, int maxTicks = 7);

		public:
			/**
			 * \brief Plot threshold current Ith vs. temperature in Grace format (with exponential fit)
			 * \param filename Output Grace file
			 * \param data IthDataProcessor object with threshold current data
			 * \param w Width parameter (not used in current version but kept for compatibility)
			 * \param l Length parameter (not used in current version but kept for compatibility)
			 */
			void plot_Ith_vs_T(std::string filename, IthDataProcessor *data, const double &w, const double &l);

			/**
			 * \brief Plot dynamic range (DR) vs. temperature in Grace format (with polynomial fit)
			 * \param filename Output Grace file
			 * \param data IthDataProcessor object with DR data
			 */
			void plot_DR_vs_T(std::string filename, IthDataProcessor *data, const double &w, const double &l);
	};
#endif // ITHGRACEPLOT_H