#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string> 

#include "Piece.hpp"

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

private: 
    std::vector<Piece> board;
    int numRedPieces;
    int numBlackPieces;

public: 
    Board(); // constructor
    ~Board(); // destructor

    int getNumRedPieces();
    int getNumBlackPieces();
    
    // prints an ASCII representation of the 2D 8x8 checkers board
    void printBoard();

    /* provides a string describing the move(s), which needs to be parsed
    * E.g. - "1>2 5>10" means Piece in position 1 moves to position 2. Piece in 
    * position 5 moves to position 10. It can be sequential too: "1>5>9" means Piece 1
    * jumps to 5 then to 9
    */
   void updateBoard(std::string moveset); 
};

#endif // !BOARD_H