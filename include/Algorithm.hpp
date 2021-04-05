#include <vector> 
#include "Player.hpp"

/**
 * Header definition for class Algorithm. 
 * This class will encapsulate the algorithmic approach the AI uses to play Checkers. 
 * 
 * There are only two major approaches supported. 
 *    1) Minimax - a depth first, depth limited search procedure. From the Richard and Knight book.
 *       The minimax function has a heuristic value for leaf nodes (end nodes and nodes at the maximum permitted depth). 
 *       Intermediate nodes get their value from a child/successor leaf node.
 *    2) Alpha-Beta Pruning - a search algorithm that decreases the number of nodes evaluated by minimax in it's search tree. 
 *       We stop evaluating a possible move when at least one option is found to be worse than a previously examined move. 
 *       NOTE: It should return the SAME result as minimax, just "prunes" branches that will not affect the final outcome, 
 *             thus improving performance. 
 */ 

class Algorithm
{

private: 
    // support functions for minimax
    
    // plausible move generator, returns a list of positions that can be made by player in Position
    std::vector<Position> movegen(Position position, Player player);
    
    // if true, return the structure
    bool deepEnough();
    

public: 
    // main function for minimax alg

    // minimax search algorithm returns the position of the best move
    Position minimax(Position pos, int depth, Player p);

    // static evaluation function returns a number representing the 
    // goodness of Position from the standpoint of Player
    int statisEvaluation(Position pos, Player p);
    
};