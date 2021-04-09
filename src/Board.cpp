#include "Board.hpp"
#include "Pieces.hpp"
#include <iostream>

// Forward declare the static data member.
BoardMoveTable Board::boardMoveTable[33];

Board::Board()
{

	// Assign the proper pieces to the player, either red or black.
	redPieces = Pieces(RED);
	blackPieces = Pieces(BLACK);
}

Board::~Board()
{

}

std::vector<Move> Board::moveGen(Color color)
{
	std::vector<Move> totalMoves;
	std::vector<Move> returnedMoves;
	
	Pieces* playerPieces;
	Pieces* opponentPieces;

	int bitOffset = 0;

	if (color == RED)
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
		
		if (((playerPieces->pieces) >> bitOffset) % 2 != 0)
		{
			// Check for possible jump mpves first.
			returnedMoves = getJumpsForPiece(color, pieceIter, playerPieces, opponentPieces);
			totalMoves.insert(totalMoves.end(), returnedMoves.begin(), returnedMoves.end());
		}
	}

	// Go through all 32 squares and see if it is one
	// of the appropriate pieces belonging to player.
	if (totalMoves.size() == 0)
	{
		for (int pieceIter = 1; pieceIter <= 32; pieceIter++)
		{
			// The offset is used to align to 0 - 31, but 
			// the board in checkers is 1 - 32. Use an offset
			// here to align to the position bits properly.
			bitOffset = pieceIter - 1;

			if (((playerPieces->pieces) >> bitOffset) % 2 != 0)
			{
				// Check for non jump moves here.
				returnedMoves = getMovesForPiece(color, pieceIter, playerPieces, opponentPieces);
				totalMoves.insert(totalMoves.end(), returnedMoves.begin(), returnedMoves.end());
			}
		}
	}
	return totalMoves;
}
std::vector<Move> Board::getJumpsForPiece(Color color, int piece, Pieces* playerPieces, Pieces* opponentPieces)
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


	// Check if a jump position is open for this piece. This goes through all of the jumps.
	for (int jumpIter = 0; jumpIter < Board::boardMoveTable[piece].jumps.size(); jumpIter++)
	{
		// Get the position of the jump, reduce it by one for an offset. Note
		// that while it is one less than the position, the direction check still works
		// since a jump will always be greater than the distance needed to determine 
		// direction.
		bitOffset = (Board::boardMoveTable[piece].jumps.at(jumpIter) - 1);

		// Check to see which direction it is going, and if it can go that direction.
		// King goes both ways		Red not a king goes "down"            Black not a king goes up
		if (isKing || ((piece - bitOffset) < 0 && color == RED) || ((piece - bitOffset) > 0 && color == BLACK))
		{
			// Combine both bit fields into one and check if the space is empty.
			if (((playerPieces->pieces | opponentPieces->pieces) >> bitOffset) % 2 == 0)
			{
				// If it is open, get the space between. We will need to check it.
				// Again with the -1 to help with the offset
				squareJumped = Board::boardMoveTable[piece].removals.at(jumpIter) - 1;

				if ((opponentPieces->pieces >> squareJumped) % 2 != 0)
				{
					//yup, we are good!!!! this means that an opponent was in this spot.
					move.startSquare = piece;
					move.destinationSquare.push_back(bitOffset + 1);
					moves.push_back(move);
					move.destinationSquare.clear();
				}
			}
		}
	}

	return moves;
}


// Knows it has a proper piece by the time it gets here. It will return the moves
// for the piece in this square.
std::vector<Move> Board::getMovesForPiece(Color color, int piece, Pieces* playerPieces, Pieces* opponentPieces)
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

	// If there were possible jumps, they need to be taken. Do not generate
	// any non jumping moves as per checkers rules.
		for (int moveIter = 0; moveIter < Board::boardMoveTable[piece].moves.size(); moveIter++)
		{
			bitOffset = Board::boardMoveTable[piece].moves.at(moveIter) - 1;
			if (isKing || ((piece - bitOffset) < 0 && color == RED) || ((piece - bitOffset) > 0 && color == BLACK))
			{
				if (((playerPieces->pieces | opponentPieces->pieces) >> bitOffset) % 2 == 0)
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

int Board::squareToRow(int square) const
{
	return  (square / 4) + 1;
}

int Board::squareToColumn(int square) const
{
	int column;
	column = square % 4;

	// Need to make 0 actually come after 3,
	// so just set it to 4 (the last column)
	if (0 == column)
	{
		column = 4;
	}

	return column;
}

bool Board::movePiece(Color color, std::string move)
{

	return true;
}

void Board::printBoard() const
{

	// The board toggles back and forth in regards to the positioning 
	// of pieces. This either adds the offset to the beginning of the 
	// row, or to the end of it. Odd rows start with a space,
	// Even rows end with a space.
	bool oddRow = true;

	// A basic bar to go across the top. Cosmetic.
	std::cout << "_________________" << std::endl;

	for (int rowIter = 0; rowIter <= 7; rowIter++)
	{
		std::cout << "|";


		for (int colIter = 0; colIter <= 3; colIter++)
		{
			if (oddRow)
			{
				std::cout << "_|";
			}

			if ((redPieces.pieces >> (rowIter * 4 + colIter)) % 2 != 0)
			{
				std::cout << "X|";
			}
			else if ((blackPieces.pieces >> (rowIter * 4 + colIter)) % 2 != 0)
			{
				std::cout << "O|";
			}
			else
			{
				std::cout << "_|";
			}

			if (!oddRow)
			{
				std::cout << "_|";
			}
		}

		// Toggle between even and odd rows for the offset.
		oddRow = !oddRow;
		std::cout << std::endl;
	}

}


// Note that this updates the complete board for both sides.
// This will always only be a single move. It will need to be processed before this.
Board Board::updateBoard(Move move, Color color)
{

	// Remove start pos
	// Position in final destination spot - probably check if it needs to be kinged here.
	// Remove all jumped pieces
	Board updatedBoard;
	updatedBoard.blackPieces = blackPieces;
	updatedBoard.redPieces = redPieces;
	


	return updatedBoard;
}

void Board::InitializeMoveTable()
{

	// In order to support faster look up, 
	// The index of the position is used as
	// the initial key to an index. Each
	// position has only a few moves,
	// even less if there is a mandatory jump.
	// The table is big, but never changes.

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