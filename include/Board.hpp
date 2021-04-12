#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <queue>
#include <string> 

#include "Pieces.hpp"

/**
 * Header definition for class Board. 
 * 
 * This class represents a virtual checkers board. 
 * Each player should have a white square on the bottom right corner. 
 * Each player puts pieces on the 12 dark squares in the first three closest rows. 
 * Each of the three rows should have 4 pieces. 
 */

class Board
{

public:

	struct Move
	{
		int startSquare;
		std::vector<int> destinationSquare;
		std::vector<int> removalSquare;
	};

	struct BoardMoveTable
	{
		// Jumps will always come in pairs. The even
		// values are the jumps, the odd values are the 
		// spaces jumped. 
		std::vector<int> jumps;
		std::vector<int> removals;
		std::vector<int> moves;
	};

	Board();
	~Board();
	static void InitializeMoveTable();
	bool movePiece(Color color, std::string move);
	std::vector<Move> moveGen(Color color);
	
	void printBoard() const;
	Board updateBoard(Move move, Color color);

	int getNumRegularPieces(Color color);
	int getNumKingPieces(Color color);
	int getNumPlayerTotalPieces(Color color);



	int squareToRow(int square) const;
	int squareToColumn(int square) const;


	std::vector<Move> getJumpsForPiece(Color color, int square, Pieces* playerPieces, Pieces* opponentPieces);
	std::vector < Board::Move> getJumpsForPiece2(Color color, int piece, Board::Move move, std::vector<Board::Move>& totalMoves, Board board);
	std::vector<Move> getMovesForPiece(Color color, int square, Pieces* playerPieces, Pieces* opponentPieces);


private:

	Pieces blackPieces;
	Pieces redPieces;

	int numRedPieces;
	int numBlackPieces;

	static BoardMoveTable boardMoveTable[33];
	void printHelperBoardRow(int row);

};

#endif