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