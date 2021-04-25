#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string> 

#include "Pieces.hpp"


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

	std::vector<Move> getJumpsForPiece(Color color, int square, Pieces* playerPieces, Pieces* opponentPieces); 
	std::vector<Move> getMovesForPiece(Color color, int square, Pieces* playerPieces, Pieces* opponentPieces);

	Pieces getPlayerPieces(Color color);
	Pieces getOpponentPieces(Color color);

	static BoardMoveTable boardMoveTable[33];

private:

	Pieces blackPieces;
	Pieces redPieces;


	void getJumpsForPieceRec(Color color, Board::Move move, std::vector<Board::Move>& totalMoves, Board board, bool wasKingPriorMove);

};

#endif