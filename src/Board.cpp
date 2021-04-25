#include "Board.hpp"
#include "Pieces.hpp"
#include <iostream>
#include <iomanip>

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

		long long* playerPieces;

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
			if(((*playerPieces >> iter) & 1) == 1)
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

	long long* playerPieces;

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
	
	Pieces* playerPieces;
	Pieces* opponentPieces;

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
std::vector<Board::Move> Board::getJumpsForPiece(Color color, int piece, Pieces* playerPieces, Pieces* opponentPieces)
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
void Board::getJumpsForPieceRec(Color color, Board::Move move, std::vector<Board::Move>& totalMovesAccumulator,  Board board, bool wasKingPriorMove)
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
std::vector<Board::Move> Board::getMovesForPiece(Color color, int piece, Pieces* playerPieces, Pieces* opponentPieces)
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

			if (((redPieces.pieces >> squareOffset) & 1) ==1)
			{
				if (redPieces.isKing(squareOffset + 1))
				{
					std::cout << Pieces::ANSII_RED_HIGH << " R " << Pieces::ANSII_END << "|";
				}
				else
				{
					std::cout << Pieces::ANSII_RED_HIGH << " r "<< Pieces::ANSII_END << "|";
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
					std::cout << Pieces::ANSII_BLUE_START <<" b "<< Pieces::ANSII_END <<"|";
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
			std::cout << std::setfill('_') << std::setw(3) << squareOffset + 1 <<"|";
		
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
	
	if (((playerPieces.pieces >> (position -1)) & 1) == 1)
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

	Pieces * playerPieces;
	Pieces * opponentPieces;


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