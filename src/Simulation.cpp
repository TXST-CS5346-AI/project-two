#include "Simulation.hpp"

#include <iostream>

/**
 * Simulation implementation
 * @author Borislav Sabotinov
 * 
 * 
 */ 

Simulation::Simulation()
{

}

Simulation::~Simulation()
{

}

/*
* Runs all games runs as delineated in Simulation.hpp
*/
void Simulation::runFullSimulation()
{
    std::cout << "Running a full simulation!" << std::endl; 
}

/*
* Runs only games using Minimax algorithm
*/
void Simulation::runMinimaxOnly()
{

}

/* 
* Runs only games using AB Prune algorithm
*/
void Simulation::runABPruneOnly()
{

}

// returns a count of the number of games played in a simulation
// each of the 3 run functions. 
int Simulation::getNumGamesPlayed()
{
    return 0; 
}

// creates a table with results for analysis. 
// how many nodes were created, etc. 
void Simulation::generateAnalysisResults()
{

}