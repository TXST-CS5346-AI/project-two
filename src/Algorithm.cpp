#include "Algorithm.hpp"

#include <limits>
#include <stdexcept>
#include <iostream>

Algorithm::Algorithm()
{
    this->minimaxLeafNodes = 0; 
    this->minimaxExpandedNodes = 0; 
    this->absearchLeafNodes = 0; 
    this->absearchExpandedNodes = 0; 
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
    this->minimaxLeafNodes = 0; 
    this->minimaxExpandedNodes = 0; 
    this->absearchLeafNodes = 0; 
    this->absearchExpandedNodes = 0;
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
    return board.moveGen(color);
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
    bool criticalPoint = false;
    Color opponentColor;
    if (Color::RED == color)
    {
        opponentColor = Color::BLACK;
    }
    else
    {
        opponentColor = Color::RED;
    }
    int gameTurn = callingPlayer.getNumTurns();
    int manWeight;
    int kingWeight;
    int mobilityWeight;
    int backRowDefenseWeight;
    int runAwayKingWeight;
    int playerBonus = 0;
    int opponentManWeight;
    int opponentKingWeight;
    int opponentMobilityWeight;
    int opponentBackRowDefenseWeight;
    int opponentRunAwayKingWeight;
    int opponentBonus = 0;
    int playerScore = 0;
    int opponentScore = 0;
    //=======================================================================
    // Check for the two obvious ones, a winning state or a losing state.
    // Used to simply exit out of the eval function qithout calculating
    // a bunch of extra values.
    //=======================================================================
    int mobility = (state.moveGen(color)).size();
    // This is a losing state, avoid it.
    if (!criticalPoint && mobility == 0)
    {
        opponentBonus = 6000000;
        criticalPoint = true;
    }
    int opponentMobility = (state.moveGen(opponentColor)).size();
    // This is a winning state - always try to go for it!
    // You actually cannot get to this under the current system.
    // It is caught by another check prior to this which actually returns a
    // loss for the opposite player, thus triggering the prior check for the
    // other player. This check is left in in order to show the complete
    // thought process.
    if (!criticalPoint && opponentMobility == 0)
    {
        playerBonus = 6000000;
        criticalPoint = true;
    }
    if (!criticalPoint)
    {
        //=======================================================================
        // Check for pretty bad states, such as a large difference in the number
        // of pieces between players. This typically means multiple jumps.
        // Intended to avoid fine tuning specific calculations.
        //=======================================================================
        int manCount = state.getNumRegularPieces(color);
        int kingCount = state.getNumKingPieces(color);
        int opponentManCount = state.getNumRegularPieces(opponentColor);
        int opponentKingCount = state.getNumKingPieces(opponentColor);
        int totalPieceDisparityCount = ((manCount + kingCount) - (opponentManCount + opponentKingCount));
        if (totalPieceDisparityCount >= 6)
        {
            playerBonus = 10000;
        }
        else if (totalPieceDisparityCount <= -6)
        {
            opponentBonus = 10000;
        }
        if ((kingCount - opponentKingCount) >= 4)
        {
            playerBonus = 10000;
        }
        else if (kingCount - opponentKingCount <= -4)
        {
            opponentBonus = 10000;
        }
        //=======================================================================
        // Check for pretty bad states, such as a large difference in the number
        // of pieces between players. This typically means multiple jumps.
        // Intended to avoid fine tuning specific calculations.
        //=======================================================================
        if (gameTurn <= 8)
        {
            manWeight = 30;
            kingWeight = 800;
            mobilityWeight = 4;
            backRowDefenseWeight = 2;
            runAwayKingWeight = 2;
            opponentManWeight = 30;
            opponentKingWeight = 800;
            opponentMobilityWeight = 4;
            opponentBackRowDefenseWeight = 2;
            opponentRunAwayKingWeight = 2;
        }
        else if (gameTurn <= 16)
        {
            manWeight = 20;
            kingWeight = 800;
            mobilityWeight = 3;
            backRowDefenseWeight = 2;
            runAwayKingWeight = 1;
            opponentManWeight = 20;
            opponentKingWeight = 800;
            opponentMobilityWeight = 3;
            opponentBackRowDefenseWeight = 2;
            opponentRunAwayKingWeight = 1;
        }
        else
        {
            manWeight = 20;
            kingWeight = 8000;
            mobilityWeight = 3;
            backRowDefenseWeight = 2;
            runAwayKingWeight = 1;
            opponentManWeight = 20;
            opponentKingWeight = 800;
            opponentMobilityWeight = 3;
            opponentBackRowDefenseWeight = 2;
            opponentRunAwayKingWeight = 1;
        }
        int backRowDefense = 0;
        int runAwayKing = 0;
        int opponentBackRowDefense = 0;
        int opponentRunAwayKing = 0;
        playerScore =
            (manCount * manWeight) +
            (kingCount * kingWeight) +
            (mobility * mobilityWeight) +
            (backRowDefense * backRowDefenseWeight) +
            (runAwayKing * runAwayKingWeight) +
            playerBonus;
        opponentScore =
            (opponentManCount * opponentManWeight) +
            (opponentKingCount * opponentKingWeight) +
            (opponentMobility * opponentMobilityWeight) +
            (opponentBackRowDefense * opponentBackRowDefenseWeight) +
            (runAwayKing * runAwayKingWeight) +
            opponentBonus;
    }
    return  (playerScore - opponentScore);
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
    std::string indentValue;                                                     // Tracking header in cout statements.  Shows who is player being processed
    std::string playerColor;                                                     // Used for debugging output
    int numPieces = state.getNumRegularPieces(color),                            //  how many regular pieces are on the board
        numKingsScore = state.getNumKingPieces(color),                           //  how many Kings are on the board
        numOponentPieces = state.getNumRegularPieces(switchPlayerColor(color)),  //  how many opponent pieces are on the board
        numOponentKingsScore = state.getNumKingPieces(switchPlayerColor(color)), // How many opponent Kings are on the board
        totalPieces = numPieces + numKingsScore + numOponentPieces + numOponentKingsScore,

        // This places a high value on the current player's pieces.  If a player's leads to the losing of a piece
        // a lower value for the evaluation will be returned
        preservePieces = numPieces * 50 + numKingsScore * 500,

        //  This places a value on taking opponent pieces,  if a move removes an opponent piece this results in
        // a higher return value for the move
        takePieces = 1200 - (numOponentPieces * 10 + numOponentKingsScore * 100),

        // When in end game (less than 8 pieces) this value is used to alter strategy between taking pieces
        // and preserving pieces
        endGameAdjust = 1,
        rowNumber = 0,
        playerPiece,
        opponentPiece,
        positionAdder = 0,
        moveScore = 0;

    const int KING = 2,
              MAN = 1;

    int squareValuesForRed[] = {1, 1, 1, 1,
                                2, 2, 2, 2,
                                3, 3, 3, 3,
                                5, 5, 5, 5,
                                8, 8, 8, 8,
                                11, 11, 11, 11,
                                15, 15, 15, 15,
                                50, 50, 50, 50};

    int squareValuesForBlack[] = {50, 50, 50, 50,
                                  15, 15, 15, 15,
                                  11, 11, 11, 11,
                                  8, 8, 8, 8,
                                  5, 5, 5, 5,
                                  3, 3, 3, 3,
                                  2, 2, 2, 2,
                                  1, 1, 1, 1};

    int squareValuesForKing[] = {1, 1, 1, 1,
                                 2, 3, 3, 2,
                                 3, 4, 4, 3,
                                 4, 5, 5, 4,
                                 4, 5, 5, 4,
                                 3, 4, 4, 3,
                                 2, 3, 3, 2,
                                 1, 1, 1, 1};

    if (color == Color::RED)
    {
        playerColor = Pieces::ANSII_RED_START;
        playerColor.append("RED ");
        playerColor.append(Pieces::ANSII_END);
    }
    else
        playerColor = "BLACK ";

    indentValue = playerColor;

    if (Pieces::ouputDebugData > 1)
        std::cout << indentValue << Pieces::ANSII_GREEN_COUT << " Evaluating Current Player:  Pieces-> "
                  << Pieces::ANSII_END << numPieces << " Kings-> " << numKingsScore << " Opponent Pieces-> "
                  << numOponentPieces << " Kings-> " << numOponentKingsScore << " total pieces-> "
                  << totalPieces << std::endl;

    if (state.getNumPlayerTotalPieces(color) == 0) // if the player is out of pieces this is a terminal state
    {
        if (Pieces::ouputDebugData > 1)
            std::cout << indentValue << Pieces::ANSII_RED_COUT << "TERMINAL STATE!! Returning -> "
                      << Pieces::ANSII_END << std::endl;

        return 777775; //  returning a high value.  If this is the opponent's move then the current plauer wants this
                       //  if this is the current player, this is not a move the player wants to take
    }

    // Piece preservation and the taking of opponent pieces are  global values. In other words, if no pieces are lost
    // or taken, all moves that have this result will have the same evaluation value.
    // This loop looks at each player's piece and the board generates a value that will separate certain moves from
    // the rest.
    for (int i = 1; i <= 32; ++i)
    {
        playerPiece = state.getPieceInSquare(i, color);
        if (playerPiece == KING) // piece is a King
        {
            // this encourages a king to move towards the center of the board
            positionAdder += squareValuesForKing[i - 1] * 10;
        }
        else if (playerPiece == MAN) // Regular pieces are encouraged to move towards the opponent's back row.
        {
            if (color == Color::RED)
            {
                positionAdder += squareValuesForRed[i - 1];
            }
            else if (color == Color::BLACK)
            {
                positionAdder += squareValuesForBlack[i - 1];
            }
        }

        //       opponentPiece = state.getPieceInSquare(i,switchPlayerColor(color));
    }

    moveScore = preservePieces + takePieces * endGameAdjust + positionAdder;
    if (Pieces::ouputDebugData > 1)
        std::cout << indentValue << Pieces::ANSII_GREEN_COUT << " Evaluated Move:  moveScore-> " << Pieces::ANSII_END
                  << moveScore << " preservePieces-> " << preservePieces << " takePieces -> "
                  << takePieces * endGameAdjust << " PositionAdder-> " << positionAdder << std::endl;
    if (Pieces::ouputDebugData > 1)
        std::cout << indentValue << Pieces::ANSII_YELLOW_COUT << " Checking opponent's Board " << Pieces::ANSII_END << std::endl;

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
    int squareValuesForRed[] = {  100, 100, 100, 100,
                                2, 1, 1, 2,
                                  1, 1, 1, 2,
                                2, 3, 3, 1,
                                  2, 4, 4, 5,
                                2, 5, 5, 1,
                                  3, 6, 6, 4,
                                110, 110, 110, 110};

    int squareValuesForBlack[] = {  110, 110, 110, 110,
                                  4, 6, 6, 3,
                                    1, 5, 5, 2,
                                  5, 4, 4, 2,
                                    1, 3, 3, 2,
                                  2, 1, 1, 1,
                                    2, 1, 1, 2,
                                  100, 100, 100, 100};

    int squareValuesForRedKing[] = {  3, 1, 1, 5,
                                    5, 2, 2, 3,
                                      3, 1, 1, 5,
                                    5, 1, 1, 3,
                                      3, 1, 1, 5,
                                    5, 1, 1, 1,
                                      3, 2, 2, 5,
                                    5, 1, 1, 3};

    int squareValuesForBlackKing[] = {  3, 1, 1, 5,
                                      5, 2, 2, 3,
                                        3, 1, 1, 5,
                                      5, 1, 1, 3,
                                        3, 1, 1, 5,
                                      5, 1, 1, 3,
                                        3, 2, 2, 5,
                                      5, 1, 1, 3};
    

    const int KING = 2, MAN = 1;

    int numPieces = state.getNumPlayerTotalPieces(color);
    int numKingsScore = state.getNumKingPieces(color);

    int casualtyScore = 0, captureScore = 0, positionScore = 0, playerPiece = 0, enemyPiece = 0; 

    std::vector<Board::Move> playerMoves = state.moveGen(color);
    std::vector<Board::Move> enemyMoves = state.moveGen(switchPlayerColor(color));

    Pieces playerPieces = state.getPlayerPieces(color);
    Pieces opponentPieces = state.getOpponentPieces(color); 
    Pieces *p_playerPieces = &playerPieces; 
    Pieces *p_opponentPieces = &opponentPieces; 

    std::vector<Board::Move> playerJumpsForPiece;
    std::vector<Board::Move> opponentJumpsForPiece;

    const long long redBackRowGrp = (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4);
    const long long blackBackRowGrp = (1LL << 29) | (1LL << 30) | (1LL << 31) | (1LL << 32);
    const long long sideColumnGrp = (1LL << 5) | (1LL << 13) | (1LL << 21) | (1LL << 12) | (1LL << 20) | (1LL << 28); 

    // CHECK TERMINAL STATE - Enemy is out of moves, this is a game ending move
    if (enemyMoves.size() == 0)
        return std::numeric_limits<int>::max() - 1000; // 
    
    // BEGIN ADVANCEMENT SCORE SECTION
    int advancementScore = 0;
    for (int move = 0; move < playerMoves.size(); move++)
    {
        if (color == Color::RED)
            advancementScore += squareValuesForRed[playerMoves.at(move).destinationSquare.back()];
        else if (color == Color::BLACK)
            advancementScore += squareValuesForBlack[playerMoves.at(move).destinationSquare.back()];
    }

    for (int enemyMove = 0; enemyMove < enemyMoves.size(); enemyMove++)
    {
        if (color == Color::RED)
            advancementScore -= squareValuesForBlack[enemyMoves.at(enemyMove).destinationSquare.back()];
        else if (color == Color::BLACK)
            advancementScore -= squareValuesForRed[enemyMoves.at(enemyMove).destinationSquare.back()];
    }
    // END ADVANCEMENT SCORE SECTION

    /*  BEGIN CAPTURES SECTION
        If for any given piece we can make more than one capture, we are done. 
        That is a very good move and we prioritize it. 

        Otherwise if we can make at least one capture, we give this state a high score 
        but we still want to examine others to see if they may be bettr
    */    
    for (int i = 0; i < playerMoves.size(); i++)
    {
        if (playerMoves.at(i).removalSquare.size() > 1)
        {
            if (Pieces::ouputDebugData)
                std::cout << "We can capture multiple pieces!" << std::endl;
            
            return std::numeric_limits<int>::max() - 10; // great move! 
        }
        else if (playerMoves.at(i).removalSquare.size() == 1)
        {
            return std::numeric_limits<int>::max() / 2; // great move but can we do better? 
        }
    }

    for (int j = 0; j < enemyMoves.size(); j++)
    {
        if (enemyMoves.at(j).removalSquare.size() > 1)
        {
            if (Pieces::ouputDebugData)
                std::cout << "Enemy can capture multiple pieces!" << std::endl;
            
            return 0; // bad move! 
        }
        else if (enemyMoves.at(j).removalSquare.size() == 1)
        {
            int capturedPieceType = state.getPieceInSquare(enemyMoves.at(j).removalSquare.at(0), color);
            if (capturedPieceType == KING)
                return 0; // bad move!
            else if (capturedPieceType == MAN)
            {
                int opponentDestinationSqr = enemyMoves.at(j).destinationSquare.at(0);

                if (color == Color::RED)
                {
                    // if we are RED, opponent is Black; if BLACK enemy lands on our back row, avoid at all cost
                    // we are therefore trying to minimize the chance of an enemy getting a KING
                    if ((1 << opponentDestinationSqr) & redBackRowGrp)
                        return 0; 
                    // opponent lands on their own back row; not so bad but we can't retaliate so avoid
                    else if ((1 << opponentDestinationSqr) & blackBackRowGrp)
                        return 0;
                    // opponent lands on one of the side squares, where we cannot retaliate. Avoid as well
                    else if ((1 << opponentDestinationSqr) & sideColumnGrp)
                        return 0; 
                    // finally, we exhaust negative scenarios and are where we want to be
                    else 
                    {
                        // get the square numbers around the enemy
                        int playerTopLeftSqr = opponentDestinationSqr - 4;  
                        int playerTopRightSqr = opponentDestinationSqr - 3; 
                        int playerBottomLeftSqr = opponentDestinationSqr + 4; 
                        int playerBottomRightSqr = opponentDestinationSqr + 5; 
                        
                        // get our pieces in those squares, if we have any
                        int playerTopLeftPiece = state.getPieceInSquare(playerTopLeftSqr, Color::RED);
                        int playerTopRightPiece = state.getPieceInSquare(playerTopRightSqr, Color::RED);
                        int playerBottomLeftPiece = state.getPieceInSquare(playerBottomLeftSqr, Color::RED);
                        int playerBottomRightPiece = state.getPieceInSquare(playerBottomRightSqr, Color::RED);

                        // Since we are RED, our MEN can only advance forward top to bottom. Top pieces adjacent to where the opponent 
                        // landed can be MAN or KING. Bottom pieces however must be KING, otherwise we can't move backwards
                        if (playerTopLeftPiece == MAN || playerTopRightPiece == MAN ||
                            playerTopLeftPiece == KING || playerTopRightPiece == KING ||
                            playerBottomLeftPiece == KING || playerBottomRightPiece == KING)
                        {
                            casualtyScore += 500; 
                        } 
                        else 
                            return 0; // we lost a piece and there is nothing we can do about it, avoid this state
                    }
                }
                else 
                {
                    // if we are BLACK, opponent is Red; if RED enemy lands on our back row, avoid at all cost
                    // we are therefore trying to minimize the chance of an enemy getting a KING
                    if ((1 << opponentDestinationSqr) & blackBackRowGrp)
                        return 0;
                        // opponent lands on their own back row; not so bad but we can't retaliate so avoid
                    else if ((1 << opponentDestinationSqr) & redBackRowGrp)
                        return 0;
                    // opponent lands on one of the side squares, where we cannot retaliate. Avoid as well
                    else if ((1 << opponentDestinationSqr) & sideColumnGrp)
                        return 0; 
                    // finally, we exhaust negative scenarios and are where we want to be
                    else 
                    {
                        // get the square numbers around the enemy
                        int playerTopLeftSqr = opponentDestinationSqr - 4;  
                        int playerTopRightSqr = opponentDestinationSqr - 3; 
                        int playerBottomLeftSqr = opponentDestinationSqr + 4; 
                        int playerBottomRightSqr = opponentDestinationSqr + 5; 

                        // get our pieces in those squares, if we have any
                        int playerTopLeftPiece = state.getPieceInSquare(playerTopLeftSqr, Color::RED);
                        int playerTopRightPiece = state.getPieceInSquare(playerTopRightSqr, Color::RED);
                        int playerBottomLeftPiece = state.getPieceInSquare(playerBottomLeftSqr, Color::RED);
                        int playerBottomRightPiece = state.getPieceInSquare(playerBottomRightSqr, Color::RED);

                        // Since we are BLACK, our MEN can only advance forward bottom to top. Top pieces adjacent to where the opponent 
                        // landed can KING only or we cannot move backwards. Bottom pieces can be either MAN or KING
                        if (playerTopLeftPiece == KING || playerTopRightPiece == KING ||
                            playerBottomLeftPiece == MAN || playerBottomRightPiece == MAN ||
                            playerBottomLeftPiece == KING || playerBottomRightPiece == KING)
                        {
                            casualtyScore += 500; 
                        } 
                        else 
                            return 0; // we lost a piece and there is nothing we can do about it, avoid this state
                    }
                }
            }
        }
    }

    // MAIN LOOP FOR SCORING CALCULATIONS
    for (int piece = 0; piece < 32; piece++)
    {
        // BEGIN POSITION SCORE SECTION
        if (color == Color::RED)
        {
            playerPiece = state.getPieceInSquare(piece, color); 
            if (playerPiece == MAN)
                positionScore += squareValuesForRed[piece];
            else if (playerPiece == KING)
                positionScore += squareValuesForRedKing[piece];

            enemyPiece = state.getPieceInSquare(piece, Color::BLACK);
            if (enemyPiece == MAN)
                positionScore -= squareValuesForBlack[piece];
            else if (enemyPiece == KING)
                positionScore -= squareValuesForBlackKing[piece];

        }
        else if (color == Color::BLACK)
        {
            playerPiece = state.getPieceInSquare(piece, color); 
            if (playerPiece == MAN)
                positionScore += squareValuesForBlack[piece];
            else if (playerPiece == KING)
                positionScore += squareValuesForBlackKing[piece];

            // enemyPiece = state.getPieceInSquare(piece, Color::RED);
            // if (enemyPiece == MAN)
            //     positionScore -= squareValuesForRed[piece];
            // else if (enemyPiece == KING)
            //     positionScore -= squareValuesForRedKing[piece];

        }
        // END POSITION SCORE SECTION        
    }

    int compositeScore = numPieces + numKingsScore + advancementScore + positionScore + captureScore + casualtyScore;

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
    {
        scoreOfGoodness = 1; // default and debug value.  Player takes first option everytime
        if (Pieces::ouputDebugData > 1)
            std::cout << Pieces::ANSII_GREEN_COUT << " TEST/DEBUG EVALUATION BRANCH RETURN VALUE = 1 " << Pieces::ANSII_END << std::endl;
        //throw std::runtime_error("Error: eval function # may only be 1, 2, or 3!");
    }
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
    if (currentDepth <= 0)
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
 * @param int passThresh = 9000000
 * @param int useThresh = -8000000
 * 
 * @return a Result struct, which consists of a value and a Move
 */

Algorithm::Result Algorithm::minimax_a_b(Board state, int depth, Color color, int useThresh, int passThresh)
{
    Algorithm::Result result; // Return structure for MiniMaxAB
    std::string indentValue;  // Tracking header in cout statments.  Shows level in recursion
    std::string playerColor;  // Used for debugging ouput
    Board::Move bestPath;     // best move struct - starts as a null move
    int newValue;    

    if (color == Color::RED)
    {
        playerColor = Pieces::ANSII_RED_START;
        playerColor.append("  RED ");
        playerColor.append(Pieces::ANSII_END);
    }
    else
    {
        playerColor = "BLACK ";
    }

    indentValue = playerColor;

    indentValue.append("1.");

    //   Debug code for values passed into the function
    if (Pieces::ouputDebugData > 0 && depth == 0)
        std::cout << indentValue << Pieces::ANSII_GREEN_COUT << "Entering MINIMAX_A_B Value:  " << result.value
                  << " Depth-> " << depth << " useThresh-> " << useThresh << " passThresh-> " << passThresh
                  << Pieces::ANSII_END << std::endl;

    indentValue.append("2.");

    if (deepEnough(depth)) // deep enough and Terminal could be combined.  Separated for error tracking
    {
        result.value = staticEval(state, color, evalVersion);

        if (Pieces::ouputDebugData > 0)
            std::cout << indentValue << Pieces::ANSII_RED_COUT << "Deep Enough, Move Evaluated.  Returning -> "
                      << result.value << " Bestmove destination size " << result.bestMove.destinationSquare.size() << Pieces::ANSII_END << std::endl;

        minimaxLeafNodes++; 

        // to deal with odd depths and keep it in the right order
        if (callingPlayer.getColor() != color)
            result.value = -result.value; 

        return result;
    }

    indentValue.append("3.");

    std::vector<Board::Move> successors = movegen(state, color);

    //  Current Player has no moves.  This is the equivalent to deep enough or terminal move
    if (successors.size() == 0)
    {
        result.value = staticEval(state, color, evalVersion);

        if (Pieces::ouputDebugData)
            std::cout << indentValue << Pieces::ANSII_RED_COUT << "Player has no moves.  Returning -> "
                      << result.value << Pieces::ANSII_END << std::endl;

        minimaxLeafNodes++; 
        
        // to deal with odd depths and keep it in the right order
        if (callingPlayer.getColor() != color)
            result.value = -result.value; 

        return result;
    }

    for (int successorIndex = 0; successorIndex < successors.size(); successorIndex++)
    {
        //Create a board at the current iteration of successors
        Board tmpState = state.updateBoard(successors.at(successorIndex), color);

        minimaxExpandedNodes++; 

        indentValue.append(">.");

        if (Pieces::ouputDebugData)
            std::cout << indentValue << Pieces::ANSII_BLUE_COUT << "Checking Moves:  Move #-> " << successorIndex + 1
                      << " Passed in parameters:  Start-> " << successors.at(successorIndex).startSquare << " Move to-> "
                      << successors.at(successorIndex).destinationSquare.back() << " Depth-> " << depth
                      << Pieces::ANSII_END << std::endl;
    
        // recursive call
        Result resultSucc = minimax_a_b(tmpState, depth - 1, switchPlayerColor(color), -passThresh, -useThresh);

        if (Pieces::ouputDebugData)
            std::cout << indentValue << Pieces::ANSII_GREEN_START << "Recursive Return:  Just checked-> "
                      << successors.at(successorIndex).startSquare << " -> "
                      << successors.at(successorIndex).destinationSquare.back()
                      << " New Value-> " << resultSucc.value << " Depth-> "
                      << depth << " useThresh-> " << useThresh << " passThresh-> " << passThresh
                      << Pieces::ANSII_END << std::endl;

        newValue = -resultSucc.value;

        if (newValue > passThresh) // Found the Best Move
        {
            if (Pieces::ouputDebugData > 0 && depth == this->maxDepth)
                std::cout << indentValue << Pieces::ANSII_YELLOW_COUT << "New Best Move.  From-> " << successors.at(successorIndex).startSquare
                          << " to-> " << successors.at(successorIndex).destinationSquare.back()
                          << " Change PassThresh  Old: " << passThresh
                          << " to  New: " << newValue << Pieces::ANSII_END << std::endl;

            passThresh = newValue;
            bestPath = successors.at(successorIndex);
        }

        if (passThresh >= useThresh) // Best move on the branch.  No need to look anymore
        {
            if (Pieces::ouputDebugData)
                std::cout << indentValue << Pieces::ANSII_YELLOW_COUT << "AB-CUTOFF!!  Best Move on the Branch.  PassThresh -> "
                          << Pieces::ANSII_END << passThresh << " UseThresh-> " << useThresh << " Returning " << std::endl;

            result.value = passThresh;
            result.bestMove = successors.at(successorIndex);

            return result;
        }
    }

    result.value = passThresh;
    result.bestMove = bestPath;
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
    if (Pieces::ouputDebugData)
    {
        if (callingPlayer.getColor() == Color::RED)
            std::cout << "RED ";
        else
            std::cout << "BLACK ";
        if (Pieces::ouputDebugData > 2)
            std::cout << "In alphaBetaSearch...." << std::endl;
    }

    int alpha = std::numeric_limits<int>::min(); // tracks best value for max, initialized to WORST case
    int beta = std::numeric_limits<int>::max();  // tracks best value for min, initialized to WORST case

    return maxValue(state, maxDepth, alpha, beta, callingPlayer.getColor());
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
Algorithm::Result Algorithm::maxValue(Board state, int depth, int alpha, int beta, Color color)
{
    if (Pieces::ouputDebugData)
    {
        if (color == Color::RED)
            std::cout << "\nRED ";
        else
            std::cout << "\nBLACK ";
        std::cout << "In maxValue()! Depth is " << depth << std::endl; 
    }

    Algorithm::Result result;
    Board::Move bestMove; 

    if (deepEnough(depth))
    {
        absearchLeafNodes++; 
        result.value = staticEval(state, color, evalVersion);
        return result; 
    }

    std::vector<Board::Move> listOfActions = actions(state, color);

    if (listOfActions.size() == 0)
    {
        absearchLeafNodes++; 
        result.value = staticEval(state, color, evalVersion);
        return result; 
    }

    if (Pieces::ouputDebugData)
        std::cout << "Not yet at a terminal state...." << std::endl; 

    result.value = std::numeric_limits<int>::min();

    for (int actionIndex = 0; actionIndex < listOfActions.size(); actionIndex++)
    {
        Player::printMove(listOfActions.at(actionIndex), color);
        absearchExpandedNodes++;
        Board tmpState = state.updateBoard(listOfActions.at(actionIndex), color);
        Algorithm::Result minValueResult = minValue(tmpState, depth - 1, alpha, beta, switchPlayerColor(color));
        
        if (minValueResult.value > result.value)  // Best move located
        {
            result.value = minValueResult.value; 
            bestMove = listOfActions.at(actionIndex); 
        }

        if (result.value >= beta)  // no need to examine branch
        {
            result.bestMove = listOfActions.at(actionIndex);
            return result;
        }

        alpha = std::max(alpha, result.value);
    }

    if (Pieces::ouputDebugData)
    {
        std::cout << "alpha: " << alpha << " beta: " << beta << " val: " << result.value << " move start: " << result.bestMove.startSquare << std::endl;
        for (int i = 0; i < result.bestMove.destinationSquare.size(); i++)
            std::cout << "dest: " << result.bestMove.destinationSquare.at(i) << std::endl;
    }

    result.bestMove = bestMove; 
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
Algorithm::Result Algorithm::minValue(Board state, int depth, int alpha, int beta, Color color)
{
    if (Pieces::ouputDebugData)
    {
        if (color == Color::RED)
            std::cout << "\nRED ";
        else
            std::cout << "\nBLACK ";
        std::cout << "In minValue()! Depth is " << depth << std::endl; 
    }

    Result result;
    Board::Move bestMove;

    if (deepEnough(depth))
    {
        absearchLeafNodes++; 
        result.value = staticEval(state, color, evalVersion);
        return result; 
    }

    std::vector<Board::Move> listOfActions = actions(state, color);

    if (listOfActions.size() == 0)
    {
        absearchLeafNodes++; 
        result.value = staticEval(state, color, evalVersion);
        return result; 
    }
    
    if (Pieces::ouputDebugData)
        std::cout << "Not yet at a terminal state...." << std::endl; 
    
    result.value = std::numeric_limits<int>::max();
    
    for (int actionIndex = 0; actionIndex < listOfActions.size(); actionIndex++)
    {
        Player::printMove(listOfActions.at(actionIndex), color);
        absearchExpandedNodes++;
        Board tmpState = state.updateBoard(listOfActions.at(actionIndex), color);
        Result maxValueResult = maxValue(tmpState, depth - 1, alpha, beta, switchPlayerColor(color));

        if (maxValueResult.value < result.value)  // Best move located
        {
            result.value = maxValueResult.value; 
            bestMove = listOfActions.at(actionIndex); 
        }

        if (result.value <= alpha)
        {
            result.bestMove = listOfActions.at(actionIndex);
            return result;
        }

        beta = std::min(beta, result.value);
    }

    if (Pieces::ouputDebugData)
    {
        std::cout << "alpha: " << alpha << " beta: " << beta << " val: " << result.value << " move start: " << result.bestMove.startSquare << std::endl;
        for (int i = 0; i < result.bestMove.destinationSquare.size(); i++)
            std::cout << "dest: " << result.bestMove.destinationSquare.at(i) << std::endl;
    }

    result.bestMove = bestMove; 
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
    if (Pieces::ouputDebugData)
        std::cout << "Red Moves " << redMoves.size() << "  Black Moves  " << blackMoves.size() << std::endl;

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

int Algorithm::passSign(int passThresh)
{
    if (passThresh < 0)
        return -1;
    else
        return 1;
}