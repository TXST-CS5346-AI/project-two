#include "Algorithm.hpp"

#include <limits>
#include <stdexcept>
#include <iostream>

Algorithm::Algorithm()
{

}

Algorithm::~Algorithm()
{

}

/**
 * Overloaded constructor for Algorithm to set internal member variables
 */ 
Algorithm::Algorithm(int evalVersion, int maxDepth, Player callingPlayer)
{
    this->evalVersion = evalVersion;
    this->maxDepth = maxDepth; 
    this->callingPlayer = callingPlayer; 
}

/**
 * movegen function gets a list of all possible moves for a player as a vector of moves.
 * This function is essentially a wrapper, which calls the movegen function in class Board. 
 * @author Borislav Sabotinov
 * 
 * @param Board board 
 * @param Player board
 * 
 * @return vector<Board::Move> listofPossibleMoves
 */ 
std::vector<Board::Move> Algorithm::movegen(Board board, Player player)
{
    return board.moveGen(player.getColor());
}

/**
 * First evaluation function 
 * @author David Torrente 
 */ 
Algorithm::Result Algorithm::evalFunctOne(Board position, Player player)
{
    Algorithm::Result result;
    return result;
}

/**
 * Second evaluation function 
 * @author Randall Henderson
 * 
 */  
Algorithm::Result Algorithm::evalFunctTwo(Board position, Player player)
{
    Algorithm::Result result;
    return result;
}

/**
 * Third evaluation function
 * @author Borislav Sabotinov
 * 
 */ 
Algorithm::Result Algorithm::evalFunctThree(Board position, Player player)
{
    Algorithm::Result result;
    return result;
}

/**
 * wrapper function that will decide which of the actual three eval functions to call
 * @author Borislav Sabotinov
 * 
 * @param Board position
 * @param Player player
 * @param int evalVersion - used to determine which of the 3 eval functions to call
 * 
 * @return a Result struct, which consists of a value and a move. 
 */ 
Algorithm::Result Algorithm::staticEval(Board position, Player player, int evalVersion)
{
    Algorithm::Result result;

    switch(evalVersion)
    {
        case 1: 
            result = evalFunctOne(position, player);
            break;
        case 2: 
            result = evalFunctTwo(position, player);
            break;
        case 3: 
            result = evalFunctThree(position, player);
            break;
        default: 
            throw std::runtime_error("Error: eval function # may only be 1, 2, or 3!");
    }

    return result;
}

/**
 * deepEnough - Basic if check, if currentDepth >= maxDepth, true; otherwise false
 * 
 * @author Randall Henderson
 * 
 */ 
bool deepEnough(int currentDepth)
{
    return false; 
}

/**
 * minimax algorithm returns the position of the best move
 * @author Randall Henderson
 * 
 * @param Board board
 * @param int depth
 * @param Player player
 * 
 * @return a Result struct, which consists of a value and a Move
 */ 
Algorithm::Result Algorithm::minimax_a_b(Board board, int depth, Player player)
{
    if (player.getColor() == Color::RED) {
        std::cout << "RED "; 
    } else {
        std::cout << "BLACK "; 
    }
    std::cout << "In minimax...." << std::endl; 
    
    Algorithm::Result result;
    return result;
}

/**
 * Alpha Beta Search
 * @author Borislav Sabotinov
 * 
 * @param Board state
 * 
 * @return a Result struct, which consists of a value and a Move
 */ 
Algorithm::Result Algorithm::alphaBetaSearch(Board state)
{
    if (callingPlayer.getColor() == Color::RED) {
        std::cout << "RED "; 
    } else {
        std::cout << "BLACK "; 
    }
    std::cout << "In alphaBetaSearch...." << std::endl;

    int alpha = std::numeric_limits<int>::min(); // tracks best value for max, initialized to WORST case
    int beta = std::numeric_limits<int>::max(); // tracks best value for min, initialized to WORST case

    return maxValue(state, alpha, beta);
}

/**
 * maxValue function determines 
 * @author Borislav Sabotinov
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
        return Algorithm::utility(state);

    result.value = std::numeric_limits<int>::min(); 

    std::vector<Board::Move> listOfActions = actions(state); 
    for (int actionIndex = 0; actionIndex < listOfActions.size(); actionIndex++)
    {
        Board tmpState = state.updateBoard(listOfActions.at(actionIndex), this->callingPlayer.getColor()); 
        Algorithm::Result tmpResult = minValue(tmpState, alpha, beta);
        result.value = std::max(result.value, tmpResult.value);

        if (result.value == tmpResult.value)
            result = tmpResult; 

        if (result.value >= beta) 
            return result; 
        
        alpha = std::max(alpha, result.value); 
    }

    return result;
}

/**
 * minValue function determines
 * @author Borislav Sabotinov
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

    if (Algorithm::terminalTest(state))
        return Algorithm::utility(state);

    result.value = std::numeric_limits<int>::max(); 

    std::vector<Board::Move> listOfActions = actions(state); 
    for (int actionIndex = 0; actionIndex < listOfActions.size(); actionIndex++)
    {
        Board tmpState = state.updateBoard(listOfActions.at(actionIndex), this->callingPlayer.getColor()); 
        Algorithm::Result tmpResult = maxValue(tmpState, alpha, beta);
        result.value = std::min(result.value, tmpResult.value);

        if (result.value == tmpResult.value)
            result = tmpResult; 

        if (result.value <= alpha) 
            return result; 
        
        beta = std::max(beta, result.value); 
    }

    return result;
}

/**
 * terminalTest function is used by alpha-beta-search to determine if 
 * the move either ends the game or leads to a leaf node.
 * @author Borislav Sabotinov
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
    bool isTerminalState = false; 
    if (state.getNumRedPieces() == 0 || state.getNumBlackPieces() == 0)
    {
        isTerminalState = true;
    } 
    else if (this->currentDepth == this->currentDepth) 
    {
        isTerminalState = true; 
    }

    return isTerminalState;
}

/**
 * Utility function to determine the best move
 * Essentially a wrapper function that calls staticEval
 * @author Borislav Sabotinov
 */
Algorithm::Result Algorithm::utility(Board state)
{
    return staticEval(state, this->callingPlayer, this->evalVersion);
} 

/**
 * actions method determines the list of possible actions, or moves, a player can make
 * Essentially a wrapper function to call Board's movegen function. 
 * @author Borislav Sabotinov
 * 
 * @param Board state
 * 
 * @return vector<Board::Move> - a list of possible moves for a player of a given color 
 */ 
std::vector<Board::Move> Algorithm::actions(Board state)
{
    return state.moveGen(this->callingPlayer.getColor());
} 


/**
 * Set the evaluation function version - 1, 2, or 3
 */ 
void Algorithm::setEvalVersion(int evalVersion)
{
    this->evalVersion = evalVersion;
}

/**
 * Set the max depth that will be used
 */ 
void Algorithm::setMaxDepth(int maxDepth) {
    this->maxDepth = maxDepth; 
}
