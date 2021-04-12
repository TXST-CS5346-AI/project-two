#include "Pieces.hpp"



Pieces::Pieces()
{

}

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

	if (color == Color::RED) // red
		//pieces = 19455;
		pieces = 4095; // Initial board state
	else // black
		//pieces = 4291952640;  
		pieces = 4293918720; // Initial board state
}


bool Pieces::isKing(int position) const
{
	// NOTE: it is 31, since the offset is 0 - 31
	// Could have written +32 -1, but better to just combine.
	// Checks the high bit (33 - 64) which is aligned with 
	// the position bit (1 - 32).
	int kingBit = position + 31;

	bool isKing = false;

	if ((pieces >> kingBit) % 2 != 0)
	{
		isKing = true;
	}

	return isKing;
}

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

