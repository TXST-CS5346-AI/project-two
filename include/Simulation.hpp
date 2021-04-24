#ifndef SIMULATION_H
#define SIMULATION_H

#include "Board.hpp"
#include "Game.hpp"

/**
 * Header definition for class Simulation. 
 * @author Borislav Sabotinov
 * 
 * This class is responsible for managing the series of Games that AI players will play for Project Two. 
 * It persists during the execution of the program and keeps track of the number of games played. 
 * It also allows to aggregate and print simulation analysis details. 
 * 
 * There are THREE (3) evaluation functions, one for each team member; two algorithms (minimax and Alpha-Beta). 
 * The simulation will execute each in turn. 
 * 
 * Fifteen runs with depth 2:
 *    1. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #1
 *    2. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *    3. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 * 
 *    4. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #1
 *    5. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #2
 *    6. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 * 
 *    7. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #1
 *    8. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #2
 *    9. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #3
 * 
 *   10. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #2
 *   11. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #3
 *   12. MinMax-A-B with Evl. Function #2  Verses MinMax-A-B with Evl. Function #3
 * 
 *   13. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *   14. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 *   15. Alpha-Beta with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 * 
 * Fifteen runs with depth 4:
 *    1. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #1
 *    2. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *    3. MinMax-A-B with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 * 
 *    4. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #1
 *    5. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #2
 *    6. MinMax-A-B with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 * 
 *    7. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #1
 *    8. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #2
 *    9. MinMax-A-B with Evl. Function #3  Verses Alpha-Beta with Evl. Function #3
 * 
 *   10. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #2
 *   11. MinMax-A-B with Evl. Function #1  Verses MinMax-A-B with Evl. Function #3
 *   12. MinMax-A-B with Evl. Function #2  Verses MinMax-A-B with Evl. Function #3
 * 
 *   13. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #2
 *   14. Alpha-Beta with Evl. Function #1  Verses Alpha-Beta with Evl. Function #3
 *   15. Alpha-Beta with Evl. Function #2  Verses Alpha-Beta with Evl. Function #3
 *
 * TOTAL 30 RUNS/GAMES WILL BE SIMULATED. 
 */ 

class Simulation 
{

private: 
    int numGamesPlayed; 

    // runs only games using Minimax algorithm
    void runMinimaxOnly();
   
    // runs only games using AB Prune algorithm
    void runABPruneOnly();

public: 
    Simulation(); // constructor
    ~Simulation(); // destructor

    // runs all games runs as delineated above
    void runFullSimulation();
    
    // public method for specific simulations
    void runSpecificSimulation(int playerOneAlg, int playerOneEvalFunct, int playerTwoAlg, int PlayerTwoEvalFunct, int depth);

    // public method for player vs AI simulation
    void runPlayerVsAISimulation(int playerAlg, int playerEvalFunct, int depth); 

    // helper function to determine winner and break out of game loop
    static bool didSomeoneWin(Board board);
    
    // returns a count of the number of games played in a simulation
    // each of the 3 run functions. 
    int getNumGamesPlayed();

    // creates a table with results for analysis. 
    // how many nodes were created, etc. 
    void generateAnalysisResults();

    void printGameConfig(int redPlayerAlg, int redPlayerEvalFunct, int blackPlayerAlg, int blackPlayerEvalFunct, int depth); 
    void printGameResults(Game::GameOver endGameStatus); 

    // helper print methods
    static void printBlackWins(); 
    static void printRedWins(); 
    static void printDraw(); 

};

#endif // !SIMULATION_H