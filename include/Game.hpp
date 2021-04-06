#ifndef GAME_H
#define GAME_H

#include "Player.hpp"
#include "Board.hpp"

/**
 * Header definition for class Game. 
 * 
 * The Game class represents a checkers Game. 
 * 
 * A game consists of a board, two players (red and black), and 
 * 12 checkers pieces for each player. 
 * 
 * A game ends when either player loses all their pieces or is blocked and 
 * has no further moves available. 
 * 
 * The startGame() function will trigger the process and no input from the user is required. 
 * Red and Black players will be created. Each player will be given 12 checker pieces. 
 * The pieces will be appropriately placed on the board. 
 * Each player will execute their strategy. 
 * 

 */ 

class Game
{

private: 
    Board state;
    Player redPlayer;
    Player blackPlayer;

public: 
    Game(); // constructor
    ~Game(); // destructor

    // The only initialization function needed, as the game will 
    // be played automatically by 2 AI players (MIN and MAX). 
    void startGame();
};

#endif // !GAME_H