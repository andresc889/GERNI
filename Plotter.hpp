/*
 * Plotter.h
 *
 *  Created on: Dec 15, 2015
 *      Author: andrescj
 */

#ifndef PLOTTER_HPP_
#define PLOTTER_HPP_

/**** MY INCLUDES *************************************************************/

#include "Graph.hpp"

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include <plstream.h>

/**** STANDARD INCLUDES *******************************************************/

#include <vector>

/**** END OF INCLUDES *********************************************************/

/*
 * Plotter
 * Based on http://plplot.sourceforge.net/examples.php?demo=00&lbind=C%2B%2B
 */

class Plotter
{
public:
    Plotter(std::vector<Graph> &);
private:
    plstream *pls;
};

#endif /* PLOTTER_HPP_ */
