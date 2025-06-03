/**
 * \file   	graceplot.h
 * \brief 	Visualiser class - contains methods for plotting data
 * \author 	Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
#pragma once
#ifndef GRACE_PLOT_H
	#define GRACE_PLOT_H
	#if HAVE_CONFIG_H
		#include "config.h"
	#endif
	#include "core/dataprocessing/LIVDataProcessor.h"
	#include "core/dataprocessing/SpectraDataProcessor.h"
	#include "core/dataprocessing/IthDataProcessor.h"
	#include <QtConcurrent>
	#include <fstream>
	#include <string>

	/**
	* \brief 	Graceplot -  data plotting class for creating Grace figures
	*
	* \details 	The class is equipped with methods that can directly generate vector figures
	*			that can be plotted via Grace package in Linux and QtGrace in Windows.
	*			The format of Grace figures stores data in well organised manner,
	*			each graph has a label G and number X, and each trace on the graph
	*			has a label S and number Y, thus G0.S0 would be one graph with one trace,
	*			G0.S0, G0.S1 would have two traces and new graphs can be added by adding
	*			traces to G1.S0, G1.S1 ... Each trace and graph can be manipulated in various ways that
	*			add legend and labels, and Grace has special type format to allow Latex-like fonts
	*			but the codes for special characters, super/subscripts etc are unique to Grace
	*			and instruction can be found in https://plasma-gate.weizmann.ac.il/Grace/doc/UsersGuide.html#typesetting
	*			To setup each graph, boundaries of graph need to be exactly specified, thus
	*			this class contins some common figure layouts (ie plots with dual axes, inset plots, subplots etc.).
	*			Due to the fact that each plot may contain different amount of traces and data, this class does not have
	*			a user-defined constructor, user needs to provide data in corresponding format to a method that would handle the data.
	*
	* \author 	Aleksandar Demic <A.Demic@leeds.ac.uk>
	*
	*/
	class GracePlot
	{
		protected:
			void setColors(std::fstream &file) const;                      ///< Set standard colors for plots
			void setEllipses(std::fstream &file) const;                    ///< Add ellipse shapes to plot
			void setGraph(std::fstream &file, std::string graphID, 
						  std::string world, std::string view, 
						  std::string subtitle) const;                      ///< Setup a graph with view and labels
			void setAxis(std::fstream &file, const std::string xy, 
						 const std::string label, double tick, 
						 double labelSize, std::string labelPlace, 
						 bool grid = false) const;                          ///< Configure axis properties
			void setSubgraph(std::fstream &file, std::string subgraphID, 
							 double linewidth, std::string linestyle, 
							 std::string color, std::string legend, 
							 bool marker = false) const;                    ///< Setup a subgraph with style and legend
			void printData(std::fstream &file, const QVector<double> &x, 
						   const QVector<double> &y, std::string graphID, 
						   std::string subgraphID) const;                    ///< Output data points to file
			std::string makeWorldString(double xmin, double ymin, 
										double xmax, double ymax) const;   ///< Format world coordinate string for graphs
	};
#endif // GRACE_PLOT_H