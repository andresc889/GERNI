/*
 * ODESystemFitnessEvaluator.hpp
 *
 *  Created on: Dec 18, 2015
 *      Author: andrescj
 */

#ifndef ODESYSTEMFITNESSEVALUATOR_HPP_
#define ODESYSTEMFITNESSEVALUATOR_HPP_

/**** MY INCLUDES *************************************************************/

#include "ODE.hpp"
/* #include "ODESolver.hpp"
 #include "ODESystem.hpp" */

/**** 3RD PARTY LIBRARY INCLUDES **********************************************/

#include "ArgUtils.h"
#include "BNFGrammar.h"
#include "FitnessEvaluator.h"

/**** STANDARD INCLUDES *******************************************************/

#include <vector>

/**** END OF INCLUDES *********************************************************/

/*
 * ODESystemFitnessEvaluator
 * My class to evaluate the evaluate the fitness of an ODE system. Based on
 * the AGE source code and the examples in the AGE documentation.
 */

class ODESystemFitnessEvaluator : public AGE::FitnessEvaluator, public AGE::ArgObject
{
public:
    static const char *expr_grammar_str;

    ODESystemFitnessEvaluator(const std::string &);
    void solve_ode_and_evaluate_fitness(AGE::IndividualCollection *, unsigned int ind);

    /* Methods overridden from FitnessEvaluator */
    ODESystemFitnessEvaluator *clone() const;
    void init();
    bool isInited() const;
    void evaluate(AGE::IndividualCollection &);
    AGE::FitnessOrder order() const;
    AGE::FitnessRange range() const;
    bool setsPhenotype() const;
    bool setsDerivationInfo() const;
    bool setsDerivationTree() const;
    bool canSetDerivationTree() const;
    void setDerivationTree(bool);
    const AGE::Grammar::Ptr& grammar() const;
    unsigned maxWraps() const;
    unsigned maxTreeHeight() const;

    /* Methods "overridden" from ArgObject */
    static ODESystemFitnessEvaluator* newObjectWithArg(const std::vector<AGE::ArgValue> & argv);
    static const AGE::ArgFactorySpec& argFactorySpec();

private:
    bool inited;

    AGE::BNFGrammar *g;
    unsigned max_wraps;
    unsigned max_tree_height;

    std::string target_path;

    std::vector<double> target_times;
    std::vector<state> target_states;
    state target_initial_state;

    double min_time;
    double max_time;
};

#endif /* ODESYSTEMFITNESSEVALUATOR_HPP_ */
