/*
 * ODESolver.cpp
 *
 *  Created on: Dec 15, 2015
 *      Author: andrescj
 */

/**** MY INCLUDES *************************************************************/

#include "Graph.hpp"
#include "ODE.hpp"
#include "ODESolver.hpp"

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include <boost/numeric/odeint/integrate/integrate_const.hpp>

/**** STANDARD INCLUDES *******************************************************/

#include <vector>

/**** END OF INCLUDES *********************************************************/

using namespace boost::numeric::odeint;

ODESolver::ODESolver(const ODESystem *system, const state *initial_state, double min_t,
        double max_t, double step_size)
{
    this->system = system;
    this->initial_state = state(*initial_state);
    this->min_t = min_t;
    this->max_t = max_t;
    this->step_size = step_size;

    size_t num_intervals = (size_t) std::floor((max_t - min_t) / step_size);

    states.reserve(num_intervals + 1);
    times.reserve(num_intervals + 1);

    solved = false;
}

bool ODESolver::solve()
{
    if (!solved)
    {
        integrate_const(stepper, *system, initial_state, min_t, max_t + step_size / 2,
                step_size, Pusher(states, times));

        solved = true;

        return true;
    }

    return false;
}

bool ODESolver::to_graph(Graph *graph, int eq_index)
{
    if (solved)
    {
        for (size_t i = 0; i < times.size(); i++)
        {
            (*graph).add_point(times[i], states[i][eq_index]);
        }

        return true;
    }

    return false;
}

ODESolver::Pusher::Pusher(std::vector<state> &pusher_states,
        std::vector<double> &pusher_times) :
        pusher_states(pusher_states), pusher_times(pusher_times)
{
}

void ODESolver::Pusher::operator()(const state &y, double t)
{
    pusher_states.push_back(y);
    pusher_times.push_back(t);
}
