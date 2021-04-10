#include "Algorithm.hpp"

#include <limits>


Algorithm::Algorithm()
{

}

Algorithm::~Algorithm()
{

}

std::vector<Move> Algorithm::movegen(Board board, Player player)
{
    std::vector<Move> listOfPossibleMoves;
    return listOfPossibleMoves;
}

Algorithm::Result Algorithm::evalFunctOne(Board position, Player p)
{
    Algorithm::Result result;
    return result;
}

Algorithm::Result Algorithm::evalFunctTwo(Board position, Player p)
{
    Algorithm::Result result;
    return result;
}

Algorithm::Result Algorithm::evalFunctThree(Board position, Player p)
{
    Algorithm::Result result;
    return result;
}

// wrapper function that will decide which of the actual three eval functions to call
Algorithm::Result Algorithm::staticEval(Board position, Player p, int evalVersion)
{
    Algorithm::Result result;
    return result;
}

// minimax algorithm returns the position of the best move
Algorithm::Result Algorithm::minimax_a_b(Board board, int depth, Player p)
{
    Algorithm::Result result;
    return result;
}

/**
 * Alpha Beta Search
 * @author Borislav Sabotinov
 */ 
Algorithm::Result Algorithm::alphaBetaSearch(Board state)
{
    int alpha = std::numeric_limits<int>::min(); // tracks best value for max, initialized to WORST case
    int beta = std::numeric_limits<int>::max(); // tracks best value for min, initialized to WORST case

    Algorithm::Result result = maxValue(state, alpha, beta);

    return result;
}

/**
 * maxValue function determines 
 * 
 * @param Board state
 * @param int alpha
 * @param int beta
 * 
 * @return int utilityValue
 */ 
Algorithm::Result Algorithm::maxValue(Board state, int alpha, int beta)
{
    Algorithm::Result result;
    
    if (Algorithm::terminalTest(state))
    {

    }

    return result;
}

/**
 * minValue function determines
 * 
 * @param Board state
 * @param int alpha
 * @param int beta
 * 
 * @return  utilityValue
 */  
Algorithm::Result Algorithm::minValue(Board state, int alpha, int beta)
{
    Algorithm::Result result;
    return result;
}

/**
 * terminalTest function is used by alpha-beta-search to determine if 
 * the move either ends the game or leads to a leaf node
 * 
 * First checks if we're at maxDepth (i.e. at a leaf/terminal node). 
 * If yes, return true. Otherwise, next we check if the move ends the game. 
 * For example - if Red player ends up with zero pieces, they lost and the game is over. 
 * 
 * @param Board state
 * 
 * @return bool isTerminalState
 */ 
bool Algorithm::terminalTest(Board state) 
{
    return false;
}



/**
 * @author Borislav Sabotinov
 */ 
void Algorithm::setEvalVersion(int evalVersion)
{
    this->evalVersion = evalVersion;
}