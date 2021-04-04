#include "Player.hpp"
#include "Board.hpp"

/**
 * Header definition for class Game. 
 * The Game class represents a checkers Game. 
 * 
 * A game consists of a board, two players (red and black), and 
 * 12 checkers pieces for each player. 
 * 
 * A game ends when either player loses all their pieces or is blocked and 
 * has no further moves available. 
 */ 

class Game
{

private: 
    Board state;
    Player redPlayer;
    Player blackPlayer;

public: 
    void startGame();
};