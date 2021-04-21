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
std::vector<Board::Move> Algorithm::movegen(Board board, Color color)
{
    return board.moveGen( color );
}

/**
 * First evaluation function 
 * @author David Torrente 
 * 
 * @param Board State
 * @param Color color
 * 
 * @return an integer score of how good we think the state is
 */
int Algorithm::evalFunctOne(Board state, Color color)
{
    return 1; 
}

/**
 * Second evaluation function 
 * @author Randall Henderson
 * 
 * @param Board State
 * @param Color color
 * 
 * @return an integer score of how good we think the state is
 */
int Algorithm::evalFunctTwo(Board state, Color color)
{
    std::string indentValue;  // Tracking header in cout statments.  Shows level in recursion
    std::string playerColor; // Used for debugging ouput
    int numPieces = state.getNumRegularPieces(color);
    int numKingsScore = state.getNumKingPieces(color);
    int numOponentPieces = state.getNumRegularPieces(switchPlayerColor(color));
    int numOponentKingsScore = state.getNumKingPieces(switchPlayerColor(color));
    int numOponentTaken = 12 -  numOponentPieces;
    int totalPieces = numPieces + numKingsScore + numOponentPieces + numOponentKingsScore;
    int preservePieces = numPieces * 50 + numKingsScore * 500;
    int takePieces = 1200 - ( numPieces * 10 + numKingsScore * 100 );
    int endGameAdjust = 1;
    int moveScore;
    int boardValue = 0;


    if (color == Color::RED)
    {
        playerColor = Pieces::ANSII_RED_START;
        playerColor.append("RED ");
        playerColor.append(Pieces::ANSII_END);
    }
    else
        playerColor = "BLACK ";

    indentValue = playerColor;

    int squareValuesForRed[] = { 100, 1, 100, 1,
                                 2, 1, 1, 2,
                                 1, 1, 1, 2,
                                 2, 3, 3, 1,
                                 2, 3, 3, 4,
                                 2, 3, 3, 1,
                                 3, 3, 3, 4,
                                 10, 10, 10, 10 };

    int squareValuesForBlack[] = { 10, 10, 10, 10,
                                   4, 3, 3, 3,
                                   1, 3, 3, 2,
                                   4, 3, 3, 2,
                                   1, 3, 3, 2,
                                   2, 1, 1, 1,
                                   2, 1, 1, 2,
                                   1, 100, 1, 100 };

    std::cout << indentValue << " Evaluating Move:  Player Pieces-> " << numPieces << " Kings-> "
              << numKingsScore << " Opponent Pieces-> " << numOponentPieces  << " Kings-> "
              << numOponentKingsScore << " total pieces-> " << totalPieces << std::endl;

    if ( state.getNumPlayerTotalPieces(color) == 0 )
    {
        std::cout << "Evalutaion says terminal move!"  << std::endl;
        return 777775;
    }

    if ( totalPieces <= 8 )
        endGameAdjust = 5;

    moveScore = preservePieces + takePieces * endGameAdjust + boardValue;

    std::cout << indentValue << " Evaluated Move:  moveScore-> " << moveScore << " preservePieces-> "
              << preservePieces << " takePieces -> "
              << takePieces * endGameAdjust << " boardValue-> " << boardValue << std::endl;

    return moveScore; //  Preserve pieces on the board,  this should make moves that loses a piece unfavorable
}

/**
 * Third evaluation function
 * @author Borislav Sabotinov
 * 
 * @param Board State
 * @param Color color
 * 
 * @return an integer score of how good we think the state is
 */
int Algorithm::evalFunctThree(Board state, Color color)
{
    int squareValuesForRed[] = { 100, 1, 100, 1, 
                                 2, 1, 1, 2, 
                                 1, 1, 1, 2, 
                                 2, 3, 3, 1, 
                                 2, 3, 3, 4, 
                                 2, 3, 3, 1, 
                                 3, 3, 3, 4, 
                                 10, 10, 10, 10 };

    int squareValuesForBlack[] = { 10, 10, 10, 10, 
                                   4, 3, 3, 3, 
                                   1, 3, 3, 2, 
                                   4, 3, 3, 2, 
                                   1, 3, 3, 2, 
                                   2, 1, 1, 1, 
                                   2, 1, 1, 2, 
                                   1, 100, 1, 100 };
    
    int numPieces = state.getNumPlayerTotalPieces(color);
    int numKingsScore = state.getNumKingPieces(color);


    std::vector<Board::Move> moves = state.moveGen(color);

    int advancementScore = 0; 
    for (int move = 0; move < moves.size(); move++) 
    {
        if (color == Color::RED)
        {
            advancementScore += squareValuesForRed[moves.at(move).destinationSquare.back()];
        }
        else if (color == Color::BLACK)
        {
            advancementScore += squareValuesForBlack[moves.at(move).destinationSquare.back()];
        }
    }

    int positionScore = 0; 
    Pieces playerPieces = state.getPlayerPieces(color);
    
    for (int piece = 0; piece < 32; piece++)
    {
        if (color == Color::RED)
        {
            int pieceBit = (playerPieces.pieces >> piece) & 1; 
            if (pieceBit == 1)
            {
                positionScore += squareValuesForRed[piece];
            }
            
        }
        else if (color == Color::BLACK)
        {
            int pieceBit = (playerPieces.pieces >> piece) & 1; 
            if (pieceBit == 1)
            {
                positionScore += squareValuesForBlack[piece];
            }
        }
        
    }

    int compositeScore = numPieces + numKingsScore + advancementScore + positionScore;

    return compositeScore; 
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
int Algorithm::staticEval(Board state, Color color, int evalVersion)
{
    int scoreOfGoodness;

    switch (evalVersion)
    {
    case 1:
        scoreOfGoodness = evalFunctOne(state, color);
        break;
    case 2:
        scoreOfGoodness = evalFunctTwo(state, color);
        break;
    case 3:
        scoreOfGoodness = evalFunctThree(state, color);
        break;
    default:
        throw std::runtime_error("Error: eval function # may only be 1, 2, or 3!");
    }

    return scoreOfGoodness;
}

/**
 * deepEnough - Basic if check, if currentDepth >= maxDepth, true; otherwise false
 * 
 * @author Randall Henderson
 * 
 */
bool Algorithm::deepEnough(int currentDepth)
{
    if ( currentDepth <= 0 )
        return true;
    else
        return false;
}

/**
 * minimax algorithm returns the position of the best move
 * @author Randall Henderson
 * 
 * @param Board board
 * @param int depth
 * @param Color color
 * @param int passThresh = INT32_MAX
 * @param int useThresh = INT32_MIN
 * 
 * @return a Result struct, which consists of a value and a Move
 */

Algorithm::Result Algorithm::minimax_a_b( Board state, Board::Move move, int depth, Color color, int useThresh, int passThresh )
{

    Algorithm::Result result;  // Return structure for MiniMaxAB
    std::string indentValue;  // Tracking header in cout statments.  Shows level in recursion
    std::string playerColor; // Used for debugging ouput
    Board::Move bestMove = move;  //  This is the best move the algorithm can find

    if (color == Color::RED)
    {
        playerColor = Pieces::ANSII_RED_START;
        playerColor.append("RED ");
        playerColor.append(Pieces::ANSII_END);
    }

    else
        playerColor = "BLACK ";

    indentValue = playerColor;
    indentValue.append( "1." );

    std::cout << indentValue << Pieces::ANSII_GREEN_COUT   << "Entering MINIMAX_A_B Values are:  Depth-> " << depth
              << " passThresh-> " << passThresh << " useThresh-> " << useThresh << Pieces::ANSII_END << std::endl;

    indentValue.append( "2." );

    if ( deepEnough(depth) )  // deep enough and Terminal could be combined.  Separated for error tracking
    {
        //std::cout << indentValue << "Deep Enough..  Evaluating moves. " << std::endl;
        result.value = staticEval(state, color, evalVersion);
        result.bestMove = move;

        std::cout << indentValue << Pieces::ANSII_RED_COUT << "Deep Enough, Move Evaluated.  Returning -> " << result.value
                  << " Move:  Start-> "  << result.bestMove.startSquare << " Move to-> "
                  << result.bestMove.destinationSquare.at(0)
                  << " Depth-> " << depth << Pieces::ANSII_END << std::endl;

        return result;
    }
    /*
    else if ( Algorithm::terminalTest(state, depth) )  // checking if the move is a terminal move
    {
        std::cout << indentValue << playerColor << "Board in Terminal State. " << std::endl;
        result.value = staticEval(state, color, evalVersion);
        result.bestMove = move;

        std::cout << indentValue << ANSII_RED_START << playerColor << "Returning -> " << result.value << " Move:  Start-> "
                  << result.bestMove.startSquare << " Move to-> " << result.bestMove.destinationSquare.at(0)
                  << " Depth-> " << depth << ANSII_END << std::endl;

        return result;
    }
     */
    //else
    //    std::cout << indentValue << playerColor << "Not in Terminal State.  Checking moves" << std::endl;

    // Call MoveGen Function
    // Loads the list of moves into the vector successors
    std::vector<Board::Move> successors = movegen(state, color);

    // Player has no moves  this is a terminal state
    if (successors.size() == 0)
    {
        //std::cout << indentValue << "Player has no moves" << std::endl;
        result.value = 777777;  // if this is current player - TAKE THIS MOVE else AVOID AT ALL COSTS
        result.bestMove = move;

        std::cout << indentValue << Pieces::ANSII_RED_COUT << "TERMINAL STATE!! Returning -> " << result.value
                  << " Move:  Start-> " << result.bestMove.startSquare << " Move to-> "
                  << result.bestMove.destinationSquare.at(0) << " Depth-> " << depth << Pieces::ANSII_END << std::endl;

        return result;
    }
    indentValue.append( "3." );

    for ( int successorIndex = 0; successorIndex < successors.size(); successorIndex++ )
    {
        //Create a board at the current iteration of successors
        Board tmpState = state.updateBoard(successors.at(successorIndex), color );
        result.bestMove = successors.at(successorIndex);

        indentValue.append( ">." );

        std::cout << indentValue << Pieces::ANSII_BLUE_COUT << "Checking Moves:  Move #-> " << successorIndex + 1
                  << " Passed in parameters:  Start-> " << successors.at(successorIndex).startSquare << " Move to-> "
                  << successors.at(successorIndex).destinationSquare.at(0) << " Depth-> " << depth
                  << Pieces::ANSII_END << std::endl;

        //std::cout << indentValue << " passThresh-> " << passThresh << " useThresh-> " << useThresh  << std::endl;

        // recursive call
        result = minimax_a_b( tmpState, successors.at(successorIndex), depth-1,
                              switchPlayerColor( color ), -passThresh, -useThresh );

        // Keeps the IndentValue from growing too big.
        indentValue.erase( indentValue.size()-2, 2 );

        int newValue = -result.value;

        //std::cout << indentValue << " NEW_VALUE -> " << newValue << std::endl;

        if ( newValue > passThresh )  // Found the Best Move
        {
            std::cout << indentValue << Pieces::ANSII_YELLOW_COUT << "New Best Move.  Change PassThresh  Old: " << passThresh
                      << " to  New: " << newValue << Pieces::ANSII_END << std::endl;

            passThresh = newValue;
            bestMove = successors.at(successorIndex);
        }

        if ( passThresh >= useThresh )  // Best move on the branch.  No need to look anymore
        {
            std::cout << indentValue << "Best Move on the Branch.  PassThresh > UseThresh  Pass-> "
                      << passThresh << " Use-> " << useThresh << " Returning " << std::endl;

            result.value = passThresh;
            result.bestMove = successors.at(successorIndex);

            std::cout << indentValue << Pieces::ANSII_RED_COUT  << "Returning -> " << result.value << " Move:  Start-> "
                      << result.bestMove.startSquare << " Move to-> " << result.bestMove.destinationSquare.at(0)
                      << " Depth-> " << depth << Pieces::ANSII_END << std::endl;

            return result;

        }
    }

        // set result to the best move found
        result.value = passThresh;
        result.bestMove = bestMove;


    std::cout << indentValue << Pieces::ANSII_BLUE_COUT << "Returning -> " << result.value << " Move:  Start-> "
              << result.bestMove.startSquare << " Move to-> " << result.bestMove.destinationSquare.at(0)
              << " Depth-> " << depth << Pieces::ANSII_END << std::endl;

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
    if (callingPlayer.getColor() == Color::RED)
        std::cout << "RED ";
    else
        std::cout << "BLACK ";
    
    std::cout << "In alphaBetaSearch...." << std::endl;

    int alpha = std::numeric_limits<int>::min(); // tracks best value for max, initialized to WORST case
    int beta = std::numeric_limits<int>::max();  // tracks best value for min, initialized to WORST case

    Board::Move move; // empty Move for first call to max-val

    return maxValue(state, move, maxDepth, alpha, beta, callingPlayer.getColor());
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
Algorithm::Result Algorithm::maxValue(Board state, Board::Move move, int depth, int &alpha, int &beta, Color color)
{
    if (color == Color::RED)
        std::cout << "\n\nRED ";
    else
        std::cout << "\n\nBLACK ";
    std::cout << "In maxValue()! Depth is " << depth << std::endl; 

    Algorithm::Result result;

    if (Algorithm::terminalTest(state, depth))
    {
        std::cout << "At terminal state!" << std::endl; 
        result.value = std::numeric_limits<int>::max();
        //result.value = 1; 
        result.bestMove = move; 
        return result; 
    } 
    else if (depth <= 0) 
    {
        std::cout << "at depth" << std::endl; 
        result.value = staticEval(state, color, this->evalVersion);
        std::cout << "Static eval gives us a score of " << result.value << std::endl; 
        result.bestMove = move;
        return result; 
    }

    std::cout << "Not yet at a terminal state...." << std::endl; 

    result.value = std::numeric_limits<int>::min();

    std::vector<Board::Move> listOfActions = actions(state, color);
    for (int actionIndex = 0; actionIndex < listOfActions.size(); actionIndex++)
    {
        Player::printMove(listOfActions.at(actionIndex), color);

        Board tmpState = state.updateBoard(listOfActions.at(actionIndex), color);
        Algorithm::Result tmpResult = minValue(tmpState, listOfActions.at(actionIndex), depth - 1, alpha, beta, switchPlayerColor(color));
        
        //result.value = std::max(result.value, tmpResult.value);

        if (result.value > tmpResult.value)
        {
            // do nothing 
        } else {
            result = tmpResult; 
        }

        if (result.value >= beta)
        {
            result.bestMove = listOfActions.at(actionIndex);
            return result;
        }

        alpha = std::max(alpha, result.value);
    }

    std::cout << "alpha: " << alpha << " beta: " << beta << " val: " << result.value << " move start: " << result.bestMove.startSquare << std::endl;

    for (int i = 0; i < result.bestMove.destinationSquare.size(); i++)
        std::cout << "dest: " << result.bestMove.destinationSquare.at(i) << std::endl;

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
Algorithm::Result Algorithm::minValue(Board state, Board::Move move, int depth, int &alpha, int &beta, Color color)
{
    if (color == Color::RED)
        std::cout << "\n\nRED ";
    else
        std::cout << "\n\nBLACK ";
    std::cout << "In minValue()! Depth is " << depth << std::endl; 

    Result result;

    if (Algorithm::terminalTest(state, depth))
    {
        std::cout << "At terminal state!" << std::endl; 
        result.value = std::numeric_limits<int>::max();
        //result.value = 1;
        result.bestMove = move; 
        return result; 
    }
    else if (depth <= 0) 
    {
        std::cout << "at depth" << std::endl; 
        result.value = staticEval(state, color, this->evalVersion);
        std::cout << "Static eval gives us a score of " << result.value << std::endl; 
        result.bestMove = move;
        return result; 
    }
    
    std::cout << "Not yet at a terminal state...." << std::endl; 

    result.value = std::numeric_limits<int>::max();

    std::vector<Board::Move> listOfActions = actions(state, color);
    for (int actionIndex = 0; actionIndex < listOfActions.size(); actionIndex++)
    {
        Player::printMove(listOfActions.at(actionIndex), color);

        Board tmpState = state.updateBoard(listOfActions.at(actionIndex), color);
        Result tmpResult = maxValue(tmpState, listOfActions.at(actionIndex), depth - 1, alpha, beta, switchPlayerColor(color));
        
        //result.value = std::min(result.value, tmpResult.value);

        if (result.value < tmpResult.value)
        {
            // do nothing
        } else {
            result = tmpResult; 
        }

        if (result.value <= alpha)
        {
            result.bestMove = listOfActions.at(actionIndex);
            return result;
        }

        beta = std::min(beta, result.value);
    }

    std::cout << "alpha: " << alpha << " beta: " << beta << " val: " << result.value << " move start: " << result.bestMove.startSquare << std::endl;
    for (int i = 0; i < result.bestMove.destinationSquare.size(); i++)
        std::cout << "dest: " << result.bestMove.destinationSquare.at(i) << std::endl;

    return result;
}

Color Algorithm::switchPlayerColor(Color color)
{
    if (color == Color::RED)
        return Color::BLACK;
    else 
        return Color::RED;
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
bool Algorithm::terminalTest(Board state, int depth)
{
    bool isTerminalState = false;
    std::vector<Board::Move> redMoves = state.moveGen(Color::RED);
    std::vector<Board::Move> blackMoves = state.moveGen(Color::BLACK);

    std::cout <<"Red Moves " << redMoves.size() << "  Balck Moves  " << blackMoves.size() << std::endl;

    if (redMoves.size() == 0 || blackMoves.size() == 0)
        isTerminalState = true; 

    return isTerminalState;
}

/**
 * Utility function to determine the best move
 * Essentially a wrapper function that calls staticEval
 * @author Borislav Sabotinov
 */
int Algorithm::utility(Board state)
{
    //return staticEval(state, this->callingPlayer, this->evalVersion);
    return 1;
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
std::vector<Board::Move> Algorithm::actions(Board state, Color color)
{
    return state.moveGen(color);
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
void Algorithm::setMaxDepth(int maxDepth)
{
    this->maxDepth = maxDepth;
}