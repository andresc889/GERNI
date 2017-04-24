/*
 * GERNI4CPP.cpp
 *
 *  Created on: Dec 18, 2015
 *      Author: andrescj
 */

/**** MY INCLUDES *************************************************************/

#include "Graph.hpp"
#include "ODE.hpp"
#include "ODESolver.hpp"
#include "ODESystem.hpp"
#include "ODESystemFitnessEvaluator.hpp"
#include "Plotter.hpp"

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include "Elements.h"
#include "Tool.h"
#include "Error.h"

/**** STANDARD INCLUDES *******************************************************/

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

/**** END OF INCLUDES *********************************************************/

int main(int argc, char * const *argv)
{
    if (argc == 3 && std::strcmp(argv[1], "plot") == 0)
    {
        // Plot mode
        std::ifstream target(argv[2]);

        // Parse the target file first
        std::vector<double> target_times;
        std::vector<state> target_states;
        state target_initial_state;

        double min_time = 0;
        double max_time = 0;

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

                    target_times.push_back(*it);
                }
                else
                {
                    target_states[line_number - 1].push_back(*it);
                }
            }

            line_number++;
        }

        target.close();

        size_t num_eqs = target_states.size();

        for (size_t i = 0; i < num_eqs; i++)
        {
            target_initial_state.push_back(target_states[i][0]);
        }

        // Obtain the ODE system
        std::string ode_expr_str;

        std::getline(std::cin, ode_expr_str);

        // Solve the ODE system
        ODESystem system(ode_expr_str, num_eqs);
        ODESolver solver(&system, &target_initial_state, min_time, max_time, 0.1);

        solver.solve();

        // Create a set of graphs for the target and the ODE system
        std::vector<Graph> graphs;

        for (size_t teq = 0; teq < num_eqs; teq++)
        {
            Graph g((Graph::plplot_color)(teq + 1), Graph::CONTINUOUS);

            for (size_t i = 0; i < target_states[teq].size(); i++)
            {
                g.add_point(target_times[i], target_states[teq][i]);
            }

            graphs.push_back(g);

            Graph fg((Graph::plplot_color)(teq + 1), Graph::LONG_DASHED);

            solver.to_graph(&fg, teq);

            graphs.push_back(fg);
        }

        // Plot the graphs
        Plotter plotter(graphs);
    }
    else
    {
        // AGE mode: based on AGE's documentation
        try
        {
            AGE::ArgFactory<AGE::CrossoverOperator> crossoverFactory;
            crossoverFactory.add<AGE::OnePointCrossover>();

            AGE::ArgFactory<AGE::MutationOperator> mutationFactory;
            mutationFactory.add<AGE::BitMutation>();
            mutationFactory.add<AGE::CodonMutation>();
            mutationFactory.add<AGE::Duplication>();

            AGE::ArgFactory<AGE::Initialiser> initFactory;
            initFactory.add<AGE::RandomInitialiser>();
            initFactory.add<AGE::RampedInitialiser>();

            AGE::ArgFactory<AGE::FitnessEvaluator> evalFactory;
            evalFactory.add<ODESystemFitnessEvaluator>();

            AGE::ArgFactory<AGE::FitnessScaling> scalingsFactory;
            // evalFactory.add<AGE::LinearFitnessScaling>();

            AGE::ArgFactory<AGE::Selector> selectorFactory;
            selectorFactory.add<AGE::RandomTournamentSelector>();

            AGE::Tool(crossoverFactory, mutationFactory, initFactory, evalFactory,
                    scalingsFactory, selectorFactory, "one-point", "", NULL, NULL,
                    "", "tour").runOrFailWithArgs(argc, argv);

        } catch (const AGE::UserError& fail_cause)
        {
            fail_cause.logAndFail("AGE failed due to the following error");
            return EXIT_FAILURE;
        } catch (const AGE::Error& fail_cause)
        {
            fail_cause.logAndFail("AGE failed due to the following internal error");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
