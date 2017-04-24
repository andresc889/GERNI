/*
 * ODE.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: andrescj
 */

/**** MY INCLUDES *************************************************************/

#include "ODE.hpp"
#include "ODESystem.hpp"

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include <exprtk.hpp>

/**** STANDARD INCLUDES *******************************************************/

#include <iostream>
#include <math.h>

/**** END OF INCLUDES *********************************************************/

parser_t ODESystem::parser;

ODESystem::ODESystem(const std::string &expr_str, size_t num_eqs)
{
    // Reserve memory for the derivatives and the current state
    dy = new double[num_eqs];
    y = new double[num_eqs];
    t = new double[1];

    // Add vectors to the parser's symbol table
    sym_table.add_vector("dy", dy, num_eqs);
    sym_table.add_vector("y", y, num_eqs);
    sym_table.add_vector("t", t, 1);

    expr.register_symbol_table(sym_table);

    // Compile expression
    parser.compile(expr_str, expr);
}

void ODESystem::operator()(const state &y, state &dy, const double t)
{
    // Copy state to member array so that we can evaluate the expression
    for (size_t i = 0; i < y.size(); i++)
    {
        this->y[i] = y[i];
    }

    this->t[0] = t;

    // Evaluate derivative
    expr.value();

    // Return the derivative through the given reference
    for (size_t i = 0; i < dy.size(); i++)
    {
        dy[i] = this->dy[i];
    }
}
