/*
 * ODESystem.hpp
 *
 *  Created on: Dec 17, 2015
 *      Author: andrescj
 */

#ifndef ODESYSTEM_HPP_
#define ODESYSTEM_HPP_

/**** MY INCLUDES *************************************************************/

#include "ODE.hpp"

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include <exprtk.hpp>

/**** STANDARD INCLUDES *******************************************************/

#include <string.h>

/**** END OF INCLUDES *********************************************************/

/* Type definitions taken from the readme.txt file for exprtk */
typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double> expression_t;
typedef exprtk::parser<double> parser_t;

/*
 * ODESystem
 * My class to represent a system of ODEs. Based on
 * .../boost_numeric_odeint/tutorial/harmonic_oscillator.html and
 * readme.txt for exprtk
 */

class ODESystem
{
public:
    ODESystem(const std::string &, size_t);
    void operator()(const state &, state &, const double);
private:
    symbol_table_t sym_table;
    expression_t expr;

    static parser_t parser;

    double *dy;
    double *y;
    double *t;
};

#endif /* ODESYSTEM_HPP_ */
