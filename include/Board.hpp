#ifndef BOARD_H
#define BOARD_H

#include <vector>
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

struct BoardMoveTable
{
	// Jumps will always come in pairs. The even
	// values are the jumps, the odd values are the 
	// spaces jumped. 
	std::vector<int> jumps;
	std::vector<int> moves;
};

class Board
{

public:

	Board();
	~Board();
	static void InitializeMoveTable();
	bool movePiece(Color color, std::string move);
	std::string moveGen(Color color);
	std::string getMovesForPiece(Color color, int square, Pieces* playerPieces, Pieces* opponentPieces);
	int getSquareJumped(int sourceSquare, int destinationSquare);

	void printBoard() const;
	Board updateBoard(std::string move);


	int getNumRedPieces() const { return numRedPieces; }
	int getNumBlackPieces() const { return numBlackPieces; }

	int squareToRow(int square) const;
	int squareToColumn(int square) const;

private:

	Pieces blackPieces;
	Pieces redPieces;

	int numRedPieces;
	int numBlackPieces;

	static BoardMoveTable boardMoveTable[33];

};

#endif