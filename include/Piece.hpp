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


enum class Color { RED, BLACK };

class Piece 
{

private: 
    Color color; 
    int row;
    int col;
    bool isKing;

public: 
    bool isKing();
    Color getColor();
    void setPosition(int row, int col);
    void makeKing();
};