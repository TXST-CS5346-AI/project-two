#include "Simulation.hpp"
#include "Game.hpp"

#include <iostream>
#include <stdexcept>

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

/**
* Runs all games runs as delineated in Simulation.hpp
* 
* p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 1
* p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 2
* p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 3
* p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 1
* p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 2
* p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 3
* p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 1
* p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 2
* p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 3
* p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 1
* p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 2
* p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 3
* p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 1
* p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 2
* p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 3
* p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 1
* p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 2
* p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 3
* p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 1
* p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 2
* p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 3
* p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 1
* p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 2
* p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 3
* p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 1
* p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 2
* p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 3
* 
*/
void Simulation::runFullSimulation()
{
    std::cout << "\033[0;32mRunning a FULL simulation!\033[0m" << std::endl;

    for (int depth = 2; depth <= 4; depth += 2)
    {
        std::cout << "Depth: " << depth << std::endl;

        for (int p1_alg = 0; p1_alg < 2; p1_alg++)
        {
            for (int p2_alg = 0; p2_alg < 2; p2_alg++)
            {
                for (int p1_eval = 1; p1_eval < 4; p1_eval++)
                {
                    for (int p2_eval = 1; p2_eval < 4; p2_eval++)
                    {
                        // omit duplicates to save time - we only care about unique runs
                        // player 1 need not use alg 2. It's already covered by Player 2
                        if (p1_alg == 2 && p2_alg == 1)
                            continue;

                        std::cout << "p1_alg: " << p1_alg << " p1_eval: " << p1_eval << " p2_alg: "
                                  << p2_alg << " p2_eval: " << p2_eval << std::endl;

                        Game *game = new Game(p1_alg, p1_eval, p2_alg, p2_eval, 4);
                        Game::GameOver endGameStatus = game->startGame();

                        // TODO: print game status, num nodes generated, time it took, etc. 

                        delete game; 

                    } // p2_eval
                }     // p1_eval
            }         // p2_alg
        }             // p1_alg
    }                 // depth
}

/**
* @param int algorithm - If 1, minimax; if 2, AB Prune
* @param int evalFunction - 1,2, or 3
*/
void Simulation::runSpecificSimulation(int playerOneAlg, int playerOneEvalFunct, int playerTwoAlg, int playerTwoEvalFunct, int depth)
{
    std::cout << "\033[0;32mRunning a SINGLE game, specific simulation!\033[0m" << std::endl;

    // Validate algorithm selections
    if ((playerOneAlg < 0 || playerOneAlg > 3) && (playerTwoAlg < 0 || playerTwoAlg > 3))
        throw std::runtime_error("Error: algorithm may only be 1 (minimax-a-b) or 2 (ab-prune)!");

    // Validate evaluation function selections
    if ((playerOneEvalFunct < 0 || playerOneEvalFunct > 3) && (playerTwoEvalFunct < 0 || playerTwoEvalFunct > 3))
        throw std::runtime_error("Error: evalFunction may only be 1, 2, or 3!");
    
    // Validate depth
    if (depth <= 0 || depth > 10)
        throw std::runtime_error("Error: depth must be > 0 and <= 10. ");

    Game *game = new Game(playerOneAlg, playerOneEvalFunct, playerTwoAlg, playerTwoEvalFunct, depth);
    Game::GameOver endGameStatus = game->startGame();

    if (endGameStatus == Game::GameOver::BLACK_WINS)
        std::cout << "BLACK WINS!!!" << std::endl;
    else if (endGameStatus == Game::GameOver::RED_WINS)
        std::cout << "RED WINS!!!" << std::endl;
    else if (endGameStatus == Game::GameOver::DRAW)
        std::cout << "DRAW!!!" << std::endl;
    else 
        std::cout << "Oops, something went wrong!" << std::endl;

    // TODO: print game status, num nodes generated, time it took, etc. 

    delete game; 
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