#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <cstdlib>
#include <iostream>
#include <map>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {
private:
	int position_score;
	Side s;
	Board *board;
	Side opponent;
public:
    Player(Side side);
    ~Player();

	int Heuristic(Move *my_move);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
