/*
 * Graph.hpp
 *
 *  Created on: Dec 15, 2015
 *      Author: andrescj
 */

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

/**** STANDARD INCLUDES *******************************************************/

#include <stdlib.h>
#include <vector>

/**** END OF INCLUDES *********************************************************/

/*
 * Graph
 * My own class to represent graphs that PlPlot can graph.
 */

class Graph
{
public:
    enum plplot_color
    {
        BLACK,
        RED,
        YELLOW,
        GREEN,
        AQUAMARINE,
        PINK,
        WHEAT,
        GREY,
        BROWN,
        BLUE,
        BLUE_VIOLET,
        CYAN,
        TURQUOISE,
        MAGENTA,
        SALMON,
        WHITE
    };

    enum plplot_line_style
    {
        CONTINUOUS = 1, SHORT_DASHED, LONG_DASHED
    };

    Graph();
    Graph(plplot_color, plplot_line_style);

    void add_point(double, double);

    size_t get_num_points();
    double get_x_at(size_t);
    double get_y_at(size_t);
    double *get_x_array();
    double *get_y_array();
    int get_line_color();
    int get_line_style();
private:
    std::vector<double> x;
    std::vector<double> y;
    plplot_color l_color;
    plplot_line_style l_style;
};

#endif /* GRAPH_HPP_ */
