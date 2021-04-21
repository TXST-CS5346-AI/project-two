#ifndef PIECES_H
#define PIECES_H

#include <string> 

/**
 * Header definition for class Piece. 
 * A "piece" is a checker piece used in the game of checkers. 
 * It may be either red or black. Color is an immutable property. 
 * It is set only once when the piece is created and placed on the board. 
 * 
 * A standard checkers piece may move only forwards in a diagonal manner. If the piece reaches the 
 * opposite end of the board, it becomes a "king" piece. This means it may now move backwards diagonally. 
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
public: 
    
    Pieces(); // constructor
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

    bool isKing(int position) const; // takes position, adds 32 and returns if what is in position is king (or not)
    void setKing(int poisition, bool toKing); // sets member variable isKing to true. Takes position adds 32 and sets the bit it to 1
    
    long long pieces;
};

#endif // !PIECES_H


