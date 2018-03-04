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
	Board *game_board;
	Side opponent;
	int steps;
public:
    Player(Side side);
    ~Player();

	int Heuristic(Board *board, Side side);
	int minimax(Move *m, int depth, Side side, Board *board);
    Move *doMove(Move *opponentsMove, int msLeft);
	int SimpleHeuristic(Board *board);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

    void setBoard(char data[]);
};

#endif
