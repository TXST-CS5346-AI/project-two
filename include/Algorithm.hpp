#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector> 
#include <string> 

#include "Player.hpp"
#include "Board.hpp"

/**
 * Header definition for class Algorithm. 
 * 
 * This class will encapsulate the algorithmic approach the AI uses to play Checkers. 
 * 
 * There are only two major algorithms supported. 
 *    1) Minimax - a depth first, depth limited search procedure. From the Richard and Knight book.
 *       The minimax function has a heuristic value for leaf nodes (end nodes and nodes at the maximum permitted depth). 
 *       Intermediate nodes get their value from a child/successor leaf node.
 *    2) Alpha-Beta Pruning - a search algorithm that decreases the number of nodes evaluated by minimax in it's search tree. 
 *       We stop evaluating a possible move when at least one option is found to be worse than a previously examined move. 
 *       NOTE: It should return the SAME result as minimax, just "prunes" branches that will not affect the final outcome, 
 *             thus improving performance. 
 * 
 * Three evaluation functions will be used in conjunction with the two search algorithms. 
 * 
 */ 

class Algorithm
{

private: 
    struct Result {
        int value;
        std::string path; 
    };
    int numNodesGenerated; 
    int evalVersion; 

    // support functions for minimax
    
    // plausible move generator, returns a list of positions that can be made by player
    std::vector<int> movegen(Board board, Player player);
    
    /* static evaluation functions return a number representing the 
    * goodness of Position from the standpoint of Player
    * A helper function staticEval is used to determine which evalFunction to use
    */

    int evalFunctOne(Board position, Player p);
    int evalFunctTwo(Board position, Player p);
    int evalFunctThree(Board position, Player p);

    int staticEval(Board position, Player p, int evalVersion);

    // if true, return the structure
    bool deepEnough();
    
public: 
    Algorithm(); // constructor
    ~Algorithm(); // destructor 

    // main function for minimax alg

    // minimax algorithm returns the position of the best move
    Result minimax_a_b(Board board, int depth, Player p);

    // AB Prune algorithm
    Result abPrune(Board state);
    
    void setEvalVersion(int evalVersion); 
};

#endif // !ALGORITHM_H