/**
 * \file   	SpectraGracePlot.h
 * \brief 	SpectraGracePlot class - specializes GracePlot for LIV data visualization
 * \author 	Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#pragma once
#ifndef SPECTRAGRACEPLOT_H
	#define SPECTRAGRACEPLOT_H

	#include "GracePlot.h"
	#include "core/dataprocessing/SpectraDataProcessor.h"
	#include <string>

	/**
	 * @class SpectraGracePlot
	 * @brief GracePlot subclass for plotting spectra data.
	 */
	class SpectraGracePlot : public GracePlot
	{
		public:
			void plot_spectra_waterfall(std::string filename, SpectraDataProcessor *data); ///< Plot spectra waterfall from data
	};
#endif // SPECTRAGRACEPLOT_H