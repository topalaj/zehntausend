#ifndef RL_SARSA_H_INCLUDED
#define RL_SARSA_H_INCLUDED 1

#include <memory>

#include "rl_agent.h"

namespace rl
{


class Sarsa : public Agent, private Policy
{
private:
    double  _alpha,
            _gamma;

protected:
    /// \note   State-action values might be implemented with afterstates
    ///          (by converting s,a into an afterstate type)
    virtual auto
    getEstimate(State const&, Action const&) const
        -> double = 0;  // (dynamic_)cast to instantiated type in here and ensure the environment provides the correct one

    virtual auto
    setEstimate(State const&, Action const&, double g)
        -> void = 0;

    virtual auto
    update(State const& s1, Action const& a1, double r,
           State const& s2, Action const& a2)
        -> void
    {
        double oldEstimate = getEstimate(s1,a1);
        double delta = r + _gamma*getEstimate(s2,a2) - oldEstimate;
        double newEstimate = oldEstimate + _alpha*delta;
        setEstimate(s1, a1, newEstimate);
    }

    virtual ~Sarsa() = 0;

public:
    // Performing one episode of sarsa lerning is implemented in generic terms.
    void performEpisode()
    {
        _environment->init();                                             // Initialize S
        // TODO Use auto
        std::shared_ptr<State const> s1 = _environment->cloneState();          //  .
        std::shared_ptr<Action const> a1 = this->Policy::operator()(*s1);      // Choose A from S using policy derived from Q
        while( !s1->isTerminal() )                                        // Repeat for each step of the episode
        {                                                                 //
            double r = this->Agent::_environment->takeAction(*a1);        // Take action A, observe R,
            std::shared_ptr<State const> s2 = _environment->cloneState();      //  S'
            std::shared_ptr<Action const> a2 = this->Policy::operator()(*s2);  // Choose A' from S' using policy derived from Q
            update(*s1,*a1,r,*s2,*a2);
            s1 = s2; a1 = a2;                                             // S <- S'; A <- A'
            // TODO Test destruction by smart pointers
        }                                                                 // until S is terminal

    }
};


}

#endif // RL_SARSA_H_INCLUDED
