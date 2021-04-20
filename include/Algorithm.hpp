#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector> 
#include <string> 

#include "Player.hpp"
#include "Board.hpp"




/**
 * Header definition for class Algorithm. 
 * @author multiple - David, Boris, and Randy
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

public: 
    Algorithm(); // constructor
    ~Algorithm(); // destructor 

    Algorithm(int evalVersion, int maxDepth, Player callingPlayer);

    struct Result {
        int value;
        Board::Move bestMove; 
    };

    // minimax algorithm returns the position of the best move
    Result minimax_a_b( Board board, Board::Move move, int depth, Color color, int useThresh, int passThresh );

    // AB Prune algorithm
    Result alphaBetaSearch(Board state);

    void setEvalVersion(int evalVersion); 
    void setMaxDepth(int maxDepth);

private: 
    

    int numNodesGenerated; 
    int evalVersion; 
    int currentDepth, maxDepth;
    Player callingPlayer; 
    
    // plausible move generator, returns a list of positions that can be made by player
    std::vector<Board::Move> movegen(Board board, Color color);
    
    /* static evaluation functions return a number representing the 
    * goodness of Position from the standpoint of Player
    * A helper function staticEval is used to determine which evalFunction to use
    */
    int evalFunctOne(Board state, Color color);
    int evalFunctTwo(Board state, Color color);
    int evalFunctThree(Board state, Color color);

    // wrapper function that will decide which of the actual three eval functions to call
    int staticEval(Board state, Color color, int evalVersion);

    // if true, return the structure
    bool deepEnough(int currentDepth);

    bool terminalTest(Board state, int depth); // terminal test for alpha-beta-search
    Result maxValue(Board state, Board::Move move, int depth, int &alpha, int &beta, Color color); 
    Result minValue(Board state, Board::Move move, int depth, int &alpha, int &beta, Color color);
    int utility(Board state);
    std::vector<Board::Move> actions(Board state, Color color);

    Color switchPlayerColor(Color color); 


};

#endif // !ALGORITHM_H