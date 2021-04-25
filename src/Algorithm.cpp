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
    int backRowDefense = 0;
    int runAwayKing = 0;
    int opponentBackRowDefense = 0;
    int opponentRunAwayKing = 0;
    int gameTurn = callingPlayer.getNumTurns();
    int manWeight;
    int kingWeight;
    int mobilityWeight;
    int backRowDefenseWeight;
    int runAwayKingWeight;
    int playerScore = 0;
    int playerBonus = 0;
    int opponentManWeight;
    int opponentKingWeight;
    int opponentMobilityWeight;
    int opponentBackRowDefenseWeight;
    int opponentRunAwayKingWeight;
    int opponentBonus = 0;
    int opponentScore = 0;
    //=======================================================================
    // Set up a few items based on the player color. Opponent and player
    // defense row.
    if (Color::RED == color)
    {
        opponentColor = Color::BLACK;
        // Calculate back row defense
        for (int redIter = 1; redIter <= 4; redIter++)
        {
            if (state.getPieceInSquare(redIter, color) == 1)
            {
                backRowDefense++;
            }
        }
    }
    else
    {
        opponentColor = Color::RED;
        for (int blackIter = 29; blackIter <= 32; blackIter++)
        {
            if (state.getPieceInSquare(blackIter, color) == 1)
            {
                backRowDefense++;
            }
        }
    }
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
        // Set some values based on the current turn to adjust play styles.
        //=======================================================================
        if (gameTurn <= 80)
        {
            manWeight = 40;
            kingWeight = 90;
            mobilityWeight = 4;
            backRowDefenseWeight = 2;
            runAwayKingWeight = 2;
            opponentManWeight = 6;
            opponentKingWeight = 16;
            opponentMobilityWeight = 4;
            opponentBackRowDefenseWeight = 2;
            opponentRunAwayKingWeight = 2;
        }
        else if (gameTurn <= 32)
        {
            manWeight = 20;
            kingWeight = 80;
            mobilityWeight = 3;
            backRowDefenseWeight = 2;
            runAwayKingWeight = 1;
            opponentManWeight = 20;
            opponentKingWeight = 80;
            opponentMobilityWeight = 3;
            opponentBackRowDefenseWeight = 2;
            opponentRunAwayKingWeight = 1;
        }
        else
        {
            manWeight = 20;
            kingWeight = 80;
            mobilityWeight = 3;
            backRowDefenseWeight = 2;
            runAwayKingWeight = 1;
            opponentManWeight = 20;
            opponentKingWeight = 80;
            opponentMobilityWeight = 1;
            opponentBackRowDefenseWeight = 2;
            opponentRunAwayKingWeight = 1;
        }
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
        // This means attempt to reduce the opponent
        // at almost any price. Even if it means losing
        // a few pieces.
        if (totalPieceDisparityCount >= 6)
        {
            playerBonus = 10000;
            opponentManWeight = 6000;
            opponentKingWeight = 16000;
            manWeight = 1;
            kingWeight = 1;
        }
        // This means that you are trailing in pieces.
        // The objective is to now set up a more defensive
        // mode of play. Typically it is bad to get here.
        else if (totalPieceDisparityCount <= -6)
        {
            opponentBonus = 10000;
            manWeight = 60;
            kingWeight = 160;
        }
        if ((kingCount - opponentKingCount) >= 4)
        {
            playerBonus = 10000;
        }
        else if (kingCount - opponentKingCount <= -4)
        {
            opponentBonus = 10000;
        }
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
        // This places a high value on the current player's pieces.  If a player's turn leads to the losing of a piece
        // a lower value for the evaluation will be returned
        pieceValue =500,
        kingValue = 5000,
        preservePlayersPieces = numPieces * pieceValue + numKingsScore * kingValue,
        preserveOpponentsPieces = numOponentPieces * pieceValue + numOponentKingsScore * kingValue,
        //  This places a value on taking opponent pieces,  if a move removes an opponent piece this results in
        // a higher return value for the move
        oponentValue = 100,
        oponentKingValue = 1000,
        opponentPieces = (numOponentPieces * oponentValue + numOponentKingsScore * oponentKingValue),  // Value of all opponent's pieces
        opponentPieceValue = (opponentPieces - numOponentKingsScore * oponentKingValue ) / oponentValue,  // value of each man
        opponentKingValue = opponentPieces - opponentPieceValue,  // value of each king
        crossOfPainValue = 50,  //This is the square immediately above, below, to the right or left of the king
        // When in end game (less than 8 pieces) this value is used to alter strategy between taking pieces
        // and preserving pieces
        endGameAdjust = 1,
        rowNumber = 0,
        playerPiece,
        opponentPiece,
        positionAdder = 0,
        opponentPositionAdder = 0,
        moveScore = 0;
    const int KING = 2,
              MAN = 1;
    int squareValuesForRed[] = {1, 1, 1, 1,
                                2, 2, 2, 2,
                                3, 3, 3, 3,
                                5, 5, 5, 5,
                                8, 8, 8, 8,
                                15, 15, 15, 15,
                                25, 25, 25, 25,
                                75, 75, 75, 75};
    int squareValuesForBlack[] = {75, 75, 75, 75,
                                  25, 25, 25, 25,
                                  15, 15, 15, 15,
                                  8, 8, 8, 8,
                                  5, 5, 5, 5,
                                  3, 3, 3, 3,
                                  2, 2, 2, 2,
                                  1, 1, 1, 1};
    int squareValuesForKing[] = {1, 1, 1, 1,
                                 3, 5, 5, 3,
                                 7, 9, 9, 7,
                                 11, 13, 13, 11,
                                 11, 13, 13, 11,
                                 7, 9, 9, 7,
                                 3, 5, 5, 3,
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
        opponentPiece = state.getPieceInSquare(i, switchPlayerColor(color));
        // get value of current player's board
        if (playerPiece == KING) // piece is a King
        {
            // this encourages a king to move towards the center of the board
            positionAdder += squareValuesForKing[i - 1] * 10;
            //std::cout << i << "  (i-1)%4 " << (i-1)%4 << " (i+1)%4 "  << (i+1)%4 << " i - 8 " << i -8 << " i + 8 " << i + 8 << std::endl;
            // look for opponents in + or - 1 or + or - 8.  if true this is a good place to be
            // look -1 (to the right)
            if ( (i-1)%4 != 0 && state.getPieceInSquare((i-1)%4, switchPlayerColor(color)) != 0 )   // not a right edge piece and an opponent piece is here!
                positionAdder += crossOfPainValue;
            if ( ((i-1)/4+1) == ( i/4 + 1) && state.getPieceInSquare((i+1)%4, switchPlayerColor(color)) != 0 )   // not a left edge piece and an opponent piece is here!
                positionAdder += crossOfPainValue;
            if ( i - 8 > 0 && state.getPieceInSquare( (i-8), switchPlayerColor(color)) != 0 )
                positionAdder += crossOfPainValue;
            if ( i + 8 < 33 && state.getPieceInSquare( (i+8), switchPlayerColor(color)) != 0 )
                positionAdder += crossOfPainValue;
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
        //Get value of opponent player's board
        if (opponentPiece == KING) // piece is a King
        {
            // this encourages a king to move towards the center of the board
            opponentPositionAdder += squareValuesForKing[i - 1] * 10;
        }
        else if (playerPiece == MAN) // Regular pieces are encouraged to move towards the opponent's back row.
        {
            if (color == Color::RED)
            {
                opponentPositionAdder += squareValuesForRed[i - 1];
            }
            else if (color == Color::BLACK)
            {
                opponentPositionAdder += squareValuesForBlack[i - 1];
            }
        }
    }
    moveScore = preservePlayersPieces + opponentPieceValue + opponentKingValue + positionAdder;
    moveScore -= (preserveOpponentsPieces + opponentPositionAdder + opponentKingValue) * .9;
    // Increase square value around opponent's location.
    // the squares above and below plus right to left have a higher value
   if (Pieces::ouputDebugData > 1)
        std::cout << indentValue << Pieces::ANSII_GREEN_COUT << " Evaluated Move:  moveScore-> " << Pieces::ANSII_END
                  << moveScore << " preservePlayersPieces-> " << preservePlayersPieces << " opponentPieces -> "
                  << opponentPieces * endGameAdjust << " PositionAdder-> " << positionAdder << std::endl;
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
    /*  Declaration of "boards" with a weight for each square
        Two for regular pieces of each color and two for kings of each color
        Encourage player to keep two pieces in the back for defense
        leaving 10 pieces for offense. Advance to the center
        but somewhat in waves as a cluster, to avoid suicidal pieces that expose themselves
    */
    int squareValuesForRed[] = {  150, 150, 150,  150,
                                100, 125, 125, 125,
                                  100, 100, 100, 75,
                                50,  75,  75,  75,
                                  50, 50, 50, 45,
                                40,  45, 45, 45,
                                   40,  40,  40, 35,
                                50, 110, 110, 110};

    int squareValuesForBlack[] = {  110, 110, 110, 50,
                                  35, 40, 40, 40,
                                    45, 45, 45, 40,
                                  45, 50, 50, 50,
                                    75,  75, 75,  50,
                                  75,  100, 100, 100,
                                    125, 125, 125, 100,
                                  150, 150, 150, 150};

    // Traveling kings 
    int squareValuesForRedKing[] = {  35, 35, 35, 35,
                                    30, 40, 30, 40,
                                      25, 50, 50, 25,
                                    20, 20, 60, 20,
                                      15, 15, 70, 15,
                                    10, 10, 10, 80,
                                      5, 5,  5, 15,
                                    1, 1,  1,  1};

    int squareValuesForBlackKing[] = {  1,  1,  1,  1,
                                      5,  5,  5,  15,
                                        10, 10, 80, 10,
                                      15, 15, 70, 15,
                                        20, 60, 20, 20,
                                      25, 50, 25, 25,
                                        40, 30, 30, 30,
                                      35, 35, 35, 35,};
    

    std::string colorTxt = (color == Color::RED) ? " (RED is Friendly) " : " (BLACK is Friendly) "; 
    // KING has 4 moves max, so value is 4; MAN has 2 moves max so values is 2
    const int KING = 2, MAN = 1, KING_VALUE = 4, MAN_VALUE = 2; 

    int numKingsScore = state.getNumKingPieces(color) * KING_VALUE;
    int numMenScore = (state.getNumPlayerTotalPieces(color) - state.getNumKingPieces(color)) * MAN_VALUE;

    int casualtyScore = 0, captureScore = 0, positionScore = 0, playerPiece = 0, enemyPiece = 0; 
    int preventOverflow = 100; 

    std::vector<Board::Move> playerMoves = state.moveGen(color);
    std::vector<Board::Move> enemyMoves = state.moveGen(switchPlayerColor(color));

    Pieces playerPieces = state.getPlayerPieces(color);
    Pieces opponentPieces = state.getOpponentPieces(color); 
    Pieces *p_playerPieces = &playerPieces; 
    Pieces *p_opponentPieces = &opponentPieces; 

    std::vector<Board::Move> playerJumpsForPiece;
    std::vector<Board::Move> opponentJumpsForPiece;

    // helper values to quickly check if a piece is in a certain notable location
    // back rows for each color player to determine KING-ing
    // sides inidcate limited moves
    const long long redBackRowGrp = (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4);
    const long long blackBackRowGrp = (1LL << 29) | (1LL << 30) | (1LL << 31) | (1LL << 32);
    const long long sideColumnGrp = (1LL << 5) | (1LL << 13) | (1LL << 21) | (1LL << 12) | (1LL << 20) | (1LL << 28); 

    // CHECK TERMINAL STATE
    if (enemyMoves.size() == 0)
        return std::numeric_limits<int>::max() - preventOverflow; // good for us if enemy has no moves left! 
    else if (playerMoves.size() == 0)
        return std::numeric_limits<int>::min() + preventOverflow; // bad for us if we're out of moves! 

    // MAIN LOOP FOR SCORING POSITION
    for (int piece = 0; piece < 32; piece++)
    {
        // BEGIN POSITION SCORE SECTION
        if (color == Color::RED)
        {
            playerPiece = state.getPieceInSquare(piece, color); 
            if (playerPiece == MAN)
                positionScore += (squareValuesForRed[piece] * MAN_VALUE);
            else if (playerPiece == KING)
                positionScore += (squareValuesForRedKing[piece] * KING_VALUE);
        }
        else if (color == Color::BLACK)
        {
            playerPiece = state.getPieceInSquare(piece, color); 
            if (playerPiece == MAN)
                positionScore += (squareValuesForBlack[piece] * MAN_VALUE);
            else if (playerPiece == KING)
                positionScore += (squareValuesForBlackKing[piece] * KING_VALUE);
        }
        // END POSITION SCORE SECTION        
    }

    /*  BEGIN CAPTURES AND ADVANCEMENT SECTION
        If for any given piece we can make more than one capture, we are done. 
        That is a very good move and we prioritize it. 

        Otherwise if we can make at least one capture, we give this state a high score 
        but we still want to examine others to see if they may be bettr
    */    
    int advancementScore = (playerMoves.size() >= enemyMoves.size()) ? 100 : -100; // do we have as many or more moves than enemy?
    
    for (int i = 0; i < playerMoves.size(); i++)
    {   
        // int playerDestinationSqr = playerMoves.at(i).destinationSquare.back(); 
        // if (color == Color::RED)
        // {
        //     advancementScore += squareValuesForRed[playerDestinationSqr];
        //     if ((1 << playerDestinationSqr) & blackBackRowGrp)
        //         advancementScore *= 100; 
        // }
        // else if (color == Color::BLACK)
        // {
        //     advancementScore += squareValuesForBlack[playerDestinationSqr];
        //     if ((1 << playerDestinationSqr) & redBackRowGrp)
        //         advancementScore *= 100; 
        // }

        if (playerMoves.at(i).removalSquare.size() > 1)
        {
            //if (Pieces::ouputDebugData)
                std::cout << " INSIDE EVAL-3: We " << colorTxt << " can capture multiple pieces on this state!  " 
                          << "Start: " << playerMoves.at(i).startSquare << "End: " << playerMoves.at(i).destinationSquare.back() 
                          << " " << std::endl;
            
            return std::numeric_limits<int>::max() - preventOverflow; // good move! 
        }
        else if (playerMoves.at(i).removalSquare.size() == 1)
        {
            int enemyCaptureSqr = playerMoves.at(i).removalSquare.back(); 
            int enemyCaptureType = state.getPieceInSquare(enemyCaptureSqr, switchPlayerColor(color)); // it's an enemy piece, what is it's type?

            if (enemyCaptureType == KING) 
            {
                //if (Pieces::ouputDebugData)
                    std::cout << " INSIDE EVAL-3: We can capture enemy KING! " << colorTxt << std::endl; 
                
                return std::numeric_limits<int>::max() - preventOverflow;
            }

            Board newBoardAfterCapture = state.updateBoard(playerMoves.at(i), color);
            std::vector<Board::Move> newEnemyMovesGivenOurCapture = newBoardAfterCapture.moveGen(switchPlayerColor(color)); 
            
            for (int y = 0; y < newEnemyMovesGivenOurCapture.size(); y++)
            {
                if (newEnemyMovesGivenOurCapture.at(y).removalSquare.size() > 1)
                {
                    //if (Pieces::ouputDebugData)
                        std::cout << " INSIDE EVAL-3: We " << colorTxt << "capture 1 piece but lose 2, avoid!" << std::endl;
            
                    return std::numeric_limits<int>::min() + preventOverflow; // we capture one piece only to lose 2, do not do this
                }
            }
        }
    }
    // END CAPTURES SECTION

    // BEGIN CASUALTY SECTION
    for (int j = 0; j < enemyMoves.size(); j++)
    {
        //std::cout << "we are here" << std::endl; 
        if (enemyMoves.at(j).removalSquare.size() > 1)
        {
            //if (Pieces::ouputDebugData)
                std::cout << " INSIDE EVAL-3: Enemy can capture multiple pieces, avoid!" << colorTxt << std::endl;
            
            return std::numeric_limits<int>::min() + preventOverflow; // bad move! 
        }
        else if (enemyMoves.at(j).removalSquare.size() == 1)
        {
            // friendly piece may is captured, what is it's type?
            int capturedPieceType = state.getPieceInSquare(enemyMoves.at(j).removalSquare.at(0), color);
            if (capturedPieceType == KING)
            {
                //if (Pieces::ouputDebugData)
                    std::cout << " INSIDE EVAL-3: Enemy can capture a KING, avoid!" << colorTxt << std::endl;

                return -10000; // bad move!
            }
            else if (capturedPieceType == MAN)
            {
                int opponentDestinationSqr = enemyMoves.at(j).destinationSquare.at(0);

                if (color == Color::RED)
                {
                    // if we are RED, opponent is Black; if BLACK enemy lands on our back row, avoid at all cost
                    // we are therefore trying to minimize the chance of an enemy getting a KING
                    if ((1 << opponentDestinationSqr) & redBackRowGrp)
                        return -5000; 
                    // opponent lands on their own back row; not so bad but we can't retaliate so avoid
                    else if ((1 << opponentDestinationSqr) & blackBackRowGrp)
                        return -50;
                    // opponent lands on one of the side squares, where we cannot retaliate. Avoid as well
                    else if ((1 << opponentDestinationSqr) & sideColumnGrp)
                        return -50; 
                } 
                else 
                {
                    // if we are BLACK, opponent is Red; if RED enemy lands on our back row, avoid at all cost
                    // we are therefore trying to minimize the chance of an enemy getting a KING
                    if ((1 << opponentDestinationSqr) & blackBackRowGrp)
                        return -5000;
                        // opponent lands on their own back row; not so bad but we can't retaliate so avoid
                    else if ((1 << opponentDestinationSqr) & redBackRowGrp)
                        return -50;
                    // opponent lands on one of the side squares, where we cannot retaliate. Avoid as well
                    else if ((1 << opponentDestinationSqr) & sideColumnGrp)
                        return -50; 
                    
                }

                Board boardAfterEnemyJump = state.updateBoard(enemyMoves.at(j), switchPlayerColor(color));  //. simulate enemy captured one of our MEN
                std::vector<Board::Move> newPlayerMoves = boardAfterEnemyJump.moveGen(color); // available moves after enemy makes capture

                /*  Go through our new available moves if we sacrifice a MAN
                    If we trade 2 for 1, do it
                    If we trade 1 for 1 and land on opponent back row, do it
                    If we just trade 1 for 1, maybe do it as it can advance our position
                */ 
                for (int x = 0; x < newPlayerMoves.size(); x++)
                {
                    if (newPlayerMoves.at(x).removalSquare.size() > 1)
                    {
                        //if (Pieces::ouputDebugData)
                            std::cout << colorTxt << " INSIDE EVAL-3: Sacrificed one MAN but can take two pieces!" << std::endl; 

                        return std::numeric_limits<int>::max() - preventOverflow; // we sacrificed one piece for two, good! 
                    } 
                    else if (newPlayerMoves.at(x).removalSquare.size() == 1)
                    {
                        int playerDestinationSqrAfterCapture = newPlayerMoves.at(x).destinationSquare.back(); 
                        captureScore += 500; 

                        if (color == Color::RED)
                        {   
                            if ((1 << playerDestinationSqrAfterCapture) & blackBackRowGrp)
                            {
                                //if (Pieces::ouputDebugData)
                                    std::cout << colorTxt << " INSIDE EVAL-3: Sacrificed one MAN but got a KING!" << std::endl; 

                                return std::numeric_limits<int>::max() - preventOverflow; // we trade 1 for 1 but get a KING! 
                            }
                            casualtyScore += 500; 
                        } 
                        else // BLACK
                        {
                            if ((1 << playerDestinationSqrAfterCapture) & redBackRowGrp)
                            {
                                //if (Pieces::ouputDebugData)
                                    std::cout << colorTxt << " INSIDE EVAL-3: Sacrificed one MAN but got a KING!" << std::endl; 

                                return std::numeric_limits<int>::max() - preventOverflow; // we trade 1 for 1 but get a KING! 
                            }
                            casualtyScore += 500;
                        }
                    }
                }
            }
        }
    }
    //END CASUALTY SECTION

    int compositeScore = numMenScore + numKingsScore + advancementScore + positionScore + captureScore + casualtyScore;

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