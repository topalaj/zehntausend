#ifndef TENK_ELIG_H_INCLUDED
#define TENK_ELIG_H_INCLUDED

#include "tenthousand.h"
#include "rl_eligibility.h"


class TenKElig : public rl::Eligibility
{
private:
    using Game_t = Tenthousand<1>;
    using Traced_t = Game_t::Afterstate_t;
    using Map_t = std::unordered_map<Traced_t, Elig_t>;
public:
    double _gamma;
    double _lambda;
    Map_t _elig;

    /// ------------------------------------------------------------------------
    /// usage
    auto begin() -> decltype( _elig.begin() );
    auto end() -> decltype( _elig.end() );

    /// ------------------------------------------------------------------------
    /// req | rl::Eligibility
    virtual auto stimulate(rl::State const& afterstate) -> void;
    virtual auto decay() -> void;
    virtual auto visit(rl::State const& afterstate) -> void;
    virtual auto init() -> void;

    /// ------------------------------------------------------------------------
    /// ctors/dtor

    TenKElig(double gam, double lam);
    virtual ~TenKElig() = default;
};

#endif // TENK_ELIG_H_INCLUDED
