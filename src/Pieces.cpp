#include "Pieces.hpp"

// ANSII codes for colored text, to improve UI and readability
std::string Pieces::ANSII_BLUE_START     = "\033[0;30;46m";
std::string Pieces::ANSII_RED_START      = "\033[0;31m";
std::string Pieces::ANSII_RED_HIGH       = "\033[9;37;41m";
std::string Pieces::ANSII_END            = "\033[0m";
std::string Pieces::ANSII_GREEN_START    = "\033[0;32m";
std::string Pieces::ANSII_BLUE_COUT      = "\033[0;30;46m";
std::string Pieces::ANSII_RED_COUT       = "\033[41;1m";
std::string Pieces::ANSII_GREEN_COUT     = "\033[0;30;42m";
std::string Pieces::ANSII_YELLOW_COUT    = "\033[30;48;5;3m";


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
	else // black
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

