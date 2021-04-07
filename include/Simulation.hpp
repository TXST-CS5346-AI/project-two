#ifndef SIMULATION_H
#define SIMULATION_H

/**
 * Header definition for class Simulation. 
 * 
 * This class is responsible for managing the series of Games that AI players will play for Project Two. 
 * It persists during the execution of the program and keeps track of the number of games played. 
 * It also allows to aggregate and print simulation analysis details. 
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
 *    10.
 *    11.
 *    12. 
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

class Simulation 
{

private: 
    int numGamesPlayed; 

public: 
    Simulation(); // constructor
    ~Simulation(); // destructor

    // runs all 18 runs as delineated above
    void runFullSimulation();
    
    // runs only 6 games using only Minimax algorithm
    void runMinimaxOnly();
   
    // runs only 6 games using only AB Prune algorithm
    void runABPruneOnly();
    
    // returns a count of the number of games played in a simulation
    // each of the 3 run functions. 
    int getNumGamesPlayed();

    // creates a table with results for analysis. 
    // how many nodes were created, etc. 
    void generateAnalysisResults(); 

};

#endif // !SIMULATION_H