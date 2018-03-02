#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    game_board = new Board();
    //board position score = (# stones you have) - (# stones your opponent has)
    opponent = WHITE;
    if (side == WHITE) {
        opponent = BLACK;
    }
    position_score = game_board->count(side) - game_board->count(opponent);

    // Set the side parameter
    s = side;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/**
 * Heuristic function takes a board position and returns a numeric "score"
 *
 */
int Player::Heuristic(Move *my_move, Board *board){
    int position_score, corner_score, moves_score;
    // score for # of stones on board
    position_score = board->count(s) - board->count(opponent);


    // Score for # of possible moves after 
    // we make a move -> the more moves, the better
    Board *test_board = board->copy();
    test_board->doMove(my_move, s);
    if (test_board->hasMoves(s)){
        int num_op_moves = 0, my_moves = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move move(i, j);
                if (test_board->checkMove(&move, s)) my_moves++;
                if (test_board->checkMove(&move, opponent)) num_op_moves++;
            }
        }
        moves_score = my_moves;
    }
    else {
        moves_score = -50;
    }


    // Score for moves adjacent to corners
    // -10 score for (1,1), (6,1), (1,6), (6, 6) diagonal to corners
    int my_corners = 0, op_corners = 0;
    if (board->get(s, 1, 1)) my_corners -= 5;
    if (board->get(opponent, 1, 1)) op_corners -= 5;
    if (board->get(s, 6, 1)) my_corners -= 5;
    if (board->get(opponent, 6, 1)) op_corners -= 5;
    if (board->get(s, 1, 6)) my_corners -= 5;
    if (board->get(opponent, 1, 6)) op_corners -= 5;
    if (board->get(s, 6, 6)) my_corners -= 5;
    if (board->get(opponent, 6, 6)) op_corners -= 5;

    // Check if our inputted move is any of (1,1), (6,1), (1,6), (6, 6)
    /*
    int X = my_move->getX();
    int Y = my_move->getY();
    if (X == 1 && Y == 1) my_corners -= 5;
    if (X == 6 && Y == 1) my_corners -= 5;
    if (X == 1 && Y == 6) my_corners -= 5;
    if (X == 6 && Y == 6) my_corners -= 5;
    */

    // -5 score for (0,1), (1,0), (0,6), (6,0), (7,1), (1,7), (7, 6), (6,7)
    // which are adjacent to corners
    if (board->get(s, 0, 1)) my_corners -= 5;
    if (board->get(opponent, 0, 1)) op_corners -= 5;
    if (board->get(s, 0, 6)) my_corners -= 5;
    if (board->get(opponent, 0, 6)) op_corners -= 5;
    if (board->get(s, 1, 0)) my_corners -= 5;
    if (board->get(opponent, 1, 0)) op_corners -= 5;
    if (board->get(s, 1, 7)) my_corners -= 5;
    if (board->get(opponent, 1, 7)) op_corners -= 5;
    if (board->get(s, 6, 0)) my_corners -= 5;
    if (board->get(opponent, 6, 0)) op_corners -= 5;
    if (board->get(s, 6, 7)) my_corners -= 5;
    if (board->get(opponent, 6, 7)) op_corners -= 5;
    if (board->get(s, 7, 1)) my_corners -= 5;
    if (board->get(opponent, 7, 1)) op_corners -= 5;
    if (board->get(s, 7, 6)) my_corners -= 5;
    if (board->get(opponent, 7, 6)) op_corners -= 5;


    // Check (0,1), (1,0), (0,6), (6,0), (7,1), (1,7), (7, 6), (6,7)
    // for our input
    /*
    if (X == 0 && Y == 1) my_corners -= 5;
    if (X == 1 && Y == 0) my_corners -= 5;
    if (X == 0 && Y == 6) my_corners -= 5;
    if (X == 6 && Y == 0) my_corners -= 5;
    if (X == 7 && Y == 1) my_corners -= 5;
    if (X == 1 && Y == 7) my_corners -= 5;
    if (X == 7 && Y == 6) my_corners -= 5;
    if (X == 6 && Y == 7) my_corners -= 5;
    */


    // Check (0,1), (1,0), (0,6), (6,0), (7,1), (1,7), (7, 6), (6,7)
    // for our input
    /*
    if (X == 0 && Y == 1) my_corners -= 3;
    if (X == 1 && Y == 0) my_corners -= 3;
    if (X == 0 && Y == 6) my_corners -= 3;
    if (X == 6 && Y == 0) my_corners -= 3;
    if (X == 7 && Y == 1) my_corners -= 3;
    if (X == 1 && Y == 7) my_corners -= 3;
    if (X == 7 && Y == 6) my_corners -= 3;
    if (X == 6 && Y == 7) my_corners -= 3;
    */

    // +7 score for moves in corners
    if (board->get(s, 0, 0)) my_corners += 10;
    if (board->get(opponent, 0, 0)) op_corners += 10;
    if (board->get(s, 0, 7)) my_corners += 10;
    if (board->get(opponent, 0, 7)) op_corners += 10;
    if (board->get(s, 7, 0)) my_corners += 10;
    if (board->get(opponent, 7, 0)) op_corners += 10;
    if (board->get(s, 7, 7)) my_corners += 10;
    if (board->get(opponent, 7, 7)) op_corners += 10;



    // Check our input for corner moves
    /*
    if (X == 0 && Y == 0) my_corners += 10;
    if (X == 0 && Y == 7) my_corners += 10;
    if (X == 7 && Y == 0) my_corners += 10;
    if (X == 7 && Y == 7) my_corners += 10;
    */

    corner_score = my_corners - op_corners;
    return position_score + 10 * corner_score + 20 * moves_score;
    
}

/*
* Minimax
*/

int Player::minimax(Move *m, int depth, Side side){
    Board *test_board = board->copy();
    test_board->doMove(m, side);
    //if depth = 0 or node is a terminal node
    if (depth == 0 || !test_board->hasMoves(side)){
        //return the heuristic value of node
        return Heuristic(m, &test_board);
    }
    //if maximizingPlayer (this player)
    if (side == this->side){
        int bestValue = -10000;
        // For each child of node
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (test_board->checkMove(&test_move, this->side)){
                    int val = minimax(&test_move, depth - 1, opponent);
                    bestValue = max(bestValue, val);
                    return bestValue;
                }
            }
        }
    }
    else{ //minimizing player
        int bestValue = 10000;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (test_board->checkMove(&test_move, opponent)){
                    int val = minimax(&test_move, depth - 1, this->side);
                    bestValue = min(bestValue, val);
                    return bestValue;
                }
            }
        }
    }
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    // Pick a valid move based on the heuristic function
    // Iterate through all possible moves and choose
    // the move with the max heuristic
    if (board->hasMoves(s))
    {
        int max_score = -10000;
        Move move(0, 0);
        int x = 0;
        int y = 0;
        std::map<int, *Move> my_moves;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (board->checkMove(&move, s)) {
                    /*
                    if (Heuristic(&move) > max_score) {
                        max_score = Heuristic(&move);
                        move = test_move;
                        x = i;
                        y = j;
                    }
                    */
                    max_score = max(max_score, minimax(&test_move, 3, s));

                    my_moves[max_score] = new Move(i,j);
                } 
            }
        }
        //return new Move(x, y);
        return my_moves[max_score];
    }

    return nullptr;
}
