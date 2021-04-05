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
 * There are THREE (3) evaluation functions, one for each team member; two algorithms (minimax and Alpha-Beta). 
 * The simulation will execute each in turn. 
 * 
 * Nine runs with depth 2:
 *    1. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #1
 *    2. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #2
 *    3. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #3
 *    4. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #2
 *    5. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #3
 *    6. MinMax-A-B with Evl. Function #2  Verses MinMax-A-B with Evl. Function #3
 *    7. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *    8. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 *    9. Alpha-Beta with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 * 
 * Nine runs with depth 4:
 *    1. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #1
 *    2. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #2
 *    3. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #3
 *    4. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #2
 *    5. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #3
 *    6. MinMax-A-B with Evl. Function #2  Verses MinMax-A-B with Evl. Function #3
 *    7. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *    8. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 *    9. Alpha-Beta with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 *
 * TOTAL 18 RUNS WILL EXECUTE BY SIMULATION. 
 */ 

class Game
{

private: 
    Board state;
    Player redPlayer;
    Player blackPlayer;

public: 
    // The only initialization function needed, as the game will 
    // be played automatically by 2 AI players (MIN and MAX). 
    void startGame();
};