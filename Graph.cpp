/*
 * Graph.cpp
 *
 *  Created on: Dec 15, 2015
 *      Author: andrescj
 */

/**** MY INCLUDES *************************************************************/

#include "Graph.hpp"

/**** STANDARD INCLUDES *******************************************************/

#include <stdlib.h>

/**** END OF INCLUDES *********************************************************/

Graph::Graph()
{
    this->l_color = Graph::WHITE;
    this->l_style = Graph::CONTINUOUS;
}

Graph::Graph(plplot_color l_color, plplot_line_style l_style)
{
    this->l_color = l_color;
    this->l_style = l_style;
}

void Graph::add_point(double x, double y)
{
    this->x.push_back(x);
    this->y.push_back(y);
}

size_t Graph::get_num_points()
{
    return x.size();
}

double Graph::get_x_at(size_t pos)
{
    return x[pos];
}

double Graph::get_y_at(size_t pos)
{
    return y[pos];
}

double *Graph::get_x_array()
{
    return x.data();
}

double *Graph::get_y_array()
{
    return y.data();
}

int Graph::get_line_color()
{
    return l_color;
}

int Graph::get_line_style()
{
    return l_style;
}
