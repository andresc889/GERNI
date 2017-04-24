/*
 * Plotter.cpp
 *
 *  Created on: Dec 15, 2015
 *      Author: andrescj
 */

/**** MY INCLUDES *************************************************************/

#include "Graph.hpp"
#include "Plotter.hpp"

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include <plplot.h>

/**** STANDARD INCLUDES *******************************************************/

#include <stdlib.h>
#include <vector>

/**** END OF INCLUDES *********************************************************/

Plotter::Plotter(std::vector<Graph> &graphs)
{
    double x_min;
    double x_max;
    double y_min;
    double y_max;

    bool first_graph = true;

    if (graphs.size() < 1)
        return;

    pls = new plstream();

    // Find minima and maxima for x and y among all the graphs
    for (size_t g = 0; g < graphs.size(); g++)
    {
        Graph &cur_graph = graphs[g];

        if (cur_graph.get_num_points() > 1)
        {
            // Find the minima and maxima
            if (first_graph)
            {
                x_min = cur_graph.get_x_at(0);
                x_max = cur_graph.get_x_at(0);
                y_min = cur_graph.get_y_at(0);
                y_max = cur_graph.get_y_at(0);

                first_graph = false;
            }

            for (size_t i = 0; i < cur_graph.get_num_points(); i++)
            {
                if (cur_graph.get_x_at(i) < x_min)
                    x_min = cur_graph.get_x_at(i);
                if (cur_graph.get_x_at(i) > x_max)
                    x_max = cur_graph.get_x_at(i);
            }

            for (size_t i = 0; i < cur_graph.get_num_points(); i++)
            {
                if (cur_graph.get_y_at(i) < y_min)
                    y_min = cur_graph.get_y_at(i);
                if (cur_graph.get_y_at(i) > y_max)
                    y_max = cur_graph.get_y_at(i);
            }
        }
    }

    if (!first_graph)
    {
        // Display device in an X window
        pls->sdev("xwin");

        // Initialize plot
        pls->init();

        // Create a labelled box to hold the plot.
        pls->env(x_min, x_max, y_min, y_max, 0, 0);
        pls->lab("t", "y", "GERNI (PLplot)");

        // Plot the graphs
        for (size_t g = 0; g < graphs.size(); g++)
        {
            Graph &cur_graph = graphs[g];

            // Set the attributes
            pls->col0(cur_graph.get_line_color());
            pls->lsty(cur_graph.get_line_style());

            // Draw the line
            pls->line(cur_graph.get_num_points(), cur_graph.get_x_array(),
                    cur_graph.get_y_array());
        }
    }

    // In C++ we don't call plend() to close PLplot library
    // this is handled by the destructor
    //
    delete pls;
}
