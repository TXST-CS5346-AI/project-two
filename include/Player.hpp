#ifndef PLAYER_H
#define PLAYER_H

#include "Piece.hpp"

/** 
 * Header definition for class Player. 
 * 
 * The Player class defines a virtual player, in the case of this program an AI. 
 * No human players are used in this game - meaning no manual input is sought from the user. 
 * Project 2, Requirement #7: "Your program should play with a computer to a computer."
 * 
 */  

class Player
{

private: 
    Color color; // represents player's color in the game, either RED or BLACK
    int numPieces; // how many pieces does Player have left
    int numPiecesTaken; // Player's current score based on captured enemy pieces
    bool isMinimax; // if false use AB Prune, if true use Minimax. Allows control over alg player uses
public: 
    Player(); // constructor
    ~Player(); // destructor

    Player(Color color); // overloaded constructor to set player color, which is IMMUTABLE

    Board takeTurn();
    int getNumPieces();
    int getNumPiecesTaken();
    // player can not gain additional pieces during the game, only lose them
    void decreasePlayerPieces(int piecesToSubtract);
};

#endif // !PLAYER_H