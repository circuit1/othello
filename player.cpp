#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    // We start out with zero steps
    steps = 0;

    game_board = new Board();

    // set our opponent's side
    if (side == WHITE) {
        opponent = BLACK;
    }
    else {
    	opponent = WHITE;
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
int Player::Heuristic(Board *board, Side side){
    int position_score, corner_score, moves_score = 0;
    // score for # of stones on board
    position_score = 2*board->count(s) - 2*board->count(opponent);


    // Score for # of possible moves after 
    // we make a move -> the more moves, the better
    
    if (board->hasMoves(side) && steps <= 24){

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move move(i, j);
                if (board->checkMove(&move, side)) moves_score += 3;
            }
        }
        
        if (side == opponent){
            moves_score = -moves_score;
        }
        
    }
    

    // Score for moves adjacent to corners
    // -10 score for (1,1), (6,1), (1,6), (6, 6) diagonal to corners

    int my_corners = 0, op_corners = 0;
    if (steps >= 18){
        if (board->get(s, 1, 1)) my_corners -= 5;
        if (board->get(opponent, 1, 1)) op_corners -= 5;
        if (board->get(s, 6, 1)) my_corners -= 5;
        if (board->get(opponent, 6, 1)) op_corners -= 5;
        if (board->get(s, 1, 6)) my_corners -= 5;
        if (board->get(opponent, 1, 6)) op_corners -= 5;
        if (board->get(s, 6, 6)) my_corners -= 5;
        if (board->get(opponent, 6, 6)) op_corners -= 5;


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
        
    }
    
    corner_score = my_corners - op_corners;

    return position_score + corner_score + moves_score;
    
}


/**
 * simple heuristic
 */
int Player::SimpleHeuristic(Board *board){
    int position_score;
    // score for # of stones on board
    position_score = board->count(s) - board->count(opponent);
    return position_score;

}

/*
* Minimax
*/

int Player::minimax(Move *m, int depth, Side side, Board *board){

    Board *test_board = board->copy();
    test_board->doMove(m, side);


    //if depth = 0 or node is a terminal node
    if (depth <= 0 || test_board->isDone()){

        //return the heuristic value of node
        
        if (testingMinimax) {
            return SimpleHeuristic(test_board);
        }
        else {
            return Heuristic(test_board, side);
        }

    }

    // set bestValue to an arbitrarily small value
    int bestValue = -1000000000; 
    //if maximizingPlayer (this player)
    if (side == s){
        // For each child of node
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (test_board->checkMove(&test_move, opponent)){
                    bestValue = max(bestValue, minimax(&test_move, depth - 1, 
                                        opponent, test_board));
                }
            }
        }
    }
    else if (side == opponent){ //minimizing player
        bestValue = 1000000000; // arbitrarily large value
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (test_board->checkMove(&test_move, s)) {
                    bestValue = min(bestValue, minimax(&test_move, depth - 1, 
                                            s, test_board));

                }
            }
        }
    }
    return bestValue;
}


/* 
*Implementation of minimax with alpha beta pruning
*/
int Player::alpha_beta(Move *m, int depth, int alpha, 
                        int beta, Side side, Board *board){
    Board *test_board = board->copy();
    test_board->doMove(m, side);

    //if depth = 0 or node is a terminal node
    if (depth <= 0 || test_board->isDone()){

        //return the heuristic value of node
        return Heuristic(test_board, side);
    }
    if (side == s){
        int bestValue = -1000000000;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (test_board->checkMove(&test_move, opponent)){
                    bestValue = max(bestValue, alpha_beta(&test_move, depth - 1, alpha, beta,
                                        opponent, test_board));
                    alpha = max(alpha, bestValue);
                    if (beta <= alpha){
                        break;
                    }
                }
           
            }
        }
        return bestValue;
    }
    else if (side == opponent){ //minimizing player
        int bestValue = 1000000000; // arbitrarily large value
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (test_board->checkMove(&test_move, s)) {
                    bestValue = min(bestValue, alpha_beta(&test_move, depth - 1, 
                                                    alpha, beta, s, test_board));
                    beta = min(beta, bestValue);
                    if (beta <= alpha){
                        break;
                    }

                }
            }
        }
        return bestValue;
    }
    return 0;
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
    steps +=2 ;

    if (game_board->hasMoves(s))
    {
        int max_score = -1000000;

        int max_x = 0;
        int max_y = 0;
        int depth = 4;
        while(msLeft > 0) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (game_board->checkMove(&test_move, s)) {

                    //int val = minimax(&test_move, 4, s, game_board);
                
                       int val = alpha_beta(&test_move, depth, -100000000, 
                                            +100000000, s, game_board);
                       if (val > max_score){
                            max_score = val;
                            max_x = i;
                            max_y = j;
                        }

                    }

                } 
            }
            depth ++;
        }
        return new Move(max_x, max_y);
    }

    return nullptr;
}


/**
 * @brief set the board given a arrangement
 
 */
void Player::setBoard(char data[])
{
    game_board->setBoard(data);
}