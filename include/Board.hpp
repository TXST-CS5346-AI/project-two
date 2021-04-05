#include <vector>
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
    std::vector<std::vector<Piece>> board;
    int numRedPieces;
    int numBlackPieces;

public: 
    int getNumRedPieces();
    int getNumBlackPieces();
    // prints an ASCII representation of the 2D 8x8 checkers board
    void printBoard();

};