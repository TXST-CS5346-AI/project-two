#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
// used by strcmp method
#include <string.h> 


#ifndef PIECES_H
#define PIECES_H

/**
* The Color enumerator is used in place of values 1 and -1 
* in order to make the code easier to read. Note the values 
* of 1 and -1. This allows for alternating play by multiplying the current 
* by 1. 

*/
enum class Color
{
    RED = 1,
    BLACK = -1
};

/**
* The Pieces class represents all pieces for a particular player. Each object
* of the class contains a single instance of pieces. In addition to this,
* ANSII code strings were included here in order to make the output
* of the code easier to read. These codes serve no functional purpose
* aside from changing text color.
*/
class Pieces
{
public:
    Pieces();
    Pieces(Color color);

    // ANSII codes for colored text, to improve UI and readability
    static std::string ANSII_BLUE_START;
    static std::string ANSII_RED_START;
    static std::string ANSII_RED_HIGH;
    static std::string ANSII_END;
    static std::string ANSII_GREEN_START;
    static std::string ANSII_BLUE_COUT;
    static std::string ANSII_RED_COUT;
    static std::string ANSII_GREEN_COUT;
    static std::string ANSII_YELLOW_COUT;

    // Debug reporting level 3 == display all debug/status lines 2== important, 1 == basic, 0 == none
    static int ouputDebugData;

    bool isKing(int position) const;
    void setKing(int poisition, bool toKing);

    // This contains 64 bits to represent the entire piece set and king status for one side.
    long long pieces;
};

#endif // !PIECES_H


#ifndef BOARD_H
#define BOARD_H


/**
 * Class board is used to represent the entire board and its current state.
 * It is based on an 8 x 8 grid, with 32 possible spaces. Two piece data
 * types are the primary memory consumers of this class. This class also includes 
 * a static member that acts as a move guide. This move guide determines the possible
 * moves for a square on the board, not for a piece. 
 */
class Board
{

public:
	/**
	* Struct Nove is used to track moves made on the board. It contains
	* a single starting location, a vector of steps to a move, 
	* as well as all of the pieces to remove when this move is made.
	*/
	struct Move
	{
		int startSquare;
		std::vector<int> destinationSquare;
		std::vector<int> removalSquare;
	};

	/**
	* Struct BoardMoveTable is a static data type that is used to speed
	* up searching for moves for each position. It is shared among all
	* boards.
	*/
	struct BoardMoveTable
	{
		std::vector<int> jumps;
		std::vector<int> removals;
		std::vector<int> moves;
	};

	Board();
	~Board();
	static void InitializeMoveTable();
	std::vector<Move> moveGen(Color color);

	void printBoard() const;
	Board updateBoard(Move move, Color color);

	int getNumRegularPieces(Color color);
	int getNumKingPieces(Color color);
	int getNumPlayerTotalPieces(Color color);
	int getPieceInSquare(int square, Color color);

	std::vector<Move> getJumpsForPiece(Color color, int square, Pieces *playerPieces, Pieces *opponentPieces);
	std::vector<Move> getMovesForPiece(Color color, int square, Pieces *playerPieces, Pieces *opponentPieces);

	Pieces getPlayerPieces(Color color);
	Pieces getOpponentPieces(Color color);

	static BoardMoveTable boardMoveTable[33];

private:
	Pieces blackPieces;
	Pieces redPieces;

	void getJumpsForPieceRec(Color color, Board::Move move, std::vector<Board::Move> &totalMoves, Board board, bool wasKingPriorMove);
};

#endif


#ifndef PLAYER_H
#define PLAYER_H


/**
 * Header definition for class Player.
 *
 * The Player class defines a virtual player, in the case of this program an AI.
 * No human players are used in this game - meaning no manual input is sought from the user.
 * Project 2, Requirement #7: "Your program should play with a computer to a computer."
 *
 */

class Player
{

private:
    Color color;        // represents player's color in the game, either RED or BLACK
    int numPieces;      // how many pieces does Player have left
    int numPiecesTaken; // Player's current score based on captured enemy pieces
    int numTurnsTaken;  // counter for Player's turns taken
    bool didPlayerMove; //  EndGame Condition

    int depth, evalVersion;

public:
    Player();  // constructor
    ~Player(); // destructor

    bool isMinimax; // if false use AB Prune, if true use Minimax. Allows control over alg player uses

    Player(bool minMax, Color color, int depth, int evalVersion); // overloaded constructor to set player color, which is IMMUTABLE

    int takeTurn(Board &state);
    int getNumPieces();
    int getNumPiecesTaken();
    int getNumTurns();
    bool getDidPlayerMove();
    Color getColor();

    void decreaseNumPieces(int numPiecesToDecreaseCount);
    void increaseNumPiecesTaken(int numPiecesToIncreaseScore);

    static void printMove(Board::Move, Color color, bool willPrintAlways);

    int minimaxExpandedNodes;  // how many nodes we expand
    int minimaxLeafNodes;      // how many nodes we expand
    int absearchExpandedNodes; // how many nodes we expand
    int absearchLeafNodes;     // how many nodes we expand

    int getMinimaxTotalNodes()
    {
        return minimaxExpandedNodes + minimaxLeafNodes;
    }

    int getAbSearchTotalNodes()
    {
        return absearchExpandedNodes + absearchLeafNodes;
    }
};

#endif // !PLAYER_H


#ifndef GAME_H
#define GAME_H


/**
 * Header definition for class Game.
 *
 * The Game class represents a checkers Game.
 *
 * A game consists of a board, two players (red and black), and
 * 12 checkers pieces for each player.
 *
 * A game ends when either player loses all their pieces or is blocked and
 * has no further moves available.
 *
 * The startGame() function will trigger the process and no input from the user is required.
 * Red and Black players will be created. Each player will be given 12 checker pieces.
 * The pieces will be appropriately placed on the board.
 * Each player will execute their strategy.
 *

 */

class Game
{

private:
    Board state;
    Player redPlayer;
    Player blackPlayer;
    const int MAX_ALLOWED_TURNS = 80;
    void printNodes(Player player, std::string colorText);

public:
    Game();                          // constructor
    ~Game();                         // destructor
    Game(bool, int, bool, int, int); // player1 algo, eval version, player2 algo, eval version, depth

    enum class GameOver
    {
        BLACK_WINS = 0,
        RED_WINS = 1,
        DRAW = 2,
        NOT_DONE = 3
    };

    // The only initialization function needed, as the game will
    // be played automatically by 2 AI players (MIN and MAX).
    // while gameOver == NOT_DONE keep playing
    GameOver startGame();

    // simple function to invert the enum value, thus determine who's turn is it next.
    // E.g., if currentPlayer is RED (1), function returns BLACK (-1)
    Color changePlayer(Color currentPlayer);

    GameOver gameOver(); // Have end game conditions been met?
    bool doesRedWin();
    bool doesBlackWin();
    bool isItADraw();
};

#endif // !GAME_H


#ifndef ALGORITHM_H
#define ALGORITHM_H


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
    Algorithm();  // constructor
    ~Algorithm(); // destructor

    Algorithm(int evalVersion, int maxDepth, Player callingPlayer);

    struct Result
    {
        int value;
        Board::Move bestMove;
    };

    int minimaxExpandedNodes;  // how many nodes we expand
    int minimaxLeafNodes;      // how many nodes we expand
    int absearchExpandedNodes; // how many nodes we expand
    int absearchLeafNodes;     // how many nodes we expand

    // minimax algorithm returns the position of the best move
    Result minimax_a_b(Board board, int depth, Color color, int useThresh, int passThresh);

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
    Result maxValue(Board state, int depth, int alpha, int beta, Color color);
    Result minValue(Board state, int depth, int alpha, int beta, Color color);
    int utility(Board state);
    std::vector<Board::Move> actions(Board state, Color color);

    Color switchPlayerColor(Color color);

    int passSign(int passthresh);
};

#endif // !ALGORITHM_H


#ifndef SIMULATION_H
#define SIMULATION_H


/**
 * Header definition for class Simulation. 
 * @author Borislav Sabotinov
 * 
 * This class is responsible for managing the series of Games that AI players will play for Project Two. 
 * It persists during the execution of the program and keeps track of the number of games played. 
 * It also allows to aggregate and print simulation analysis details. 
 * 
 * There are THREE (3) evaluation functions, one for each team member; two algorithms (minimax and Alpha-Beta). 
 * The simulation will execute each in turn. 
 * 
 * Fifteen runs with depth 2:
 *    1. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #1
 *    2. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *    3. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 * 
 *    4. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #1
 *    5. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #2
 *    6. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 * 
 *    7. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #1
 *    8. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #2
 *    9. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #3
 * 
 *   10. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #2
 *   11. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #3
 *   12. MinMax-A-B with Evl. Function #2  Verses MinMax-A-B with Evl. Function #3
 * 
 *   13. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *   14. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 *   15. Alpha-Beta with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 * 
 * Fifteen runs with depth 4:
 *    1. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #1
 *    2. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *    3. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 * 
 *    4. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #1
 *    5. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #2
 *    6. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 * 
 *    7. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #1
 *    8. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #2
 *    9. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #3
 * 
 *   10. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #2
 *   11. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #3
 *   12. MinMax-A-B with Evl. Function #2  Verses MinMax-A-B with Evl. Function #3
 * 
 *   13. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *   14. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 *   15. Alpha-Beta with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 *
 * TOTAL 30 RUNS/GAMES WILL BE SIMULATED. 
 */

class Simulation
{

private:
    int numGamesPlayed;

    // runs only games using Minimax algorithm
    void runMinimaxOnly();

    // runs only games using AB Prune algorithm
    void runABPruneOnly();

public:
    Simulation();  // constructor
    ~Simulation(); // destructor

    // runs all games runs as delineated above
    void runFullSimulation();

    // public method for specific simulations
    void runSpecificSimulation(int playerOneAlg, int playerOneEvalFunct, int playerTwoAlg, int PlayerTwoEvalFunct, int depth);

    // public method for player vs AI simulation
    void runPlayerVsAISimulation(int playerAlg, int playerEvalFunct, int depth);

    // helper function to determine winner and break out of game loop
    static bool didSomeoneWin(Board board);

    // returns a count of the number of games played in a simulation
    // each of the 3 run functions.
    int getNumGamesPlayed();

    // creates a table with results for analysis.
    // how many nodes were created, etc.
    void generateAnalysisResults();

    void printGameConfig(int redPlayerAlg, int redPlayerEvalFunct, int blackPlayerAlg, int blackPlayerEvalFunct, int depth);
    void printGameResults(Game::GameOver endGameStatus);

    // helper print methods
    static void printBlackWins();
    static void printRedWins();
    static void printDraw();
};

#endif // !SIMULATION_H

// CLASS PIECES

// ANSII codes for colored text, to improve UI and readability
std::string Pieces::ANSII_BLUE_START = "\033[0;30;46m";
std::string Pieces::ANSII_RED_START = "\033[0;31m";
std::string Pieces::ANSII_RED_HIGH = "\033[9;37;41m";
std::string Pieces::ANSII_END = "\033[0m";
std::string Pieces::ANSII_GREEN_START = "\033[0;32m";
std::string Pieces::ANSII_BLUE_COUT = "\033[0;30;46m";
std::string Pieces::ANSII_RED_COUT = "\033[41;1m";
std::string Pieces::ANSII_GREEN_COUT = "\033[0;30;42m";
std::string Pieces::ANSII_YELLOW_COUT = "\033[30;48;5;3m";

int Pieces::ouputDebugData = 3;

/**
 * Constructor | Pieces | Pieces
 *
 * Summary :	Unused. Constructor with
 *				a parameter is always required.
 * 
 * @author : David Torrente 
 * 
 */
Pieces::Pieces()
{
}

/**
 * Constructor | Pieces | Pieces
 *
 * Summary :	Sets up the player pieces depending on
 *				the color passed in.
 * 
 * @author : David Torrente
 * 
 * @param Color color	: The color to assign the pieces to.
 * 
 */
Pieces::Pieces(Color color)
{

	// The following are bit fields to be used for the board. Each value
	// turns on or off a bit. For example, 4095 looks like:
	// 1111 1111 1111, which will position a piece in squares 1 - 12
	// top squares. The middle is blank, so 0000 0000, which covers
	// squares 13 - 20. Lastly,
	// black is placed in the lowest bits similar to white's pattern,
	// but in spaces 21 - 32, hence, the large number. Neither start with
	// kings, so bits 33 - 64 are all zeros on both sides.

	// Note that the commented out values are primarily for debugging
	// special move cases or interesting situations. They are retained here as
	// pairs. If used, each pair must be uncommented.
	if (color == Color::RED) // red
		//pieces = 1;
		//pieces = 19455;
		//pieces = 1152921504875282432;
		pieces = 4095; // Initial board state
	else			   // black
		//pieces = 4291952640;
		//pieces = 128;
		//pieces = 16974848;
		pieces = 4293918720; // Initial board state
}

/**
 * Member Function | Pieces | isKing
 *
 * Summary :	Determines if the piece in a given position is
 *				a king or not. Prior to calling this, the position
 *				must be checked to see if the position is occupied
 *				by a player.
 *
 * @author : David Torrente 
 * 
 * @param int position	:	The position on the board to
							check to see if it is a king.
 *
 * @return bool			:	Returns true if it is a king piece,
 *							false otherwise.
 *
 */
bool Pieces::isKing(int position) const
{
	// NOTE: it is 31, since the offset is 0 - 31
	// Could have written +32 -1, but better to just combine.
	// Checks the high bit (33 - 64) which is aligned with
	// the position bit (1 - 32).
	int kingBit = position + 31;

	bool isKing = false;

	if (((pieces >> kingBit) & 1) == 1)
	{
		isKing = true;
	}

	return isKing;
}

/**
 * Member Function | Pieces | setKing
 *
 * Summary :	Sets the king bit for a given position to either
 *				be on or off. 
 *
 * @author : David Torrente 
 * 
 * @param int position	:	The position on the board to
 *							toggle to a king or not king.
 * 
 * @param bool toKing	:	True if the piece is to become
 *							a king, false if it is to be reduced 
 *							to not a king. Reducing the piece type
 *							is primarily done to keep the board clean.
 *
 */
void Pieces::setKing(int position, bool toKing)
{
	int kingBit = position + 31;

	if (toKing == true)
	{
		pieces = pieces | (1ULL << kingBit);
	}
	else
	{
		pieces = pieces & ~(1ULL << kingBit);
	}
}



// CLASS BOARD

// Forward declare the static data member.
Board::BoardMoveTable Board::boardMoveTable[33];

/**
  * Constructor | Board | Board
  * 
  * Summary	: Creates both the red player and black player
  *				 piece list. If not already done, it also
  *				instantiates the static move table.
  *
  * @author : David Torrente
  * 
  */
Board::Board()
{
	// Guarded internally to prevent from having moves added to it.
	InitializeMoveTable();

	// Assign the proper pieces to the player, either red or black.
	redPieces = Pieces(Color::RED);
	blackPieces = Pieces(Color::BLACK);
}

/**
 * Destructor | Board | ~Board
 *
 * Summary : Class destructor. No special code required.
 * 
 * @author : David Torrente
 * 
 */
Board::~Board()
{
}

/**
 * Member Function | Board | getPlayerPieces
 *
 * Summary :	Gets the pieces that belong to a particular player. 
 *
 * @author : David Torrente
 * 
 * @param Color color :	 The player color of pieces to get.
 *
 * @return Pieces :	The pieces belonging to the player.
 * 
 */
Pieces Board::getPlayerPieces(Color color)
{
	if (color == Color::RED)
	{
		return redPieces;
	}
	else
	{
		return blackPieces;
	}
}

/**
 * Member Function | Board | getOpponentPieces
 *
 * Summary :	Gets the pieces that belong to the 
 *				opposing player.
 *
 * @author : David Torrente
 * 
 * @param Color color :	 The player color of pieces to
 *						 get the opponent of.
 *
 * @return Pieces :	The pieces belonging to the 
 *						opposing player.
 *
 */
Pieces Board::getOpponentPieces(Color color)
{
	if (color == Color::RED)
	{
		return blackPieces;
	}
	else
	{
		return redPieces;
	}
}

/**
 * Member Function | Board | getNumRegularPieces
 *
 * Summary :	Gets the count of man pieces that belong 
 *				to the player.
 *
 * @author : David Torrente

 * @param Color color :	 The player color of pieces to
 *						 get the count for.
 *
 * @return int :	A value 0 to 12 which is the count
 *					of man pieces.
 *
 */
int Board::getNumRegularPieces(Color color)
{
	return getNumPlayerTotalPieces(color) - getNumKingPieces(color);
}

/**
 * Member Function | Board | getNumKingPieces
 *
 * Summary :	Gets the count of king pieces that belong
 *				to the player.
 *
 * @author : David Torrente
 * 
 * @param Color color :	 The player color of pieces to
 *						 get the count for.
 *
 * @return int :	A value 0 to 12 which is the count
 *					of king pieces.
 *
 */
int Board::getNumKingPieces(Color color)
{
	{
		int kingPieceCount = 0;

		long long *playerPieces;

		if (color == Color::RED)
		{
			playerPieces = &redPieces.pieces;
		}
		else
		{
			playerPieces = &blackPieces.pieces;
		}

		for (int iter = 32; iter < 64; iter++)
		{
			if (((*playerPieces >> iter) & 1) == 1)
			{
				kingPieceCount++;
			}
		}

		return kingPieceCount;
	}
}

/**
 * Member Function | Board | getNumPlayerTotalPieces
 *
 * Summary :	Gets the count of total pieces that belong
 *				to the player.
 *
 * @author : David Torrente
 *
 * @param Color color :	 The player color of pieces to
 *						 get the count for.
 *
 * @return int :	A value 0 to 12 which is the count
 *					of total pieces.
 *
 */
int Board::getNumPlayerTotalPieces(Color color)
{
	int totalPieceCount = 0;

	long long *playerPieces;

	if (color == Color::RED)
	{
		playerPieces = &redPieces.pieces;
	}
	else
	{
		playerPieces = &blackPieces.pieces;
	}

	for (int iter = 0; iter < 32; iter++)
	{

		if (((*playerPieces >> iter) & 1) == 1)
		{
			totalPieceCount++;
		}
	}

	return totalPieceCount;
}

/**
 * Member Function | Board | moveGen
 *
 * Summary :	Gets all of the possible moves for the player
 *				based on the color parameter. Note that this
 *				is bound by the mandatory jump rule, meaning
 *				that a player who can jump will not be given
 *				the option to move to a adjacent space. These
 *				adjacent space moves will not even be computed
 *				if there is a jump possible.
 *
 * @author : David Torrente
 *
 * @param Color color :			The player color to get the moves
 *								for.
 * 
 * @return vector<Board::Move> :	A vector of possible moves
 *									for the player.
 *
 */
std::vector<Board::Move> Board::moveGen(Color color)
{
	std::vector<Move> totalMoves;
	std::vector<Move> returnedMoves;

	Pieces *playerPieces;
	Pieces *opponentPieces;

	int bitOffset = 0;

	if (color == Color::RED)
	{
		playerPieces = &redPieces;
		opponentPieces = &blackPieces;
	}

	else
	{
		playerPieces = &blackPieces;
		opponentPieces = &redPieces;
	}

	// Go through all 32 squares and see if it is one
	// of the appropriate pieces belonging to player.
	for (int pieceIter = 1; pieceIter <= 32; pieceIter++)
	{
		// The offset is used to align to 0 - 31, but
		// the board in checkers is 1 - 32. Use an offset
		// here to align to the position bits properly.
		bitOffset = pieceIter - 1;

		if ((((playerPieces->pieces) >> bitOffset) & 1) == 1)
		{
			// Check for possible jump mpves first.
			returnedMoves = getJumpsForPiece(color, pieceIter, playerPieces, opponentPieces);
			totalMoves.insert(totalMoves.end(), returnedMoves.begin(), returnedMoves.end());
		}
	}

	// Go through all 32 squares and see if it is one
	// of the appropriate pieces belonging to player.
	// Do this only if no jumps are possible. This is controlled
	// by the if condition here. No need to get moves if there are
	// jumps already found.
	if (totalMoves.size() == 0)
	{
		for (int pieceIter = 1; pieceIter <= 32; pieceIter++)
		{
			// The offset is used to align to 0 - 31, but
			// the board in checkers is 1 - 32. Use an offset
			// here to align to the position bits properly.
			bitOffset = pieceIter - 1;

			// Check if player is in this space
			if ((((playerPieces->pieces) >> bitOffset) & 1) == 1)
			{
				// Check for non jump moves here.
				returnedMoves = getMovesForPiece(color, pieceIter, playerPieces, opponentPieces);
				totalMoves.insert(totalMoves.end(), returnedMoves.begin(), returnedMoves.end());
			}
		}
	}
	return totalMoves;
}

/**
 * Member Function | Board | getJumpsForPiece
 *
 * Summary :	Gets all of the possible jumps for the specific
 *				piece based on the color parameter and location.
 *				This is only called after confirming that the player
 *				has a piece in the proper location.
 *
 * @author : David Torrente
 *
 * @param Color color :				The player color of pieces to
 *									get the jumps for.
 * 
 * @param int piece	  :				The board location to get the jumps
 *									for.
 * 
 * @param Pieces *playerPieces :	A pointer to the player pieces.
 *									Needed since these can block a possible
 *									jump.
 *
 * @param Pieces *opponentPieces :	A pointer to the opponent pieces.
 *									Needed since these can block a possible
 *									jump as well as to confirm that a jump
 *									can happen.
 *
 * @return vector<Board::Move> :	A vector of possible jumps
 *									for the particular piece.
 *
 */
std::vector<Board::Move> Board::getJumpsForPiece(Color color, int piece, Pieces *playerPieces, Pieces *opponentPieces)
{
	// The returned vector of all possible jumps carried out completely
	std::vector<Move> finalMoves;

	Board board;

	bool wasKingPriorMove = false;

	if (color == Color::RED)
	{
		board.redPieces = *playerPieces;
		board.blackPieces = *opponentPieces;
	}
	else
	{
		board.redPieces = *opponentPieces;
		board.blackPieces = *playerPieces;
	}
	// In order to start the recursion, this is needed.
	Move move;
	move.startSquare = piece;
	wasKingPriorMove = board.getPlayerPieces(color).isKing(move.startSquare);

	getJumpsForPieceRec(color, move, finalMoves, board, wasKingPriorMove);

	return finalMoves;
}

/**
 * Member Function | Board | getJumpsForPieceRec
 *
 * Summary :	Gets all of the possible jumps in a single jump attempt.
 *				Often called "double" jumping. This creates a full
 *				jump chain. a recursive call.
 *
 * @author : David Torrente
 * 
 * @param Color color :			The player color of pieces to
 *								get the jump chain for.
 *
 * @param Move move	  :			A starter move. Contains a starting
 *								space, with an empty destination vector.
 *								when passed in to a recursive call,
 *								this destination vector may contain
 *								moves, signifying that it is in the middle
 *								of a jump chain.
 * 
 * @param vector<Board> & totalMovesAccumulator : Accumulates all possible
 *										jumps at the end of each jump chain. Passed
 *										in by reference and returned to calling 
 *										function.
 * 
 * @param Board board :			The current state of the board, updated for each jump.
 * 
 * @param bool wasKingPriorMove	: A value used to determine if the king was a king prior 
 *								  to this move. Becoming a king stops a jump chain.
 *
 */
void Board::getJumpsForPieceRec(Color color, Board::Move move, std::vector<Board::Move> &totalMovesAccumulator, Board board, bool wasKingPriorMove)
{

	int piece;

	bool endOfJumpChain = true;

	bool isKing = false;

	int bitOffset = 0;
	int squareJumped = 0;

	// This determines if we are starting the jump sequence or in the middle of it.
	if (move.destinationSquare.size() == 0)
	{
		piece = move.startSquare;
		// use the current board as it is.
	}
	else
	{
		piece = move.destinationSquare.back();
		board = updateBoard(move, color);
	}

	// First, determine if it is a king. This is needed to see which moves
	// are valid for this piece/player. Move either up/down at first.
	isKing = board.getPlayerPieces(color).isKing(piece);

	if (isKing == wasKingPriorMove)
	{
		// Check if a jump position is open for this piece. This goes through all of the jumps.
		for (int jumpIter = 0; jumpIter < Board::boardMoveTable[piece].jumps.size(); jumpIter++)
		{
			// Get the position of the jump, reduce it by one for an offset. Note
			// that while it is one less than the position, the direction check still works
			// since a jump will always be greater than the distance needed to determine
			// direction.
			bitOffset = (Board::boardMoveTable[piece].jumps.at(jumpIter) - 1);

			// Check to see which direction it is going, and if it can go that direction.
			// King goes both ways		Red not a king goes "down"            Black not a king goes "up"
			if (isKing || ((piece - bitOffset) < 0 && color == Color::RED) || ((piece - bitOffset) > 0 && color == Color::BLACK))
			{
				// Combine both bit fields into one and check if the space is empty.
				if ((((board.getPlayerPieces(color).pieces | board.getOpponentPieces(color).pieces) >> bitOffset) & 1) == 0)
				{
					// If it is open, get the space between. We will need to check it.
					// Again with the -1 to help with the offset
					squareJumped = Board::boardMoveTable[piece].removals.at(jumpIter) - 1;

					if (((board.getOpponentPieces(color).pieces >> squareJumped) & 1) == 1)
					{
						// Yup, we are good!!!! this means that an opponent was in this spot and
						// we can jump them.

						// Since we found a new jump, we need to keep going with the jump chain
						endOfJumpChain = false;

						// Do not change start square. This is set to the overall start of the jump.
						// Commented out to show the thought process behind setting the move.
						// move.startSquare = piece;

						move.destinationSquare.push_back(bitOffset + 1);
						move.removalSquare.push_back(squareJumped + 1);

						//make recursive call here
						getJumpsForPieceRec(color, move, totalMovesAccumulator, board, wasKingPriorMove);
						// Remove the jump we just added to the chain. We've already made the recursive call
						move.destinationSquare.pop_back();
						move.removalSquare.pop_back();
					}
				}
			}
		}
	}

	if (endOfJumpChain == true)
	{
		// Once here, it means we are at the leaf of a jump or
		// that we became a king due to this jump.
		// We add it to the list at that point.
		// It will rise back up the chain when the stack unwinds.

		if (move.destinationSquare.size() != 0)
		{
			totalMovesAccumulator.push_back(move);
		}
	}
}

/**
 * Member Function | Board | getMovesForPiece
 *
 * Summary :	Gets all of the possible moves for a piece. This call knows that the
 *				piece is a proper piece for this color to play on prior to this call.
 *				Also note that prior to this call, jumps should be checked. If there
 *				are jumps, do not allow these moves.
 *
 * @author : David Torrente 
 * 
 * @param Color color :				The player color of pieces to
 *									get the moves for.
 *
 * @param int piece	  :				The board location to get the moves
 *									for.
 *
 * @param Pieces *playerPieces :	A pointer to the player pieces.
 *									Needed since these can block a possible
 *									jump.
 *
 * @param Pieces *opponentPieces :	A pointer to the opponent pieces.
 *									Needed since these can block a possible
 *									jump as well as to confirm that a jump
 *									can happen.
 *
 * @return vector<Board::Move> :	A vector of possible moves
 *									for the particular piece.
 *
 */
std::vector<Board::Move> Board::getMovesForPiece(Color color, int piece, Pieces *playerPieces, Pieces *opponentPieces)
{
	std::vector<Move> moves;
	Move move;

	bool isKing = false;

	int squareJumped = 0;

	int bitOffset = 0;

	// First, determine if it is a king. This is needed to see which moves
	// are valid for this piece/player. Move either up/down at first.
	if (playerPieces->isKing(piece))
	{
		isKing = true;
	}
	else
	{
		isKing = false;
	}

	for (int moveIter = 0; moveIter < Board::boardMoveTable[piece].moves.size(); moveIter++)
	{
		bitOffset = Board::boardMoveTable[piece].moves.at(moveIter) - 1;
		if (isKing || ((piece - bitOffset) < 0 && color == Color::RED) || ((piece - bitOffset) > 0 && color == Color::BLACK))
		{
			if ((((playerPieces->pieces | opponentPieces->pieces) >> bitOffset) & 1) == 0)
			{
				move.startSquare = piece;
				move.destinationSquare.push_back(bitOffset + 1);
				moves.push_back(move);
				move.destinationSquare.clear();
			}
		}
	}

	return moves;
}

/**
 * Member Function | Board | printBoard
 *
 * Summary :	Prints the current state of the board. It is a constant function.
 *
 * @author : David Torrente 
 * 
 */
void Board::printBoard() const
{
	int squareOffset = 0;

	// The board toggles back and forth in regards to the positioning
	// of pieces. This either adds the offset to the beginning of the
	// row, or to the end of it. Odd rows start with a space,
	// Even rows end with a space.
	bool oddRow = true;

	// A basic bar to go across the top. Cosmetic.
	std::cout << "_________________________________" << std::endl;

	for (int rowIter = 0; rowIter <= 7; rowIter++)
	{
		std::cout << "|";

		for (int colIter = 0; colIter <= 3; colIter++)
		{
			if (oddRow)
			{
				std::cout << "   |";
			}

			squareOffset = (rowIter * 4 + colIter);

			if (((redPieces.pieces >> squareOffset) & 1) == 1)
			{
				if (redPieces.isKing(squareOffset + 1))
				{
					std::cout << Pieces::ANSII_RED_HIGH << " R " << Pieces::ANSII_END << "|";
				}
				else
				{
					std::cout << Pieces::ANSII_RED_HIGH << " r " << Pieces::ANSII_END << "|";
				}
			}
			else if (((blackPieces.pieces >> squareOffset) & 1) == 1)
			{
				if (blackPieces.isKing(squareOffset + 1))
				{
					std::cout << Pieces::ANSII_BLUE_START << " B " << Pieces::ANSII_END << "|";
				}
				else
				{
					std::cout << Pieces::ANSII_BLUE_START << " b " << Pieces::ANSII_END << "|";
				}
			}
			else
			{
				std::cout << "   |";
			}

			if (!oddRow)
			{
				std::cout << "   |";
			}
		}

		// Now print the numeric values of the squares.
		std::cout << std::endl;

		std::cout << "|";

		for (int colIterNum = 0; colIterNum <= 3; colIterNum++)
		{
			if (oddRow)
			{
				std::cout << "___|";
			}

			squareOffset = (rowIter * 4 + colIterNum);
			std::cout << std::setfill('_') << std::setw(3) << squareOffset + 1 << "|";

			if (!oddRow)
			{
				std::cout << "___|";
			}
		}

		// Toggle between even and odd rows for the offset.
		oddRow = !oddRow;
		std::cout << std::endl;
	}
}

/**
 * Member Function | Board | getPieceInSquare
 *
 * Summary :	Gets the type of piece at a particular location for a particular color.
 *				Note that the position does not need to be checked for a piece prior to this
 *				call. If the position does not contain a piece of the matching color type, 
 *				it will report as empty for that color.
 *
 * @author : David Torrente 
 * 
 * @param int piece	  :				The board location to get the piece type
 *									for.
 * 
 * @param Color color :				The player color to get the piece type for.
 *
 * @return int	:					a value, 0 to 2, specifying the piece type of the given 
 *									color. 0 = no piece for this color. 1 = man piece
 *									for this color. 2 = king piece for this color.
 *
 */
int Board::getPieceInSquare(int position, Color color)
{
	int pieceType = 0;
	Pieces playerPieces;

	if (Color::RED == color)
	{
		playerPieces = redPieces;
	}
	else
	{
		playerPieces = blackPieces;
	}

	if (((playerPieces.pieces >> (position - 1)) & 1) == 1)
	{
		pieceType = 1;

		if (playerPieces.isKing(position))
		{
			pieceType = 2;
		}
	}

	return pieceType;
}

/**
 * Member Function | Board | updateBoard
 *
 * Summary :	Updates the entire board based on a given move. Note that this updates the complete
 *				board for both sides. This will always operate on a single properly formatted move.
 *				It will also convert pieces into kings if they reach the back row of the opposing 
 *				player.
 *
 * @author : David Torrente 
 * 
 * @param Move move	  :				The move to apply to the board. Includes the destination 
 *									and pieces to remove.
 *
 * @param Color color :				The player color that is applying this move. Needed primarily 
 *									to determine if a piece needs to be kinged.
 *
 * @return Board	:				Returns a new copy of the board. Note that this is only 
 *									two bitfields (small).
 *
 */
Board Board::updateBoard(Move move, Color color)
{

	Board updatedBoard;
	updatedBoard.blackPieces = blackPieces;
	updatedBoard.redPieces = redPieces;

	Pieces *playerPieces;
	Pieces *opponentPieces;

	if (color == Color::RED)
	{
		playerPieces = &updatedBoard.redPieces;
		opponentPieces = &updatedBoard.blackPieces;
	}
	else
	{
		playerPieces = &updatedBoard.blackPieces;
		opponentPieces = &updatedBoard.redPieces;
	}

	// Position in final destination spot - probably check if it needs to be kinged here.
	// Do this first since you will need to clear the king flag.
	playerPieces->pieces = playerPieces->pieces | (1LL << (move.destinationSquare.back() - 1));
	if (playerPieces->isKing(move.startSquare))
	{
		playerPieces->setKing(move.startSquare, false);
		playerPieces->setKing(move.destinationSquare.back(), true);
	}
	else
	{
		// Check to see if we've landed in the kinging row, the back row opposite the starting side.
		if (color == Color::RED && (move.destinationSquare.back() >= 29) || (color == Color::BLACK && (move.destinationSquare.back() <= 4)))
		{
			playerPieces->setKing(move.destinationSquare.back(), true);
		}
	}

	// Remove start pos
	if (move.startSquare != move.destinationSquare.back())
	{
		playerPieces->pieces = playerPieces->pieces & ~(1LL << (move.startSquare - 1));
	}

	// Remove all jumped spots and set them back to not a king.
	for (int jumpedSpaceIter = 0; jumpedSpaceIter < move.removalSquare.size(); jumpedSpaceIter++)
	{
		opponentPieces->pieces = opponentPieces->pieces & ~(1LL << (move.removalSquare.at(jumpedSpaceIter) - 1));
		opponentPieces->pieces = opponentPieces->pieces & ~(1LL << (move.removalSquare.at(jumpedSpaceIter) + 31));
	}

	return updatedBoard;
}

/**
 * Member Function | Board | InitializeMoveTable
 *
 * Summary :	Sets up the static move table which is shared among all
 *				boards. Note that it is a move table, not a piece tracker.
 *				pieces are stored in the board. In order to prevent adding 
 *				moves each time a constructor is called, the move table
 *				will not add any additional values after it is set up 
 *				the first time.
 *
 * @author : David Torrente 
 * 
 */
void Board::InitializeMoveTable()
{

	// In order to support faster look up,
	// The index of the position is used as
	// the initial key to an index. Each
	// position has only a few moves,
	// even less if there is a mandatory jump.
	// The table is big, but never changes.

	if (boardMoveTable[1].jumps.size() == 0)
	{
		boardMoveTable[1].jumps.push_back(10);
		boardMoveTable[1].removals.push_back(6);
		boardMoveTable[1].moves.push_back(5);
		boardMoveTable[1].moves.push_back(6);

		boardMoveTable[2].jumps.push_back(9);
		boardMoveTable[2].removals.push_back(6);
		boardMoveTable[2].jumps.push_back(11);
		boardMoveTable[2].removals.push_back(7);
		boardMoveTable[2].moves.push_back(6);
		boardMoveTable[2].moves.push_back(7);

		boardMoveTable[3].jumps.push_back(10);
		boardMoveTable[3].removals.push_back(7);
		boardMoveTable[3].jumps.push_back(12);
		boardMoveTable[3].removals.push_back(8);
		boardMoveTable[3].moves.push_back(7);
		boardMoveTable[3].moves.push_back(8);

		boardMoveTable[4].jumps.push_back(11);
		boardMoveTable[4].removals.push_back(8);
		boardMoveTable[4].moves.push_back(8);

		boardMoveTable[5].jumps.push_back(14);
		boardMoveTable[5].removals.push_back(9);
		boardMoveTable[5].moves.push_back(1);
		boardMoveTable[5].moves.push_back(9);

		boardMoveTable[6].jumps.push_back(13);
		boardMoveTable[6].removals.push_back(9);
		boardMoveTable[6].jumps.push_back(15);
		boardMoveTable[6].removals.push_back(10);
		boardMoveTable[6].moves.push_back(1);
		boardMoveTable[6].moves.push_back(2);
		boardMoveTable[6].moves.push_back(9);
		boardMoveTable[6].moves.push_back(10);

		boardMoveTable[7].jumps.push_back(14);
		boardMoveTable[7].removals.push_back(10);
		boardMoveTable[7].jumps.push_back(16);
		boardMoveTable[7].removals.push_back(11);
		boardMoveTable[7].moves.push_back(2);
		boardMoveTable[7].moves.push_back(3);
		boardMoveTable[7].moves.push_back(10);
		boardMoveTable[7].moves.push_back(11);

		boardMoveTable[8].jumps.push_back(15);
		boardMoveTable[8].removals.push_back(11);
		boardMoveTable[8].moves.push_back(3);
		boardMoveTable[8].moves.push_back(4);
		boardMoveTable[8].moves.push_back(11);
		boardMoveTable[8].moves.push_back(12);

		boardMoveTable[9].jumps.push_back(2);
		boardMoveTable[9].removals.push_back(6);
		boardMoveTable[9].jumps.push_back(18);
		boardMoveTable[9].removals.push_back(14);
		boardMoveTable[9].moves.push_back(5);
		boardMoveTable[9].moves.push_back(6);
		boardMoveTable[9].moves.push_back(13);
		boardMoveTable[9].moves.push_back(14);

		boardMoveTable[10].jumps.push_back(1);
		boardMoveTable[10].removals.push_back(6);
		boardMoveTable[10].jumps.push_back(3);
		boardMoveTable[10].removals.push_back(7);
		boardMoveTable[10].jumps.push_back(17);
		boardMoveTable[10].removals.push_back(14);
		boardMoveTable[10].jumps.push_back(19);
		boardMoveTable[10].removals.push_back(15);
		boardMoveTable[10].moves.push_back(6);
		boardMoveTable[10].moves.push_back(7);
		boardMoveTable[10].moves.push_back(14);
		boardMoveTable[10].moves.push_back(15);

		boardMoveTable[11].jumps.push_back(2);
		boardMoveTable[11].removals.push_back(7);
		boardMoveTable[11].jumps.push_back(4);
		boardMoveTable[11].removals.push_back(8);
		boardMoveTable[11].jumps.push_back(18);
		boardMoveTable[11].removals.push_back(15);
		boardMoveTable[11].jumps.push_back(20);
		boardMoveTable[11].removals.push_back(16);
		boardMoveTable[11].moves.push_back(7);
		boardMoveTable[11].moves.push_back(8);
		boardMoveTable[11].moves.push_back(15);
		boardMoveTable[11].moves.push_back(16);

		boardMoveTable[12].jumps.push_back(3);
		boardMoveTable[12].removals.push_back(8);
		boardMoveTable[12].jumps.push_back(19);
		boardMoveTable[12].removals.push_back(16);
		boardMoveTable[12].moves.push_back(8);
		boardMoveTable[12].moves.push_back(16);

		boardMoveTable[13].jumps.push_back(6);
		boardMoveTable[13].removals.push_back(9);
		boardMoveTable[13].jumps.push_back(22);
		boardMoveTable[13].removals.push_back(17);
		boardMoveTable[13].moves.push_back(9);
		boardMoveTable[13].moves.push_back(17);

		boardMoveTable[14].jumps.push_back(5);
		boardMoveTable[14].removals.push_back(9);
		boardMoveTable[14].jumps.push_back(7);
		boardMoveTable[14].removals.push_back(10);
		boardMoveTable[14].jumps.push_back(21);
		boardMoveTable[14].removals.push_back(17);
		boardMoveTable[14].jumps.push_back(23);
		boardMoveTable[14].removals.push_back(18);
		boardMoveTable[14].moves.push_back(9);
		boardMoveTable[14].moves.push_back(10);
		boardMoveTable[14].moves.push_back(17);
		boardMoveTable[14].moves.push_back(18);

		boardMoveTable[15].jumps.push_back(6);
		boardMoveTable[15].removals.push_back(10);
		boardMoveTable[15].jumps.push_back(8);
		boardMoveTable[15].removals.push_back(11);
		boardMoveTable[15].jumps.push_back(22);
		boardMoveTable[15].removals.push_back(18);
		boardMoveTable[15].jumps.push_back(24);
		boardMoveTable[15].removals.push_back(19);
		boardMoveTable[15].moves.push_back(10);
		boardMoveTable[15].moves.push_back(11);
		boardMoveTable[15].moves.push_back(18);
		boardMoveTable[15].moves.push_back(19);

		boardMoveTable[16].jumps.push_back(7);
		boardMoveTable[16].removals.push_back(11);
		boardMoveTable[16].jumps.push_back(23);
		boardMoveTable[16].removals.push_back(19);
		boardMoveTable[16].moves.push_back(11);
		boardMoveTable[16].moves.push_back(12);
		boardMoveTable[16].moves.push_back(19);
		boardMoveTable[16].moves.push_back(20);

		boardMoveTable[17].jumps.push_back(10);
		boardMoveTable[17].removals.push_back(14);
		boardMoveTable[17].jumps.push_back(26);
		boardMoveTable[17].removals.push_back(22);
		boardMoveTable[17].moves.push_back(13);
		boardMoveTable[17].moves.push_back(14);
		boardMoveTable[17].moves.push_back(21);
		boardMoveTable[17].moves.push_back(22);

		boardMoveTable[18].jumps.push_back(9);
		boardMoveTable[18].removals.push_back(14);
		boardMoveTable[18].jumps.push_back(11);
		boardMoveTable[18].removals.push_back(15);
		boardMoveTable[18].jumps.push_back(25);
		boardMoveTable[18].removals.push_back(22);
		boardMoveTable[18].jumps.push_back(27);
		boardMoveTable[18].removals.push_back(23);
		boardMoveTable[18].moves.push_back(14);
		boardMoveTable[18].moves.push_back(15);
		boardMoveTable[18].moves.push_back(22);
		boardMoveTable[18].moves.push_back(23);

		boardMoveTable[19].jumps.push_back(10);
		boardMoveTable[19].removals.push_back(15);
		boardMoveTable[19].jumps.push_back(12);
		boardMoveTable[19].removals.push_back(16);
		boardMoveTable[19].jumps.push_back(26);
		boardMoveTable[19].removals.push_back(23);
		boardMoveTable[19].jumps.push_back(28);
		boardMoveTable[19].removals.push_back(24);
		boardMoveTable[19].moves.push_back(15);
		boardMoveTable[19].moves.push_back(16);
		boardMoveTable[19].moves.push_back(23);
		boardMoveTable[19].moves.push_back(24);

		boardMoveTable[20].jumps.push_back(11);
		boardMoveTable[20].removals.push_back(16);
		boardMoveTable[20].jumps.push_back(27);
		boardMoveTable[20].removals.push_back(24);
		boardMoveTable[20].moves.push_back(16);
		boardMoveTable[20].moves.push_back(24);

		boardMoveTable[21].jumps.push_back(14);
		boardMoveTable[21].removals.push_back(17);
		boardMoveTable[21].jumps.push_back(30);
		boardMoveTable[21].removals.push_back(25);
		boardMoveTable[21].moves.push_back(17);
		boardMoveTable[21].moves.push_back(25);

		boardMoveTable[22].jumps.push_back(13);
		boardMoveTable[22].removals.push_back(17);
		boardMoveTable[22].jumps.push_back(15);
		boardMoveTable[22].removals.push_back(18);
		boardMoveTable[22].jumps.push_back(29);
		boardMoveTable[22].removals.push_back(25);
		boardMoveTable[22].jumps.push_back(31);
		boardMoveTable[22].removals.push_back(26);
		boardMoveTable[22].moves.push_back(17);
		boardMoveTable[22].moves.push_back(18);
		boardMoveTable[22].moves.push_back(25);
		boardMoveTable[22].moves.push_back(26);

		boardMoveTable[23].jumps.push_back(14);
		boardMoveTable[23].removals.push_back(18);
		boardMoveTable[23].jumps.push_back(16);
		boardMoveTable[23].removals.push_back(19);
		boardMoveTable[23].jumps.push_back(30);
		boardMoveTable[23].removals.push_back(26);
		boardMoveTable[23].jumps.push_back(32);
		boardMoveTable[23].removals.push_back(27);
		boardMoveTable[23].moves.push_back(18);
		boardMoveTable[23].moves.push_back(19);
		boardMoveTable[23].moves.push_back(26);
		boardMoveTable[23].moves.push_back(27);

		boardMoveTable[24].jumps.push_back(15);
		boardMoveTable[24].removals.push_back(19);
		boardMoveTable[24].jumps.push_back(31);
		boardMoveTable[24].removals.push_back(27);
		boardMoveTable[24].moves.push_back(19);
		boardMoveTable[24].moves.push_back(20);
		boardMoveTable[24].moves.push_back(27);
		boardMoveTable[24].moves.push_back(28);

		boardMoveTable[25].jumps.push_back(18);
		boardMoveTable[25].removals.push_back(22);
		boardMoveTable[25].moves.push_back(21);
		boardMoveTable[25].moves.push_back(22);
		boardMoveTable[25].moves.push_back(29);
		boardMoveTable[25].moves.push_back(30);

		boardMoveTable[26].jumps.push_back(17);
		boardMoveTable[26].removals.push_back(22);
		boardMoveTable[26].jumps.push_back(19);
		boardMoveTable[26].removals.push_back(23);
		boardMoveTable[26].moves.push_back(22);
		boardMoveTable[26].moves.push_back(23);
		boardMoveTable[26].moves.push_back(30);
		boardMoveTable[26].moves.push_back(31);

		boardMoveTable[27].jumps.push_back(18);
		boardMoveTable[27].removals.push_back(23);
		boardMoveTable[27].jumps.push_back(20);
		boardMoveTable[27].removals.push_back(24);
		boardMoveTable[27].moves.push_back(23);
		boardMoveTable[27].moves.push_back(24);
		boardMoveTable[27].moves.push_back(31);
		boardMoveTable[27].moves.push_back(32);

		boardMoveTable[28].jumps.push_back(19);
		boardMoveTable[28].removals.push_back(24);
		boardMoveTable[28].moves.push_back(24);
		boardMoveTable[28].moves.push_back(32);

		boardMoveTable[29].jumps.push_back(22);
		boardMoveTable[29].removals.push_back(25);
		boardMoveTable[29].moves.push_back(25);

		boardMoveTable[30].jumps.push_back(21);
		boardMoveTable[30].removals.push_back(25);
		boardMoveTable[30].jumps.push_back(23);
		boardMoveTable[30].removals.push_back(26);
		boardMoveTable[30].moves.push_back(25);
		boardMoveTable[30].moves.push_back(26);

		boardMoveTable[31].jumps.push_back(22);
		boardMoveTable[31].removals.push_back(26);
		boardMoveTable[31].jumps.push_back(24);
		boardMoveTable[31].removals.push_back(27);
		boardMoveTable[31].moves.push_back(26);
		boardMoveTable[31].moves.push_back(27);

		boardMoveTable[32].jumps.push_back(23);
		boardMoveTable[32].removals.push_back(27);
		boardMoveTable[32].moves.push_back(27);
		boardMoveTable[32].moves.push_back(28);
	}
}


// CLASS PLAYER

Player::Player()
{
}
Player::~Player()
{
}

Player::Player(bool minMaxState, Color color, int depth, int evalVersion)
{
    this->color = color;
    numPieces = 12;     // how many pieces does Player have left
    numPiecesTaken = 0; // Player's current score based on captured enemy pieces
    numTurnsTaken = 0;  // counter for Player's turns taken
    isMinimax = minMaxState;
    this->depth = depth;
    this->evalVersion = evalVersion;

    this->minimaxExpandedNodes = 0;
    this->minimaxLeafNodes = 0;
    this->absearchExpandedNodes = 0;
    this->absearchLeafNodes = 0;
}

int Player::takeTurn(Board &state)
{
    Algorithm::Result result;
    Algorithm *algorithm = new Algorithm(evalVersion, depth, *this);

    if (isMinimax)
    {
        result = algorithm->minimax_a_b(state, this->depth, this->color, 9000000, -8000000);
        this->minimaxExpandedNodes += algorithm->minimaxExpandedNodes;
        this->minimaxLeafNodes += algorithm->minimaxLeafNodes;
    }
    else
    {
        result = algorithm->alphaBetaSearch(state);
        this->absearchExpandedNodes += algorithm->absearchExpandedNodes;
        this->absearchLeafNodes += algorithm->absearchLeafNodes;
    }

    if (result.bestMove.destinationSquare.size() == 0)
    {
        didPlayerMove = false; // Player did not make a turn
    }
    else
    {
        state = state.updateBoard(result.bestMove, this->color);
        printMove(result.bestMove, this->color, true);
        numTurnsTaken++;      // incremente Player's own turn counter
        didPlayerMove = true; // return true as player did make a turn
        state.printBoard();
    }

    // return how many pieces the player took during their turn
    return result.bestMove.removalSquare.size();
}

int Player::getNumPieces()
{
    return numPieces;
}

int Player::getNumPiecesTaken()
{
    return numPiecesTaken;
}

int Player::getNumTurns()
{
    return numTurnsTaken;
}

bool Player::getDidPlayerMove()
{
    return didPlayerMove;
}

Color Player::getColor()
{
    return color;
}

void Player::decreaseNumPieces(int numPiecesToDecreaseCount)
{
    numPieces -= numPiecesToDecreaseCount;
}

void Player::increaseNumPiecesTaken(int numPiecesToIncreaseScore)
{
    numPiecesTaken += numPiecesToIncreaseScore;
}

void Player::printMove(Board::Move move, Color color, bool alwaysPrint)
{
    std::string colorStr;
    if (color == Color::BLACK)
        colorStr = "Black";
    else
        colorStr = "Red";

    int conditionalPrint;
    if (alwaysPrint)
        conditionalPrint = 1;
    else
        conditionalPrint = Pieces::ouputDebugData;

    if (conditionalPrint)
    {
        std::cout << colorStr << " moves from " << move.startSquare << " to destination (in sequence): ";
        for (int i = 0; i < move.destinationSquare.size(); i++)
            std::cout << "dest: " << move.destinationSquare.at(i) << std::endl;
    }
}



// CLASS GAME

Game::Game()
{
}

Game::~Game()
{
}

Game::Game(bool player1MinMax, int evalVersionP1, bool player2MinMax, int evalVersionP2, int depth)
{
    state = Board();
    state.InitializeMoveTable();
    redPlayer = Player(player1MinMax, Color::RED, depth, evalVersionP1);
    blackPlayer = Player(player2MinMax, Color::BLACK, depth, evalVersionP2);
}

Game::GameOver Game::startGame()
{
    int piecesTaken;

    while (true)
    {
        std::cout << "\n\nRound " << blackPlayer.getNumTurns() + 1 << " Black's Move..." << std::endl;

        piecesTaken = blackPlayer.takeTurn(state);
        blackPlayer.increaseNumPiecesTaken(piecesTaken);
        redPlayer.decreaseNumPieces(piecesTaken);

        if (piecesTaken > 0)
            std::cout << "  BLACK player took " << piecesTaken << " piece(s)." << std::endl;

        if (doesBlackWin())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::BLACK_WINS;
        }

        if (doesRedWin())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::RED_WINS;
        }

        if (isItADraw())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::DRAW;
        }

        std::cout << "\n\nRound " << redPlayer.getNumTurns() + 1 << " Red's Move..." << std::endl;

        piecesTaken = redPlayer.takeTurn(state);
        redPlayer.increaseNumPiecesTaken(piecesTaken);
        blackPlayer.decreaseNumPieces(piecesTaken);

        if (piecesTaken > 0)
            std::cout << "  RED player took " << piecesTaken << " piece(s)." << std::endl;

        if (doesBlackWin())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::BLACK_WINS;
        }

        if (doesRedWin())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::RED_WINS;
        }

        if (isItADraw())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::DRAW;
        }
    }
}

void Game::printNodes(Player player, std::string colorText)
{
    std::cout << std::endl;
    if (player.isMinimax)
    {
        std::cout << colorText << " Leaf Nodes: " << player.minimaxLeafNodes << std::endl;
        std::cout << colorText << " Expanded Nodes: " << player.minimaxExpandedNodes << std::endl;
        std::cout << colorText << " Total Nodes: " << player.getMinimaxTotalNodes() << std::endl;
    }
    else
    {
        std::cout << colorText << " Leaf Nodes: " << player.absearchLeafNodes << std::endl;
        std::cout << colorText << " Expanded Nodes: " << player.absearchExpandedNodes << std::endl;
        std::cout << colorText << " Total Nodes: " << player.getAbSearchTotalNodes() << std::endl;
    }
    std::cout << std::endl;
}

Color Game::changePlayer(Color currentPlayer)
{
    if (currentPlayer == Color::BLACK)
        return Color::RED;
    else
        return Color::BLACK;
}

bool Game::doesBlackWin()
{
    std::vector<Board::Move> redMoves = state.moveGen(Color::RED);
    return (redMoves.size() == 0);
}

bool Game::doesRedWin()
{
    std::vector<Board::Move> blackMoves = state.moveGen(Color::BLACK);
    return (blackMoves.size() == 0);
}

bool Game::isItADraw()
{
    if (redPlayer.getNumTurns() >= MAX_ALLOWED_TURNS ||
        blackPlayer.getNumTurns() >= MAX_ALLOWED_TURNS)
        return true;
    return false;
}


// CLASS ALGORITHM

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


// CLASS SIMULATION

/**
 * Simulation implementation
 * @author Borislav Sabotinov
 * 
 * Responsible for driving the simulation based on user preference provided in Main
 */

Simulation::Simulation()
{
    this->numGamesPlayed = 0;
}

Simulation::~Simulation()
{
}

/**
* Runs all games runs as delineated in Simulation.hpp
* R = redundant game simulated 
* 
* 1.  p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 1  R
* 2.  p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 2
* 3.  p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 3
* 4.  p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 1  R
* 5.  p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 2  R
* 6.  p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 3
* 7.  p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 1  R
* 8.  p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 2  R
* 9.  p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 3
* 10. p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 1
* 11. p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 2
* 12. p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 3
* 13. p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 1
* 14. p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 2
* 15. p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 3
* 16. p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 1
* 17. p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 2
* 18. p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 3
* 19. p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 1  R
* 20. p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 2  R
* 21. p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 3
* 22. p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 1
* 23. p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 2
* 24. p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 3
* 25. p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 1  R
* 26. p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 2  
* 27. p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 3  R
* 
*/
void Simulation::runFullSimulation()
{
    std::cout << "\033[0;32mRunning a FULL simulation!\033[0m" << std::endl;

    for (int depth = 2; depth <= 4; depth += 2)
    {
        std::cout << "Depth: " << depth << std::endl;

        for (int p1_alg = 0; p1_alg < 2; p1_alg++)
        {
            for (int p2_alg = 0; p2_alg < 2; p2_alg++)
            {
                for (int p1_eval = 1; p1_eval < 4; p1_eval++)
                {
                    for (int p2_eval = 1; p2_eval < 4; p2_eval++)
                    {
                        // omit duplicates to save time - we only care about unique runs
                        // player 1 need not use alg 2. It's already covered by Player 2
                        if (p1_alg == 2 && p2_alg == 1)
                            continue;

                        std::cout << "p1_alg: " << p1_alg << " p1_eval: " << p1_eval << " p2_alg: "
                                  << p2_alg << " p2_eval: " << p2_eval << std::endl;

                        Game *game = new Game(p1_alg, p1_eval, p2_alg, p2_eval, depth);
                        Game::GameOver endGameStatus = game->startGame();
                        numGamesPlayed++;
                        printGameResults(endGameStatus); 
                        delete game;

                    } // p2_eval
                }     // p1_eval
            }         // p2_alg
        }             // p1_alg
    }                 // depth
}

/**
 * Allows the user to run a specific, custom simulation based on their preference
 * @param int redPlayerAlg - If 1, minimax; if 0, AB Prune
 * @param int redPlayerEvalFunct - 1,2, 3, or 4
 * @param int blackPlayerAlg - If 1, minimax; if 0, AB Prune
 * @param int blackPlayerEvalFunct - 1,2, 3, or 4
 * @param int depth - 2 to 15, preferrably 2 or 4 as per project requirements
 */
void Simulation::runSpecificSimulation(int redPlayerAlg, int redPlayerEvalFunct, int blackPlayerAlg, int blackPlayerEvalFunct, int depth)
{
    std::cout << Pieces::ANSII_GREEN_START << "Running a SINGLE game, specific simulation!" << Pieces::ANSII_END << std::endl;

    // Validate algorithm selections
    if ((redPlayerAlg < 0 || redPlayerAlg > 1) && (blackPlayerAlg < 0 || blackPlayerAlg > 1))
        throw std::runtime_error("Error: algorithm may only be 1 (minimax-a-b) or 0 (ab-prune)!");

    // Validate evaluation function selections
    if ((redPlayerEvalFunct <= 0 || redPlayerEvalFunct > 4) && (blackPlayerEvalFunct <= 0 || blackPlayerEvalFunct > 4))
        throw std::runtime_error("Error: evalFunction may only be 1, 2, 3, or 4!");

    // Validate depth
    if (depth <= 1 || depth > 15)
        throw std::runtime_error("Error: depth must be > 1 and <= 15. ");

    Game *game = new Game(redPlayerAlg, redPlayerEvalFunct, blackPlayerAlg, blackPlayerEvalFunct, depth);
    Game::GameOver endGameStatus = game->startGame();

    printGameResults(endGameStatus);
    printGameConfig(redPlayerAlg, redPlayerEvalFunct, blackPlayerAlg, blackPlayerEvalFunct, depth);

    delete game;
}

/**
 * Helper function to print overall Game results, depending on who won
 * @param Game::GameOver endGameStatus - the game will return to us who was the winner (if any)
 */
void Simulation::printGameResults(Game::GameOver endGameStatus)
{
    if (endGameStatus == Game::GameOver::BLACK_WINS)
        printBlackWins();
    else if (endGameStatus == Game::GameOver::RED_WINS)
        printRedWins();
    else if (endGameStatus == Game::GameOver::DRAW)
        printDraw();
    else
        std::cout << "Oops, something went wrong!" << std::endl;
}

/**
 * runPlayerVsAISimulation - play a game with a human against a computer player
 * This is a fun option, it allows a person to test themselves against the AI
 * @param int playerAlg - what alg will the AI use
 * @param int playerEvalFunct - what eval function will AI use
 * @param int depth - what depth will AI use
 */
void Simulation::runPlayerVsAISimulation(int playerAlg, int playerEvalFunct, int depth)
{

    Player computerPlayer = Player(playerAlg, Color::RED, depth, playerEvalFunct);
    bool gameOver = false;
    int moveSelection;
    Color computerPlayerColor = Color::RED;
    Color humanPlayerColor = Color::BLACK;
    Color currentPlayerColor = humanPlayerColor;
    Board board;
    board.printBoard();

    while (!gameOver)
    {
        if (currentPlayerColor == humanPlayerColor) // BLACK
        {
            std::vector<Board::Move> blackMoves = board.moveGen(humanPlayerColor);
            // PRINT OUT BLACK'S MOVES
            std::cout << "Black's moves (b/B): ";
            for (int blackMoveIter = 0; blackMoveIter < blackMoves.size(); blackMoveIter++)
            {
                std::cout << "<" << blackMoveIter + 1 << "> " << blackMoves.at(blackMoveIter).startSquare;
                for (int destinationIter = 0; destinationIter < blackMoves.at(blackMoveIter).destinationSquare.size(); destinationIter++)
                {
                    std::cout << " to " << blackMoves.at(blackMoveIter).destinationSquare.at(destinationIter);
                }
                std::cout << ", ";
            }
            std::cout << std::endl;

            // GET HUMAN PLAYER MOVE
            bool isSelectionValid = false;
            while (!isSelectionValid)
            {
                std::cout << "Select BLACK (Human) move: ";
                std::cin >> moveSelection;
                if (moveSelection > blackMoves.size() || moveSelection < 0)
                {
                    std::cerr << "Out of range; please enter a valid choice!" << std::endl;
                }
                else
                {
                    board = board.updateBoard(blackMoves.at(moveSelection - 1), Color::BLACK);
                    currentPlayerColor = computerPlayerColor; // RED
                    isSelectionValid = true;
                }
            }
        }
        else if (currentPlayerColor == computerPlayerColor) // RED
        {
            // AI TAKES TURN AND PRINTS BOARD
            int numPiecesTakenByAI = computerPlayer.takeTurn(board);
            currentPlayerColor = humanPlayerColor; // BLACK
        }

        // CHECK WIN-LOSS CONDITIONS
        gameOver = didSomeoneWin(board); // if true, game will end
    }
    board.printBoard(); // print final board after someone wins
}

/**
 * At the end of the game, print the game configuration the user provided for ease of reference
 * @param int redPlayerAlg - If 1, minimax; if 0, AB Prune
 * @param int redPlayerEvalFunct - 1,2, 3, or 4
 * @param int blackPlayerAlg - If 1, minimax; if 0, AB Prune
 * @param int blackPlayerEvalFunct - 1,2, 3, or 4
 * @param int depth - 2 to 15, preferrably 2 or 4 as per project requirements
 */
void Simulation::printGameConfig(int redPlayerAlg, int redPlayerEvalFunct, int blackPlayerAlg, int blackPlayerEvalFunct, int depth)
{
    std::string algs[2] = {"Alpha-Beta-Search", "Minimax-Alpha-Beta"};
    std::cout << "Red player alg: " << algs[redPlayerAlg] << ", eval: " << redPlayerEvalFunct << std::endl;
    std::cout << "Black player alg: " << algs[blackPlayerAlg] << ", eval: " << blackPlayerEvalFunct << std::endl;
    std::cout << "Depth: " << depth << std::endl;
}

/**
 * didSomeoneWin - returns true if one player won, to break out of game loops
 * @param Board board
 * 
 * @return true if someone won, otherwise false
 */
bool Simulation::didSomeoneWin(Board board)
{
    bool isGameOver = false;
    std::vector<Board::Move> redMoves = board.moveGen(Color::RED);
    std::vector<Board::Move> blackMoves = board.moveGen(Color::BLACK);

    if (blackMoves.size() == 0)
    {
        isGameOver = true;
        printRedWins();
    }
    else if (redMoves.size() == 0)
    {
        isGameOver = true;
        printBlackWins();
    }

    return isGameOver;
}

/**
 * Helper function to display if Red wins
 */
void Simulation::printRedWins()
{
    std::cout << "\nRED WINS!!!" << std::endl;
    std::cout << "RED Player: ᕙ(-_-')ᕗ" << std::endl;
    std::cout << "But most importantly, BLACK looooses (boooo!)" << std::endl;
    std::cout << "BLACK Player: (╯°□°）╯︵ ┻━┻" << std::endl;
}

/**
 * Helper function to display if Black wins
 */
void Simulation::printBlackWins()
{
    std::cout << "\nBLACK WINS!!!" << std::endl;
    std::cout << "BLACK Player: ᕙ(-_-')ᕗ" << std::endl;
    std::cout << "But most importantly, RED looooses (boooo!)" << std::endl;
    std::cout << "RED Player: (╯°□°）╯ ︵ ┻━┻" << std::endl;
}

/**
 * Helper function to display if we run out of permitted turns and get a draw
 */
void Simulation::printDraw()
{
    std::cout << "DRAW!!!" << std::endl;
    std::cout << "Red - (ง •̀_•́)ง   ლ( `Д’ ლ) - Black" << std::endl;
    std::cout << "Mission FAILED...We'll get em next time!" << std::endl;
}

/** 
 * getNumGamesPlayed - returns a count of the number of games played in a simulation
 *
 */
int Simulation::getNumGamesPlayed()
{
    return numGamesPlayed;
}

/**
 * generateAnalsysisResults creates a table with results for analysis how many nodes were created, etc.
 */
void Simulation::generateAnalysisResults()
{
}



// MAIN APPLICATION DRIVER

/**
 * Main entry way into the application via main() method. 
 * @author Borislav Sabotinov
 * 
 * The user can display a help menu.
 * The user will be prompted to select how they wish to interact with the program. 
 * Available options are: 
 *    1. a full simulation, 
 *    2. partial (single game) simulation, 
 *    3. player vs. player, or
 *    4. player vs. AI. 
 */

// helper functions to make main() more readable and conscise
void printWelcomeMsg();
void printHelpMenu();
void printMainMenuOptions();
void executeRunBasedOnUserInput(int userInput, bool &isInputValid);
void getCustomSimUserInput(int &computerPlayerAlg, int &computerPlayerEval, int &depth);
void getCustomSimUserInput(int &playerOneAlg, int &playerOneEvalFunct, int &playerTwoAlg, int &playerTwoEvalFunct, int &depth);
void runManualGame();
void goodbye();

/**
* Main function, which serves as an entry point to the Checkers application. 
* @author Borislav Sabotinov
* 
* User may invoke a help menu by passing in either -h or -help as a CLI parameter when launching the program. 
* 
* @param int argc - count of the number of CLI arguments provided 
* @param char* argv[] - char array of the CLI arguments
* 
* @return EXIT_SUCCESS if the program completes successfully 
*/
int main(int argc, char *argv[])
{
    // display help menu
    if (argc >= 2)
    {
        std::string cliArg = argv[1];

        if (cliArg == "-h" || cliArg == "-help")
        {
            printHelpMenu();
            return EXIT_SUCCESS;
        }

        if (cliArg == "-nc") // disable color
        {
            // ANSII codes for colored text, to improve UI and readability
            Pieces::ANSII_BLUE_START = "";
            Pieces::ANSII_RED_START = "";
            Pieces::ANSII_RED_HIGH = "";
            Pieces::ANSII_END = "";
            Pieces::ANSII_GREEN_START = "";
            Pieces::ANSII_BLUE_COUT = "";
            Pieces::ANSII_RED_COUT = "";
            Pieces::ANSII_GREEN_COUT = "";
            Pieces::ANSII_YELLOW_COUT = "";
        }
        else if (cliArg == "-no") //
        {
            Pieces::ouputDebugData = 0;
        }
        else if (cliArg == "-ncno")
        {
            // ANSII codes for colored text, to improve UI and readability
            Pieces::ANSII_BLUE_START = "";
            Pieces::ANSII_RED_START = "";
            Pieces::ANSII_RED_HIGH = "";
            Pieces::ANSII_END = "";
            Pieces::ANSII_GREEN_START = "";
            Pieces::ANSII_BLUE_COUT = "";
            Pieces::ANSII_RED_COUT = "";
            Pieces::ANSII_GREEN_COUT = "";
            Pieces::ANSII_YELLOW_COUT = "";
            Pieces::ouputDebugData = 0;
        }
    }

    printWelcomeMsg();
    printMainMenuOptions();

    bool isInputValid = false;
    while (!isInputValid)
    {
        int userInput;
        std::cout << "Your choice " << Pieces::ANSII_BLUE_START << "(1, 2, 3, or 4)" << Pieces::ANSII_END << ": ";
        std::cin >> userInput;

        executeRunBasedOnUserInput(userInput, isInputValid);
    }

    goodbye();

    return EXIT_SUCCESS;
}

/**
 * Prints a welcome message to the console, along with the authors' names and emails. 
 */
void printWelcomeMsg()
{
    std::cout << Pieces::ANSII_GREEN_START << "Welcome to the Checkers AI Program." << Pieces::ANSII_END << std::endl;
    std::cout << "Authors: " << Pieces::ANSII_RED_START << " David Torrente (dat54@txstate.edu), Randall Henderson (rrh93@txstate.edu), "
              << "Borislav Sabotinov (bss64@txstate.edu)." << Pieces::ANSII_END << std::endl;
    std::cout << "Re-run this program with -h or -help CLI argument to see a help menu or refer to README for instructions."
              << std::endl;
    std::cout << std::endl;
}

/**
 * Prints a help message to the console if -h or -help are provided as CLI arguments when invoking the program
 */
void printHelpMenu()
{
    std::cout << "To use this program, please read the instructions below and re-launch." << std::endl;
    std::cout << "Additional details for building and execution are also available in the README file." << std::endl;
    std::cout << std::endl;
    std::cout << "Run with -nc for No Color, with -no for No Debug Output, or with -ncno for both No Color AND No Debug Output." << std::endl;
    std::cout << "When executing the program, you will be prompted to enter the algorithm and evaluation "
              << "function for the simulation." << std::endl;
    std::cout << "Please follow the instructions on the screen - if you do not care for any specific custom "
              << "configuration, simply select Run All." << std::endl;
    std::cout << "This will bypass everything and simply run the simulation for all algorithms and all "
              << "evaluation functions sequentially."
              << "\nThis is the most common and preferred option." << std::endl;
}

/**
 * Prints the main menu with option codes. 
 */
void printMainMenuOptions()
{
    std::cout << "NOTE: If 1 is selected below, you will NOT be prompted further for any eval function or algorithm. "
              << "All will be simulated in order." << std::endl;
    std::cout << "For RED player, r = MAN and R = KING; for BLACK player, b = MAN and B = KING." << std::endl;
    std::cout << std::endl;

    std::cout << "Choose a game mode below: " << std::endl;
    std::cout << "     1. Full Simulation (recommended)" << std::endl;
    std::cout << "     2. Single Custom Simulation" << std::endl;
    std::cout << "     3. Player vs Player (manual game)" << std::endl;
    std::cout << "     4. Player vs AI (will be asked to select AI playstyle)" << std::endl;
    std::cout << "     Ctrl + C to terminate program at any time." << std::endl;
    std::cout << std::endl;
}

/**
 * Given the user's choice in the main menu, execute the program accordingly.
 */
void executeRunBasedOnUserInput(int userInput, bool &isInputValid)
{
    Simulation *simulation = new Simulation();
    switch (userInput)
    {
    case 1: // full sim
        isInputValid = true;
        simulation->runFullSimulation();
        std::cout << "# of Games Played: " << simulation->getNumGamesPlayed() << std::endl;
        break;
    case 2: // one custom sim
        isInputValid = true;
        int playerOneAlg, playerOneEvalFunct, playerTwoAlg, playerTwoEvalFunct, depth;
        getCustomSimUserInput(playerOneAlg, playerOneEvalFunct, playerTwoAlg, playerTwoEvalFunct, depth);
        simulation->runSpecificSimulation(playerOneAlg, playerOneEvalFunct, playerTwoAlg, playerTwoEvalFunct, depth);
        break;
    case 3: // player vs. player
        isInputValid = true;
        runManualGame();
        break;
    case 4: // player vs. ai
        isInputValid = true;
        getCustomSimUserInput(playerOneAlg, playerOneEvalFunct, depth);
        simulation->runPlayerVsAISimulation(playerOneAlg, playerOneEvalFunct, depth);
        break;
    default:
        std::cerr << "Invalid option selected! Valid choices are 1, 2, 3, and 4" << std::endl;
    }
}

/**
 * For a custom simulation, obtains user input and returns via parameters
 * @param int &computerPlayerAlg
 * @param int &computerPlayerEval
 * @param int &depth
 * 
 * @return int &computerPlayerAlg passed in by reference
 * @return int &computerPlayerEval passed in by reference
 * @return int &depth passed in by reference
 */
void getCustomSimUserInput(int &computerPlayerAlg, int &computerPlayerEval, int &depth)
{
    std::cout << "Please select the type of simulation you wish to run by entering in it's number." << std::endl;
    std::cout << "1. Run Minimax-A-B algorithm" << std::endl;
    std::cout << "0. Run Alpha-Beta-Search algorithm" << std::endl;

    // PLAYER CHOICES
    std::cout << "Algorithm for RED - Player 1 " << Pieces::ANSII_BLUE_START << "(1 for minimax, 0 for ab-Search)" << Pieces::ANSII_END << ":";
    std::cin >> computerPlayerAlg;
    std::cout << std::endl;
    std::cout << "Evaluation for RED - Player 1 " << Pieces::ANSII_BLUE_START << "(1 (David's), 2 (Randy's), 3 (Boris'), 4 (returns 1st available move))" << Pieces::ANSII_END << ": ";
    std::cin >> computerPlayerEval;

    // DEPTH
    std::cout << "Enter the depth for the search tree " << Pieces::ANSII_BLUE_START << "(2 or 4 recommended; min = 2, max = 15)" << Pieces::ANSII_END << ": ";
    std::cin >> depth;
}

/**
 * getCustomSimUserInput is a helper function to obtain the algorithm and eval function for Player 1 and Player 2
 * One game will be simulated only using this input. 
 */
void getCustomSimUserInput(int &playerOneAlg, int &playerOneEvalFunct, int &playerTwoAlg, int &playerTwoEvalFunct, int &depth)
{
    std::cout << "Please select the type of simulation you wish to run by entering in it's number." << std::endl;
    std::cout << "1. Run Minimax-A-B algorithm" << std::endl;
    std::cout << "0. Run Alpha-Beta-Search algorithm" << std::endl;

    // PLAYER ONE CHOICES
    std::cout << "Algorithm for RED - Player 1 " << Pieces::ANSII_BLUE_START << "(1 for minimax, 0 for ab-Search)" << Pieces::ANSII_END << ":";
    std::cin >> playerOneAlg;
    std::cout << std::endl;
    std::cout << "Evaluation for RED - Player 1 " << Pieces::ANSII_BLUE_START << "(1 (David's), 2 (Randy's), 3 (Boris'), 4 (returns 1st available move))" << Pieces::ANSII_END << ": ";
    std::cin >> playerOneEvalFunct;

    // PLAYER TWO CHOICES
    std::cout << "Algorithm for BLACK - Player 2 " << Pieces::ANSII_BLUE_START << "(1 for minimax, 0 for ab-Search)" << Pieces::ANSII_END << ": ";
    std::cin >> playerTwoAlg;
    std::cout << std::endl;
    std::cout << "Evaluation for BLACK - Player 2 " << Pieces::ANSII_BLUE_START << "(1 (David's), 2 (Randy's), 3 (Boris'), 4 (returns 1st available move))" << Pieces::ANSII_END << ": ";
    std::cin >> playerTwoEvalFunct;

    // DEPTH
    std::cout << "Enter the depth for the search tree " << Pieces::ANSII_BLUE_START << "(2 or 4 recommended; min = 2, max = 15)" << Pieces::ANSII_END << ": ";
    std::cin >> depth;
}

/**
 * runManualGame function provides a human user the ability to play checkers with another human. 
 * It is a manual, input based game where a player must enter the number of the turn they wish to execute.
 * @author David Torrente
 * @author Borislav Sabotinov 
 */
void runManualGame()
{
    bool gameOver = false;
    int moveSelection;
    Color currentPlayer = Color::BLACK;
    std::vector<Board::Move> redMoves;
    std::vector<Board::Move> blackMoves;
    Board board;
    board.printBoard();

    while (!gameOver)
    {
        std::vector<Board::Move> redMoves = board.moveGen(Color::RED);
        std::vector<Board::Move> blackMoves = board.moveGen(Color::BLACK);
        std::cout << std::endl;
        std::cout << "Red (r = MAN / R = KING) moves: ";

        for (int redMoveIter = 0; redMoveIter < redMoves.size(); redMoveIter++)
        {
            std::cout << "<" << redMoveIter + 1 << "> " << redMoves.at(redMoveIter).startSquare;
            for (int destinationIter = 0; destinationIter < redMoves.at(redMoveIter).destinationSquare.size(); destinationIter++)
            {
                std::cout << " to " << redMoves.at(redMoveIter).destinationSquare.at(destinationIter);
            }
            std::cout << ", ";
        }
        std::cout << std::endl;

        std::cout << "Black (b = MAN / B = KING) moves: ";
        for (int blackMoveIter = 0; blackMoveIter < blackMoves.size(); blackMoveIter++)
        {
            std::cout << "<" << blackMoveIter + 1 << "> " << blackMoves.at(blackMoveIter).startSquare;
            for (int destinationIter = 0; destinationIter < blackMoves.at(blackMoveIter).destinationSquare.size(); destinationIter++)
            {
                std::cout << " to " << blackMoves.at(blackMoveIter).destinationSquare.at(destinationIter);
            }
            std::cout << ", ";
        }
        std::cout << std::endl;

        bool isSelectionValid = false;
        if (currentPlayer == Color::RED)
        {
            while (!isSelectionValid)
            {
                std::cout << "Select RED move: ";
                std::cin >> moveSelection;
                if (moveSelection > redMoves.size() || moveSelection < 0)
                {
                    std::cerr << "Out of range; please enter a valid choice!" << std::endl;
                }
                else
                {
                    board = board.updateBoard(redMoves.at(moveSelection - 1), Color::RED);
                    currentPlayer = Color::BLACK;
                    isSelectionValid = true;
                }
            }
        }
        else  // BLACK's turn
        {
            isSelectionValid = false;
            while (!isSelectionValid)
            {
                std::cout << "Select BLACK move: ";
                std::cin >> moveSelection;
                if (moveSelection > blackMoves.size() || moveSelection < 0)
                {
                    std::cerr << "Out of range; please enter a valid choice!" << std::endl;
                }
                else
                {
                    board = board.updateBoard(blackMoves.at(moveSelection - 1), Color::BLACK);
                    currentPlayer = Color::RED;
                    isSelectionValid = true;
                }
            }
        }

        board.printBoard();
        gameOver = Simulation::didSomeoneWin(board);
    }
}

/**
 * Displays a compatible, pleasant graphic to the user as a goodbye
 */
void goodbye()
{
    std::cout << std::endl;
    std::cout << Pieces::ANSII_BLUE_START << " __  __  __  __  __     __  " << Pieces::ANSII_END << std::endl;
    std::cout << Pieces::ANSII_BLUE_START << "/ _ /  \\/  \\|  \\|__)\\_/|_ | " << Pieces::ANSII_END << std::endl;
    std::cout << Pieces::ANSII_BLUE_START << "\\__)\\__/\\__/|__/|__) | |__. " << Pieces::ANSII_END << std::endl;
    std::cout << Pieces::ANSII_BLUE_START << "                            " << Pieces::ANSII_END << std::endl;
    std::cout << std::endl;
}
