#ifndef HANOI_TOWER_GAME_H
#define HANOI_TOWER_GAME_H

#pragma once

#include "Stack.h"
#include <vector>

class Game {
public:
    Game();
    void solve();

    // An overloaded operator<<, allowing us to print the stack via `cout<<`:
    friend std::ostream& operator<<(std::ostream & os, const Game & game);

private:
    std::vector<Stack> stacks_;

private:
    void _move(unsigned index1, unsigned index2);
    void _legalMove(unsigned index1, unsigned index2);
};

#endif //HANOI_TOWER_GAME_H
