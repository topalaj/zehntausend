#ifndef TENK_H_INCLUDED
#define TENK_H_INCLUDED 1

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "dice.h"
#include "tenKThrow.h"
#include "tenKState.h"
#include "Afterstate.h"
#include "move.h"
#include "tenKError.h"

#define TURN_MINIMUM 350



/// ------------------------------------------------------------------------
/// class Tenthousand ------------------------------------------------------

template<unsigned int P=1>
class Tenthousand
{
public:
    // Provide some "generic" types for RL interfaces.
    typedef unsigned int Player;
    typedef TenKThrow Throw_t;
    typedef TenKState State_t;
    typedef Move Move_t;
    typedef Afterstate Afterstate_t;

    typedef typename std::vector<Move_t> MoveVector_t;
    typedef MoveVector_t ActionVector_t;
private:

    Player _player;                                                          // the player who's turn it is
    std::array<unsigned int, P> _points;                                          // points for each player
    //bool _won;

    // note: Dice<6> rolls values in [1,6]
    Dice<6> _cup;                                                            // one cup of dice for all players
    unsigned int _current;

    // helping stuff during individual turns
    std::array<bool,6> _active;                                                   // dice NOT put to the side
    std::array<unsigned int,8> _aside;                                            // how many of each were put aside from a throw 0-5: dice, 6: TOTAL, 7: ANY

    // checks for a triplet of the given figure in the ACTIVE dices position 0 and 3 indicate existence ( == NAN or not)
    std::array<int,6> _triples();  /// \tbc return rref?

    Player _incrementPlayer();
    void _abortTurn(std::string const& s = "");   // abort with 0 points for the player (throws illegal_move_error)

protected:
    void _refresh(void) &;
    void _setCurrent(unsigned int c) { _current = c; }

public:
    Throw_t thrown() const;   // note: do not put aside befostatere
    State_t state() const;

    /// \return     0 if state is terminal
    ///        else 0 if move is illegal
    ///        else the current points after the legal move
    static unsigned int legal(State_t const&, Move_t const&);
    static Afterstate_t afterstate(State_t const&, Move_t const& m);
    static auto simulate(State_t const& s, Move_t const& a) -> std::pair<double,State_t>;

    static MoveVector_t legalMoves(State_t const&);
private:
    typedef MoveVector_t (*_legalMovesFunc_t)(State_t const&);
public:
    static constexpr _legalMovesFunc_t legalActions = &legalMoves;
    MoveVector_t legalMoves() const;

    enum Figure {ONE = 0, TWO, THREE, FOUR, FIVE, SIX, TOTAL, ANY, MAX, NAN};

    void putAside(void);            // put max possible value aside
    void putAside(unsigned int d);  // put one dice aside
    void putAside(Move_t const&) &;   /// \tbc replace arg type Move with type throw. (or overload)
    Dice<6> const& roll(void);      // roll active dice
    unsigned int finishTurn(void);  // stop and save current points
    auto makeMove(Move_t const&) & -> void;

    // API for reading status
    Player const& getPlayer() const { return _player; }
    unsigned int getPoints(Player p) const { return p<=P ? _points[p] : 0; }
    unsigned int getCurrent() const { return _current; }
    unsigned int activeDice() const { return 6 - _aside[TOTAL]; }
    bool getPutAny() const { return (bool)_aside[ANY]; }

    Tenthousand();  /// \tbc don't start game in constructor, wait for a function call like start()
    ~Tenthousand() = default;

    void print() const;
};


#include "tenK.inc"


#endif