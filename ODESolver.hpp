/*
 * ODESolver.h
 *
 *  Created on: Dec 15, 2015
 *      Author: andrescj
 */

#ifndef ODESOLVER_HP_
#define ODESOLVER_HP_

/**** MY INCLUDES *************************************************************/

#include "Graph.hpp"
#include "ODE.hpp"
#include "ODESystem.hpp"

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>

/**** STANDARD INCLUDES *******************************************************/

#include <vector>

/**** END OF INCLUDES *********************************************************/

/*
 * ODESolver
 * My class to wrap the Boost odeint library to solve ODEs. Based on
 * http://www.boost.org/doc/libs/1_59_0/libs/numeric/odeint/doc/html/
 */

class ODESolver
{
public:
    ODESolver(const ODESystem *, const state *, double, double, double);
    bool solve();
    bool to_graph(Graph *, int);

    struct Pusher
    {
        std::vector<state> &pusher_states;
        std::vector<double> &pusher_times;

        Pusher(std::vector<state> &, std::vector<double> &);
        void operator()(const state &, double);
    };

private:
    const ODESystem *system;
    state initial_state;
    std::vector<state> states;
    std::vector<double> times;

    double min_t;
    double max_t;
    double step_size;

    boost::numeric::odeint::runge_kutta4<state> stepper;

    bool solved;
};

#endif /* ODESOLVER_HP_ */
