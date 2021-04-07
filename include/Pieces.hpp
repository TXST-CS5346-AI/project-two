#ifndef PIECE_H
#define PIECE_H

#include <string> 

/**
 * Header definition for class Piece. 
 * A "piece" is a checker piece used in the game of checkers. 
 * It may be either red or black. Color is an immutable property. 
 * It is set only once when the piece is created and placed on the board. 
 * 
 * A standard checkers piece may move only forwards in a diagonal manner. If the piece reaches the 
 * opposite end of the board, it becomes a "king" piece. This means it may now move backwards diagonally. 
 * A piece's position is determined by it's board placement - row and column number.  
 * 
 * A piece from player A may take a piece from player B by "jumping" over it diagonally if the square 
 * on the opposite side is empty. Jumps are MANDATORY - a player must make a jump if one is available to them. 
 * If multiple jumps are available, the player simply selects among the choices. 
 * 
 * A jump may be continuous - the piece must continue capturing enemy pieces if more are available for capture. 
 */

enum class Color { RED = 1, BLACK = -1 };

class Pieces 
{

private: 
    // 64 bits; lower 32 used for board position. Upper 32 bits used to determine if a piece is king
    long long pieces; 
public: 
    Pieces(); // constructor
    ~Pieces(); // destructor 

    bool isKing(int position); // takes position, adds 32 and returns if what is in position is king (or not)
    void makeKing(int poisition); // sets member variable isKing to true. Takes position adds 32 and sets the bit it to 1
    int getCount(); // counts the num of 1s in the lower 32 bits
};

#endif // !PIECE_H