/*
 * ODESystemFitnessEvaluator.cpp
 *
 *  Created on: Dec 18, 2015
 *      Author: andrescj
 */

/**** MY INCLUDES *************************************************************/

#include "ODESolver.hpp"
#include "ODESystem.hpp"
#include "ODESystemFitnessEvaluator.hpp"

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include "ArgUtils.h"
#include "BNFGrammar.h"
#include "FitnessEvaluator.h"
#include "IndividualCollection.h"

/**** STANDARD INCLUDES *******************************************************/

#include <float.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

/**** END OF INCLUDES *********************************************************/

const char *ODESystemFitnessEvaluator::expr_grammar_str = ""
        "<expr> ::= <expr> <biop> <expr> | <uiop>(<expr>) | (<expr>) | <dgit>.<dgit> | <y> | t\n"
        "<biop> ::= + | - | * | /\n"
        "<uiop> ::= -\n"
        "<dgit> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9\n";

ODESystemFitnessEvaluator::ODESystemFitnessEvaluator(const std::string &target_path)
{
    this->inited = false;

    this->target_path = target_path;

    this->min_time = 0;
    this->max_time = 0;

    // Parse the target file
    std::ifstream target(target_path.data());

    std::string line;
    size_t line_number = 0;
    bool first_time = true;

    // Read line by line: http://stackoverflow.com/a/2291840
    while (std::getline(target, line))
    {
        if (line_number > 0)
        {
            target_states.push_back(state());
        }

        // Iterate through line: http://stackoverflow.com/a/237280
        std::istringstream line_stream(line);
        std::istream_iterator<double> start(line_stream), end;

        for (std::istream_iterator<double> it = start; it != end; it++)
        {
            if (line_number == 0)
            {
                if (first_time)
                {
                   min_time = max_time = *it;
                   first_time = false;
                }
                else
                {
                    if (*it < min_time)
                    {
                        min_time = *it;
                    }

                    if (*it > max_time)
                    {
                        max_time = *it;
                    }
                }

                this->target_times.push_back(*it);
            }
            else
            {
                this->target_states[line_number - 1].push_back(*it);
            }
        }

        line_number++;
    }

    target.close();

    size_t num_eqs = this->target_states.size();

    for (size_t i = 0; i < num_eqs; i++)
    {
        this->target_initial_state.push_back(this->target_states[i][0]);
    }

    // Initialize the ODE grammar: http://stackoverflow.com/a/332132
    std::ostringstream ode_grammar_ss;

    ode_grammar_ss << "<ode>  ::= ";

    for (size_t i = 0; i < num_eqs; i++)
    {
        ode_grammar_ss << "dy[" << i << "] := <expr>";

        if (num_eqs > 1 && i < num_eqs - 1)
        {
            ode_grammar_ss << "; ";
        }
    }

    //ode_grammar_ss << "<ode>  ::= dy[0] := y[1]; dy[1] := <expr>";

    ode_grammar_ss << "\n";
    ode_grammar_ss << ODESystemFitnessEvaluator::expr_grammar_str;
    ode_grammar_ss << "<y>    ::= ";

    for (size_t i = 0; i < num_eqs; i++)
    {
        ode_grammar_ss << "y[" << i << "]";

        if (num_eqs > 1 && i < num_eqs - 1)
        {
            ode_grammar_ss << " | ";
        }
    }

    ode_grammar_ss << "\n";

    this->g = new AGE::BNFGrammar(ode_grammar_ss.str().data());
    this->max_wraps = 80;
    this->max_tree_height = AGE::DepthInfinite;
}

void ODESystemFitnessEvaluator::solve_ode_and_evaluate_fitness(AGE::IndividualCollection *population_ptr, unsigned int ind_index)
{
    AGE::IndividualCollection &population = *population_ptr;

    const AGE::Individual &ind = population.individual(ind_index);

    // Map genotype to phenotype
    AGE::StringPtr expr_str_ptr = g->stringDerivedUsingCodons(
            ind.chromosome().codonVector(), max_wraps, max_tree_height, NULL,
            NULL);

    // Test for validity
    if (expr_str_ptr.IsValid())
    {
        population.setValidityForIndividual(ind_index, true);
        population.setPhenotypeForIndividual(ind_index, expr_str_ptr);
    }
    else
    {
        population.setValidityForIndividual(ind_index, false);
        return;
    }

    // Solve ODE system of individual
    ODESystem system(*(expr_str_ptr.GetRawPointer()), target_states.size());
    ODESolver solver(&system, &target_initial_state, min_time, max_time, 0.1);

    solver.solve();

    // Calculate squared error for all equations
    double total_error = 0;

    for (size_t i = 0; i < target_states.size(); i++)
    {
        Graph g;

        solver.to_graph(&g, i);

        size_t cur_target_time = 0;

        for (size_t j = 0; j < g.get_num_points(); j++)
        {
            // If we find inf or nan in a calculated y, the individual is invalid
            if (std::isnan(g.get_y_at(j)) || std::isinf(g.get_y_at(j)))
            {
                population.setValidityForIndividual(ind_index, false);
                return;
            }

            if (std::fabs(g.get_x_at(j) - target_times[cur_target_time]) < 0.05)
            {
                total_error += std::pow(g.get_y_at(j) - target_states[i][cur_target_time], 2);
                cur_target_time++;
            }
        }
    }

    // Reward for an expression that includes <y>'s
    /* std::string &expr = *(expr_str_ptr.GetRawPointer());
    int num_ys = 1;

    for (size_t i = 0; i < expr.length(); i++)
    {
        if (expr[i] == 'y')
        {
            num_ys++;
        }
    } */

    // Reward shorter expressions
    /* total_error *= expr_str_ptr.GetRawPointer()->length();
    total_error /= target_times.size(); */

    float fitness = (float)total_error;

    if (std::isinf(fitness))
    {
        population.setValidityForIndividual(ind_index, false);
        return;
    }

    // Assign the fitness to the individual
    population.setFitnessForIndividual(ind_index, (float) total_error);
}

ODESystemFitnessEvaluator *ODESystemFitnessEvaluator::clone() const
{
    return new ODESystemFitnessEvaluator(target_path);
}

void ODESystemFitnessEvaluator::init()
{
    inited = true;
}

bool ODESystemFitnessEvaluator::isInited() const
{
    return inited;
}

void ODESystemFitnessEvaluator::evaluate(AGE::IndividualCollection &population)
{
    AGE::IndividualCollection::const_iterator i;
    unsigned int j;

    for (i = population.begin(), j = 0; i != population.end(); i++, j++)
    {
        solve_ode_and_evaluate_fitness(&population, j);
    }
}

AGE::FitnessOrder ODESystemFitnessEvaluator::order() const
{
    return AGE::LowerBetter;
}

AGE::FitnessRange ODESystemFitnessEvaluator::range() const
{
    return AGE::FitnessNonnegative;
}

bool ODESystemFitnessEvaluator::setsPhenotype() const
{
    return true;
}

bool ODESystemFitnessEvaluator::setsDerivationInfo() const
{
    return false;
}

bool ODESystemFitnessEvaluator::setsDerivationTree() const
{
    return false;
}

bool ODESystemFitnessEvaluator::canSetDerivationTree() const
{
    return false;
}

void ODESystemFitnessEvaluator::setDerivationTree(bool doSet)
{
    (void) doSet;
}

const AGE::Grammar::Ptr& ODESystemFitnessEvaluator::grammar() const
{
    static const AGE::Grammar::Ptr ret_g(g);

    return ret_g;
}

unsigned ODESystemFitnessEvaluator::maxWraps() const
{
    return max_wraps;
}

unsigned ODESystemFitnessEvaluator::maxTreeHeight() const
{
    return max_tree_height;
}

ODESystemFitnessEvaluator* ODESystemFitnessEvaluator::newObjectWithArg(
        const std::vector<AGE::ArgValue> &argv)
{
    return new ODESystemFitnessEvaluator(argv[0].stringValue());
}

const AGE::ArgFactorySpec& ODESystemFitnessEvaluator::argFactorySpec()
{
    // Based on Listing 6.4 in the AGE documentation
    static AGE::ArgFactorySpec spec =
    { "odereg", 1, AGE::ArgSpecVector() };

    if (spec.argSpecs.empty())
    {
        AGE::ArgSpec as_target_path;

        as_target_path.label = "target: filename.txt";
        as_target_path.type = AGE::StringArg;
        as_target_path.stringCheck = NULL;

        spec.argSpecs.push_back(as_target_path);
    }

    return spec;
}
