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
    if (side == WHITE) {
        opponent = BLACK;
    }
    else{
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
int Player::Heuristic(Board *board){
    int position_score, corner_score, moves_score = 0;
    // score for # of stones on board
    position_score = board->count(s) - board->count(opponent);


    // Score for # of possible moves after 
    // we make a move -> the more moves, the better
    /*
    if (board->hasMoves(s)){
        //int num_op_moves = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move move(i, j);
                if (board->checkMove(&move, s)) moves_score++;
                //if (board->checkMove(&move, opponent)) num_op_moves++;
            }
        }
        //moves_score -= num_op_moves;
    }
    
    */

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


    // -5 score for (0,1), (1,0), (0,6), (6,0), (7,1), (1,7), (7, 6), (6,7)
    // which are adjacent to corners
    /*
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
    
    */

    // +10 score for moves in corners

    if (board->get(s, 0, 0)) my_corners += 10;
    if (board->get(opponent, 0, 0)) op_corners += 10;
    if (board->get(s, 0, 7)) my_corners += 10;
    if (board->get(opponent, 0, 7)) op_corners += 10;
    if (board->get(s, 7, 0)) my_corners += 10;
    if (board->get(opponent, 7, 0)) op_corners += 10;
    if (board->get(s, 7, 7)) my_corners += 10;
    if (board->get(opponent, 7, 7)) op_corners += 10;

    corner_score = my_corners - op_corners;
    //corner_score = my_corners;

    return position_score + corner_score + moves_score;
    
}


/**
 * simple heuristic
 */
int Player::SimpleHeuristic(Board *board){
    fprintf(stderr,"Called SimpleHeuristic\n");
    int position_score;
    // score for # of stones on board
    if (s == WHITE) {
        fprintf(stderr,"Color is white\n");
    }
    fprintf(stderr,"You have: %d\n",board->count(s));
    fprintf(stderr,"Opponent has: %d\n", board->count(opponent));
    position_score = board->count(s) - board->count(opponent);
    return position_score;

}
/*
* Minimax
*/

int Player::minimax(Move *m, int depth, Side side, Board *board){
	fprintf(stderr,"Depth : %d\n", depth);

    Board *test_board = board->copy();
    test_board->doMove(m, side);

    fprintf(stderr,"Heuristic after move: %d\n", SimpleHeuristic(test_board));

    //if depth = 0 or node is a terminal node
    if (depth <= 0 || test_board->isDone()){

        //return the heuristic value of node
        
        if (testingMinimax) {
            return SimpleHeuristic(test_board);
        }
        else {
            return Heuristic(test_board);
        }

    }

    // set bestValue to an arbitrarily small value
    int bestValue = -1000000000; 
    //if maximizingPlayer (this player)
    if (side == s){
        fprintf(stderr,"Maximizing curr player\n");
        // For each child of node
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (test_board->checkMove(&test_move, opponent)){
                    bestValue = max(bestValue, minimax(&test_move, depth - 1, 
                                        opponent, test_board));
                    fprintf(stderr,"best value for me: %d\n", bestValue);
                }
            }
        }
    }
    else if (side == opponent){ //minimizing player
       fprintf(stderr,"minimizing player\n");
        bestValue = 1000000000; // arbitrarily large value
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (test_board->checkMove(&test_move, s)) {
                    bestValue = min(bestValue, minimax(&test_move, depth - 1, 
                                            s, test_board));
                    fprintf(stderr,"best value, opponent: %d\n", bestValue);
                }
            }
        }
    }
    return bestValue;
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
    if (game_board->hasMoves(s))
    {
        fprintf(stderr,"Has moves\n");
        int max_score = -1000000;

        // int x = 0;
        // int y = 0;
        //std::map<int, Move*> my_moves;
        int max_x = 0;
        int max_y = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move test_move(i, j);
                if (game_board->checkMove(&test_move, s)) {
                    fprintf(stderr, "i: %d\n", i);
                     fprintf(stderr,"j: %d\n", j);
                    /*
                    if (Heuristic(&move) > max_score) {
                        max_score = Heuristic(&move);
                        move = test_move;
                        x = i;
                        y = j;
                    }
                    */
                    int val = minimax(&test_move, 4, s, game_board);
                    fprintf(stderr,"minimax: %d\n", val);
                    if (val > max_score){
                    	max_score = val;
                    	max_x = i;
                    	max_y = j;
                    }

                    //my_moves[max_score] = new Move(i,j);
                } 
            }
        }
        //return new Move(x, y);
        return new Move(max_x, max_y);
    }

    return nullptr;
}


/**
 * @brief set the board
 */
void Player::setBoard(char data[])
{
    game_board->setBoard(data);
}