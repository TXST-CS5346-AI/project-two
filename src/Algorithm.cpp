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
 * Member Function | Algorithm | evalFunctionOne
 *
 * Summary :	Evaluates the current board and produces a score for it based on
 *              the current player of this ply. Note that it toggles back and forth
 *              from maximizing to minimizing.
 *
 * @author : David Torrente
 *
 * @param State state	:			The state of the board to be used in evaluation.
 *
 * @param Color color   :			The player color that is considered the current player.
 *
 * @return int	        :			Returns an evaluated score for the board.
 *
 */
int Algorithm::evalFunctOne(Board state, Color color)
{
    int gameTurn = callingPlayer.getNumTurns();
    int finalScore = 0;
    bool criticalPoint = false;
    Color opponentColor;
    int backRowDefense = 0;
    int opponentBackRowDefense = 0;
    int centerPiece;
    int centerControl = 0;
    // These are the weights for the opening series of turns.
    // They later adjust to better fit end game play.
    int manWeight = 40;
    int kingWeight = 90;
    int mobilityWeight = 5;
    int backRowDefenseWeight = 5;
    int centerControlWeight = 6;
    int playerScore = 0;
    int playerBonus = 0;
    int opponentManWeight = 40;
    int opponentKingWeight = 90;
    int opponentMobilityWeight = 5;
    int opponentBackRowDefenseWeight = 2;
    int opponentScore = 0;
    int opponentBonus = 0;
    std::vector<Board::Move> playerMoves;
    std::vector<Board::Move> opponentMoves;
    //=======================================================================
    // Set up a few items based on the player color. Opponent and player
    // defense row.
    //=======================================================================
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
            if (state.getPieceInSquare(redIter + 28, opponentColor) == 1)
            {
                opponentBackRowDefense++;
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
            if (state.getPieceInSquare(blackIter - 28, opponentColor) == 1)
            {
                opponentBackRowDefense++;
            }
        }
    }
    int manCount = state.getNumRegularPieces(color);
    int kingCount = state.getNumKingPieces(color);
    int opponentManCount = state.getNumRegularPieces(opponentColor);
    int opponentKingCount = state.getNumKingPieces(opponentColor);
    //=======================================================================
    // Check for the two obvious ones, a winning state or a losing state.
    // Used to simply exit out of the eval function without calculating
    // a bunch of extra values.
    //=======================================================================
    playerMoves = state.moveGen(color);
    int mobility = playerMoves.size();
    // This is a losing state, avoid it.
    // Note that vs. other evaluation functions, this may not happen.
    // This means that while it sees a losing state, the other eval
    // may take a different path and void out these results.
    if (!criticalPoint && mobility == 0)
    {
        opponentBonus = opponentBonus + 6000000;
        criticalPoint = true;
    }
    opponentMoves = state.moveGen(opponentColor);
    int opponentMobility = opponentMoves.size();
    // This is a winning state, try to get it.
    // Note that vs. other evaluation functions, this may not happen.
    // This means that while it sees a losing state, the other eval
    // may take a different path and void out these results.
    int totalJumped = 0;
    int totalToJump = opponentKingCount + opponentManCount;
    for (int moveIter = 0; !criticalPoint && moveIter < playerMoves.size(); moveIter++)
    {
        totalJumped = playerMoves.at(moveIter).destinationSquare.size();
        if ((totalToJump - totalJumped) == 0)
        {
            playerBonus = playerBonus + 6000000;
            criticalPoint = true;
        }
    }
    if (!criticalPoint)
    {
        //=======================================================================
        // Set some values based on the current turn to adjust play styles.
        // Games tend to run a little longer, so the tunr numbers are set up
        // higher than one would expect.
        //=======================================================================
        if (gameTurn >= 40)
        {
            manWeight = 20;
            kingWeight = 80;
            mobilityWeight = 3;
            backRowDefenseWeight = 2;
            centerControlWeight = 2;
            opponentManWeight = 20;
            opponentKingWeight = 80;
            opponentMobilityWeight = 3;
            opponentBackRowDefenseWeight = 1;
        }
        else if (gameTurn >= 60)
        {
            manWeight = 10;
            kingWeight = 30;
            mobilityWeight = 3;
            backRowDefenseWeight = 0;
            centerControlWeight = 1;
            opponentManWeight = 40;
            opponentKingWeight = 90;
            opponentMobilityWeight = 1;
            opponentBackRowDefenseWeight = 0;
        }
        //=======================================================================
        // Check for pretty bad states, such as a large difference in the number
        // of pieces between players. This typically means multiple jumps.
        // Intended to avoid fine tuning specific calculations.
        //=======================================================================
        int totalPieceDisparityCount = ((manCount + kingCount) - (opponentManCount + opponentKingCount));
        // This means attempt to reduce the opponent
        // at almost any price. Even if it means losing
        // a few pieces.
        if (totalPieceDisparityCount >= 2)
        {
            playerBonus = playerBonus + 1000;
            opponentManWeight = 600;
            opponentKingWeight = 1600;
            manWeight = 1;
            kingWeight = 1;
        }
        // This is a larger disparity. Be even more agressive.
        if (totalPieceDisparityCount >= 4)
        {
            playerBonus = playerBonus + 10000;
            opponentManWeight = 6000;
            opponentKingWeight = 16000;
            manWeight = 1;
            kingWeight = 1;
        }
        // This means that you are trailing in pieces.
        // The objective is to now set up a more defensive
        // mode of play. Typically it is bad to get here.
        else if (totalPieceDisparityCount <= -2)
        {
            opponentBonus = opponentBonus + 1000;
            manWeight = 60;
            kingWeight = 160;
        }
        // A difference in kings can lead to a loss.
        // This is a small adjustment to monitor this.
        if ((kingCount - opponentKingCount) >= 2)
        {
            playerBonus = playerBonus + 100;
        }
        else if (kingCount - opponentKingCount <= -2)
        {
            opponentBonus = opponentBonus + 100;
        }
        //=======================================================================
        // Check for a specific location bonus.
        // 14,15,17,18,19,22,23 are the center locations.
        // Add a bonus if you occupy them.
        //=======================================================================
        for (int centerIter = 14; centerIter <= 23; centerIter++)
        {
            centerPiece = state.getPieceInSquare(centerIter, color);
            // Double the control value if it is a king in that location.
            if (centerPiece == 1)
            {
                centerControl = centerControl + 1;
            }
            else if (centerPiece == 2)
            {
                centerControl = centerControl + 2;
            }
            // Skip the edges
            if (centerIter == 15 || centerIter == 19)
            {
                centerIter++;
                if (centerIter == 20)
                {
                    centerIter++;
                }
            }
        }
    }
    // Tally up the player overall score and then the opponent overall score.
    playerScore =
        (manCount * manWeight) +
        (kingCount * kingWeight) +
        (mobility * mobilityWeight) +
        (backRowDefense * backRowDefenseWeight) +
        playerBonus;
    opponentScore =
        (opponentManCount * opponentManWeight) +
        (opponentKingCount * opponentKingWeight) +
        (opponentMobility * opponentMobilityWeight) +
        (opponentBackRowDefense * opponentBackRowDefenseWeight) +
        opponentBonus;
    finalScore = playerScore - opponentScore;
    // Clamp the final score based on the current scoring
    // system. Note that these values can later be adjusted
    // to use a wider range.
    if (finalScore <= -8000000)
    {
        finalScore = -7999999;
    }
    if (finalScore >= 9000000)
    {
        finalScore = 7999999;
    }
    return finalScore;
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
    const int KING = 2,
              MAN = 1;
    std::string indentValue; // Tracking header in cout statements.  Shows who is player being processed
    std::string playerColor; // Used for debugging output
    // these four variables are the number of pieces each player has on the board in this state.
    int numPieces = state.getNumRegularPieces(color),                              //  how many regular pieces are on the board
        numKingPieces = state.getNumKingPieces(color),                             //  how many Kings are on the board
        numOpponentPieces = state.getNumRegularPieces(switchPlayerColor(color)),   //  same as above
        numOpponentKingsPieces = state.getNumKingPieces(switchPlayerColor(color)), // except opponent values
        // Total Pieces on the board
        totalPieces = numPieces + numKingPieces + numOpponentPieces + numOpponentKingsPieces,
        // This places a value on the player's pieces.  This weight can be modified to encourage defensive
        // piece preservation strategy
        pieceValue = 560,
        kingValue = 3000,
        //  total value of the piece preservation portion of the equation
        preservePlayersPieces = numPieces * pieceValue + numKingPieces * kingValue,
        // total value of the opponents piece value
        preserveOpponentsPieces = numOpponentPieces * pieceValue + numOpponentKingsPieces * kingValue,
        //  This places a value on taking opponent pieces,  if a move removes an opponent piece this results in
        // a higher return value for the move
        opponentValue = 100,
        opponentKingValue = 1000,
        opponentPieces = (numOpponentPieces * opponentValue + numOpponentKingsPieces * opponentKingValue),        // Value of all opponent's pieces
        reduceOpponentPieceValue = (opponentPieces - numOpponentKingsPieces * opponentKingValue) / opponentValue, // value of each man
        reduceOpponentKingValue = opponentPieces - reduceOpponentPieceValue,                                      // value of each king
        crossOfPainValue = 50,                                                                                    //This is the square immediately above, below, to the right or left of the king
        // When in end game (less than 8 pieces) this value is used to alter strategy between taking pieces
        // and preserving pieces
        endGameAdjust = 1,
        playerPiece,
        opponentPiece,
        positionAdder = 0,
        opponentPositionAdder = 0,
        playerScore,
        opponentScore,
        moveScore,
        currentTerminal = 0,
        opponentTerminal = 0;
    //  The opponent's board will be scored the same as the current player.  This percentage allows for the
    // adjustment in the impact the player's board has on the final value
    double opponentEvaluationWeight = .55;
    // the values Red squares.  This encourages the Red men to move towards the opponents base line
    // the two high values for Red's base line encourages a base defense strategy
    // startup Board
    int squareValuesForRed[] = {10, 1, 10, 1,
                                8, 5, 5, 2,
                                5, 3, 3, 5,
                                5, 3, 5, 3,
                                5, 8, 5, 8,
                                15, 8, 15, 8,
                                15, 25, 15, 25,
                                75, 75, 75, 75};
    if (numOpponentPieces == 3) // only Kings left, encourage all men to move to the back row
    {
        for (int i = 0; i < 32; ++i)
        {
            squareValuesForRed[i] = (i % 4) * 3;
        }
    }
    int squareValuesForBlack[32]; // this board will be initialized below
    // black values are the opposite of Red
    for (int i = 31; i >= 0; --i)
        squareValuesForBlack[31 - i] = squareValuesForRed[i];
    // this array is used to encourage the King to move towards the center of the board
    int squareValuesForKing[] = {1, 1, 1, 1,
                                 3, 5, 5, 3,
                                 7, 9, 9, 7,
                                 8, 13, 13, 8,
                                 8, 13, 13, 8,
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
                  << Pieces::ANSII_END << numPieces << " Kings-> " << numKingPieces << " Opponent Pieces-> "
                  << numOpponentPieces << " Kings-> " << numOpponentKingsPieces << " total pieces-> "
                  << totalPieces << std::endl;
    if (numPieces + numKingPieces == 0) // if the current player is out of pieces this is a terminal state
    {
        if (Pieces::ouputDebugData > 1)
            std::cout << indentValue << Pieces::ANSII_RED_COUT << "Current Player is in TERMINAL STATE!! "
                      << Pieces::ANSII_END << std::endl;
        currentTerminal = -877775; //  returning a high value.  the current player is about to lose
                                   // Avoid this move!
    }
    if (numOpponentPieces + numOpponentKingsPieces == 0) // if the current player is out of pieces this is a terminal state
    {
        //if (Pieces::ouputDebugData > 1)
        std::cout << indentValue << Pieces::ANSII_RED_COUT << "Opponent is in TERMINAL STATE!!  "
                  << Pieces::ANSII_END << std::endl;
        opponentTerminal = 777775; //  The opponent's is about to lose
        //  Go for it if possible
    }
    // Piece preservation and the taking of opponent pieces are  global values. In other words, if no pieces are lost
    // or taken, all moves that have this result will have the same evaluation value.
    // This loop looks at each player's piece and the board generates a value that will separate certain moves from
    // the rest.
    for (int i = 1; i <= 32; ++i)
    {
        // is a player or opponent in this square?  RV (0 = empty, 1 = man, 2 = king)
        playerPiece = state.getPieceInSquare(i, color);
        opponentPiece = state.getPieceInSquare(i, switchPlayerColor(color));
        // get value of current player's board
        if (playerPiece == KING) // piece is a King
        {
            // this encourages a king to move towards the center of the board
            positionAdder += squareValuesForKing[i - 1] * 10;
            // Cross of Pain Calculation
            // look for opponents in column + or - 1 or row + or - 8.  if true this is a good place to be
            // Current piece is not on the right edge!
            if ((i - 1) % 4 != 0 && state.getPieceInSquare((i - 1) % 4, switchPlayerColor(color)) != 0)
                positionAdder += crossOfPainValue;
            // Current piece is not on the left edge!  i.e. they are in the same row
            if (((i - 1) / 4 + 1) == (i / 4 + 1) && state.getPieceInSquare((i + 1) % 4, switchPlayerColor(color)) != 0)
                positionAdder += crossOfPainValue;
            // Current piece is not on the top row!
            if (i - 8 > 0 && state.getPieceInSquare((i - 8), switchPlayerColor(color)) != 0)
                positionAdder += crossOfPainValue;
            // Current piece is not on the bottom row!
            if (i + 8 < 33 && state.getPieceInSquare((i + 8), switchPlayerColor(color)) != 0)
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
        //Get value of opponent player's board using the same method as player
        if (opponentPiece == KING) // piece is a King
        {
            // this encourages a king to move towards the center of the board
            opponentPositionAdder += squareValuesForKing[i - 1] * 10;
            // Does the current player end up in the Cross of Pain Calculation
            // look for opponents in column + or - 1 or row + or - 8.  if true this is a good place to be
            // Current piece is not on the right edge!
            if ((i - 1) % 4 != 0 && state.getPieceInSquare((i - 1) % 4, switchPlayerColor(color)) != 0)
                opponentPositionAdder += crossOfPainValue;
            // Current piece is not on the left edge!
            if (((i - 1) / 4 + 1) == (i / 4 + 1) && state.getPieceInSquare((i + 1) % 4, switchPlayerColor(color)) != 0)
                opponentPositionAdder += crossOfPainValue;
            // Current piece is not on the top row!
            if (i - 8 > 0 && state.getPieceInSquare((i - 8), switchPlayerColor(color)) != 0)
                opponentPositionAdder += crossOfPainValue;
            // Current piece is not on the bottom row!
            if (i + 8 < 33 && state.getPieceInSquare((i + 8), switchPlayerColor(color)) != 0)
                opponentPositionAdder += crossOfPainValue;
        }
        else if (playerPiece == MAN) // Opponent's Regular pieces are encouraged to move towards the back row.
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
    playerScore = preservePlayersPieces + positionAdder + currentTerminal;
    opponentScore = (preserveOpponentsPieces + opponentPositionAdder + reduceOpponentKingValue) * opponentEvaluationWeight + opponentTerminal;
    moveScore = playerScore - opponentScore;
    if (Pieces::ouputDebugData > 1)
        std::cout << indentValue << Pieces::ANSII_GREEN_COUT << " Evaluated Move:  moveScore-> " << Pieces::ANSII_END
                  << moveScore << std::endl;

    // Clamp the final score based on the current scoring
    // system. Note that these values can later be adjusted
    // to use a wider range.
    if (moveScore <= -8000000)
    {
        moveScore = -7999999;
    }
    if (moveScore >= 9000000)
    {
        moveScore = 7999999;
    }
    return moveScore;
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
        Try to attack the opponent's "double corner" from where a kinged piece can escape faster
    */
    int squareValuesForRed[] = {7, 1, 7, 1,
                                1, 2, 2, 2,
                                1, 5, 5, 5,
                                1, 3, 3, 3,
                                1, 4, 4, 4,
                                1, 5, 250, 250,
                                1, 250, 500, 500,
                                50, 100, 100, 1000};

    int squareValuesForBlack[] = {1000, 100, 100, 50,
                                  500, 500, 250, 1,
                                  250, 250, 5, 1,
                                  4, 4, 4, 1,
                                  3, 3, 3, 1,
                                  5, 5, 5, 1,
                                  2, 2, 2, 1,
                                  1, 7, 1, 7};

    // Kings preference for center, with some traversal lines
    // to attempt and avoid a "back-and-forth" pattern
    int squareValuesForKing[] = {1, 1, 1, 1,
                                 1, 5, 5, 55,
                                 5, 15, 45, 1,
                                 1, 5, 35, 5,
                                 5, 25, 25, 1,
                                 1, 15, 5, 15,
                                 5, 5, 5, 10,
                                 1, 1, 1, 1};

    std::string colorTxt = (color == Color::RED) ? " (RED is Friendly) " : " (BLACK is Friendly) ";
    // KING has 4 moves max, so value is 4; MAN has 2 moves max so values is 2
    const int KING = 2, MAN = 1, KING_VALUE = 4, MAN_VALUE = 2;

    int numPlayerTotalPieces = state.getNumPlayerTotalPieces(color);
    int numEnemyTotalPieces = state.getNumPlayerTotalPieces(switchPlayerColor(color));
    int numPlayerTotalKings = state.getNumKingPieces(color);
    int numEnemyTotalKings = state.getNumKingPieces(switchPlayerColor(color));
    int numPlayerTotalMen = numPlayerTotalPieces - numPlayerTotalKings;
    int numEnemyTotalMen = numEnemyTotalPieces - numEnemyTotalKings;

    int numKingsScore = numPlayerTotalKings * KING_VALUE;
    int numMenScore = numPlayerTotalMen * MAN_VALUE;

    int diffInNumPieces = numPlayerTotalPieces - numEnemyTotalPieces;
    int diffInNumKings = numPlayerTotalKings - numEnemyTotalKings;
    int diffInNumMen = numPlayerTotalMen - numEnemyTotalMen;

    // PIECE BONUS/PENALTY - UNCOMMENT TO ACTIVATE
    // if diff in kings is positive, score is amplified with a bonus
    // if diff in kings is negative, however, score is penalized accordingly (by adding a negative)
    // numKingsScore += (40 * diffInNumKings);

    // if diff in men is positive, score is amplified with a bonus
    // if diff in men is negative, however, score is penalized accordingly (by adding a negative)
    // numMenScore += (20 * diffInNumMen);
    // END PIECE BONUS/PENALTY

    int casualtyScore = 0, captureScore = 0, positionScore = 0, playerPiece = 0, enemyPiece = 0, advancementScore = 0;

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
        return 7999999; // good for us if enemy has no moves left!
    else if (playerMoves.size() == 0)
        return -7999999; // bad for us if we're out of moves!

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
                positionScore += (squareValuesForKing[piece] * KING_VALUE);
        }
        else if (color == Color::BLACK)
        {
            playerPiece = state.getPieceInSquare(piece, color);
            if (playerPiece == MAN)
                positionScore += (squareValuesForBlack[piece] * MAN_VALUE);
            else if (playerPiece == KING)
                positionScore += (squareValuesForKing[piece] * KING_VALUE);
        }
        // END POSITION SCORE SECTION
    }

    // Check our moves; 1000 points for a safe capture, 2000 points for a multi-jump
    for (int i = 0; i < playerMoves.size(); i++)
    {
        if (playerMoves.at(i).removalSquare.size() > 1)
        {
            if (Pieces::ouputDebugData)
                std::cout << " INSIDE EVAL-3: We " << colorTxt << " can capture multiple pieces on this state!  "
                          << "Start: " << playerMoves.at(i).startSquare << "End: " << playerMoves.at(i).destinationSquare.back()
                          << " " << std::endl;

            captureScore += 2000;
        }
        else if (playerMoves.at(i).removalSquare.size() == 1)
        {

            int enemyCaptureSqr = playerMoves.at(i).removalSquare.back();
            int enemyCaptureType = state.getPieceInSquare(enemyCaptureSqr, switchPlayerColor(color)); // it's an enemy piece, what is it's type?

            if (enemyCaptureType == KING)
            {
                if (Pieces::ouputDebugData)
                    std::cout << " INSIDE EVAL-3: We can capture enemy KING! " << colorTxt << std::endl;

                captureScore += 400;
            }
            else if (enemyCaptureType == MAN)
                captureScore += 200;

            int destSqr = playerMoves.at(i).destinationSquare.back();
            std::vector<int> adjMoves = state.boardMoveTable[destSqr].moves;

            if (color == Color::RED)
            {
                for (int j = 0; j < adjMoves.size(); j++)
                {
                    if (adjMoves.at(j) > destSqr) // check enemy MEN and KING below
                    {
                        int enemyPiece = state.getPieceInSquare(adjMoves.at(j), switchPlayerColor(color));
                        if (enemyPiece == MAN || enemyPiece == KING)
                            captureScore -= 100; // not safe
                    }
                    else if (adjMoves.at(j) < destSqr) // we're red, anyting above us can only capture if enemy KING
                    {
                        int enemyPiece = state.getPieceInSquare(adjMoves.at(j), switchPlayerColor(color));
                        if (enemyPiece == KING)
                            captureScore -= 100; // not safe
                    }
                    else
                        captureScore += 1000; // we're safe to capture
                }
            }
            else
            {
                for (int j = 0; j < adjMoves.size(); j++)
                {
                    if (adjMoves.at(j) < destSqr) // check enemy MEN and KING above
                    {
                        int enemyPiece = state.getPieceInSquare(adjMoves.at(j), switchPlayerColor(color));
                        if (enemyPiece == MAN || enemyPiece == KING)
                            captureScore -= 100; // not safe
                    }
                    else if (adjMoves.at(j) > destSqr) // we're black, anyting below us can only capture if enemy KING
                    {
                        int enemyPiece = state.getPieceInSquare(adjMoves.at(j), switchPlayerColor(color));
                        if (enemyPiece == KING)
                            captureScore -= 100; // not safe
                    }
                    captureScore += 1000; // we're safe to capture
                }
            }
        }
    }

    // BEGIN CASUALTY SECTION
    for (int j = 0; j < enemyMoves.size(); j++)
    {
        //std::cout << "we are here" << std::endl;
        if (enemyMoves.at(j).removalSquare.size() > 1)
        {
            if (Pieces::ouputDebugData)
                std::cout << " INSIDE EVAL-3: Enemy can capture multiple pieces, avoid!" << colorTxt << std::endl;

            casualtyScore -= 40000; // we lose too much, really bad
        }
        else if (enemyMoves.at(j).removalSquare.size() == 1)
        {
            // friendly piece is captured, what is it's type?
            int capturedPieceType = state.getPieceInSquare(enemyMoves.at(j).removalSquare.at(0), color);
            if (capturedPieceType == KING)
            {
                if (Pieces::ouputDebugData)
                    std::cout << " INSIDE EVAL-3: Enemy can capture a KING, avoid!" << colorTxt << std::endl;

                casualtyScore -= 4000; // we lose a KING, a valuable piece
            }
            else if (capturedPieceType == MAN) // we lose one MAN
            {
                int opponentDestinationSqr = enemyMoves.at(j).destinationSquare.at(0);
                std::vector<int> adjMoves = state.boardMoveTable[opponentDestinationSqr].moves;

                if (color == Color::RED)
                {
                    // if we are RED, opponent is Black; if BLACK enemy lands on our back row, avoid at all cost
                    // we are therefore trying to minimize the chance of an enemy getting a KING
                    if ((1 << opponentDestinationSqr) & redBackRowGrp)
                        casualtyScore -= 5000;
                    // opponent lands on their own back row; not so bad but we can't retaliate so avoid
                    else if ((1 << opponentDestinationSqr) & blackBackRowGrp)
                        casualtyScore -= 2000;
                    // opponent lands on one of the side squares, where we cannot retaliate. Avoid as well
                    else if ((1 << opponentDestinationSqr) & sideColumnGrp)
                        casualtyScore -= 2000;
                    else
                    {
                        // if (diffInNumMen >= 1)
                        // {
                        for (int j = 0; j < adjMoves.size(); j++)
                        {
                            int ourPiece = state.getPieceInSquare(adjMoves.at(j), switchPlayerColor(color));
                            // check if we have a king below; we're red, only our king can go upwards
                            if (adjMoves.at(j) > opponentDestinationSqr)
                            {
                                if (ourPiece == KING)
                                    captureScore += 1000; // can retaliate
                            }
                            // we're red, we can retaliate with MAN or KING if enemy is above
                            else if (adjMoves.at(j) < opponentDestinationSqr)
                            {
                                if (ourPiece == MAN || ourPiece == KING)
                                    captureScore += 1000; // can retaliate
                            }
                            else
                                captureScore += 0; // we cannot capture
                        }
                        // }
                    }
                }
                else
                {
                    // if we are BLACK, opponent is Red; if RED enemy lands on our back row, avoid at all cost
                    // we are therefore trying to minimize the chance of an enemy getting a KING
                    if ((1 << opponentDestinationSqr) & blackBackRowGrp)
                        casualtyScore -= 5000;
                    // opponent lands on their own back row; not so bad but we can't retaliate so avoid
                    else if ((1 << opponentDestinationSqr) & redBackRowGrp)
                        casualtyScore -= 2000;
                    // opponent lands on one of the side squares, where we cannot retaliate. Avoid as well
                    else if ((1 << opponentDestinationSqr) & sideColumnGrp)
                        casualtyScore -= 2000;
                    // if we've gotten this far, we lose one MAN and opponent lands somewhere we can retaliate
                    // We ask - Can we? If yes, do it if we have piece parity or an advantage of more pieces
                    else
                    {
                        // if (numPlayerTotalPieces >= numEnemyTotalPieces)
                        // {
                        for (int j = 0; j < adjMoves.size(); j++)
                        {
                            int ourPiece = state.getPieceInSquare(adjMoves.at(j), switchPlayerColor(color));
                            if (adjMoves.at(j) < opponentDestinationSqr) // check our KING above
                            {
                                if (ourPiece == KING)
                                    captureScore += 1000; // can retaliate
                            }
                            // we're black, if enemy is below we can retaliate with MAN or KING
                            else if (adjMoves.at(j) > opponentDestinationSqr)
                            {
                                if (ourPiece == MAN || ourPiece == KING)
                                    captureScore += 1000; // can retaliate
                            }
                            captureScore += 0; // we cannot retaliate
                        }
                        // }
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
 * maxValue - algorithm from Russel & Norvig, implemented to fit this program
 * This function would determine the score of goodness for a state passed in to the MAX player
 * There is indirect recursion as MAX calls MIN, which calls MAX again and so on.  
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

    // terminal check
    if (listOfActions.size() == 0)
    {
        absearchLeafNodes++;
        result.value = staticEval(state, color, evalVersion); // eval acts as utility funct
        return result;
    }

    if (Pieces::ouputDebugData)
        std::cout << "Not yet at a terminal state...." << std::endl;

    result.value = std::numeric_limits<int>::min();

    for (int actionIndex = 0; actionIndex < listOfActions.size(); actionIndex++)
    {
        Player::printMove(listOfActions.at(actionIndex), color, false);
        absearchExpandedNodes++;
        Board tmpState = state.updateBoard(listOfActions.at(actionIndex), color);
        Algorithm::Result minValueResult = minValue(tmpState, depth - 1, alpha, beta, switchPlayerColor(color));

        if (minValueResult.value > result.value) // Best move located
        {
            result.value = minValueResult.value;
            bestMove = listOfActions.at(actionIndex);
        }

        if (result.value >= beta) // no need to examine branch
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
 * minValue function - algorithm from Russel & Norvig, implemented to fit this program
 * This function would determine the score of goodness for a state passed in to the MIN player
 * There is indirect recursion as MAX calls MIN, which calls MAX again and so on. 
 * @author Borislav Sabotinov
 * 
 * @param Board state
 * @param int alpha
 * @param int beta
 * 
 * @return Result structure, which contains a value score and a Board::Move bestMove structure
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

    // terminal check
    if (listOfActions.size() == 0)
    {
        absearchLeafNodes++;
        result.value = staticEval(state, color, evalVersion); // eval acts as utility funct
        return result;
    }

    if (Pieces::ouputDebugData)
        std::cout << "Not yet at a terminal state...." << std::endl;

    result.value = std::numeric_limits<int>::max();

    for (int actionIndex = 0; actionIndex < listOfActions.size(); actionIndex++)
    {
        Player::printMove(listOfActions.at(actionIndex), color, false);
        absearchExpandedNodes++;
        Board tmpState = state.updateBoard(listOfActions.at(actionIndex), color);
        Result maxValueResult = maxValue(tmpState, depth - 1, alpha, beta, switchPlayerColor(color));

        if (maxValueResult.value < result.value) // Best move located
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

/**
 * Helper function to switch a color, so if we are RED and we want our opponent, we would get BACK
 * @author Borislav Sabotinov
 * 
 * @param Color color - the color we want to invert
 * 
 * @return Color - the color opposite of what we passed in
 */
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