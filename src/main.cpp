#include <iostream> 
#include <string.h>  // used by strcmp method

#include "Simulation.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "Pieces.hpp"
#include "Algorithm.hpp"

/**
* Main function, which serves as an entry point to the Checkers application. 
* @author Borislav Sabotinov
* 
* User may invoke a help menu by passing in either -h or -help as a CLI parameter when launching the program. 
* 
* @param int argc - count of the number of CLI arguments provided 
* @param char* argv[] - char array of the CLI arguments
* 
* @return EXIT_SUCCESS if the program completes successfully 
*/ 
int main(int argc, char* argv[])
{
	std::cout << "Welcome to the Checkers AI Program." << std::endl;
    std::cout << "Authors: \033[0;31m David Torrente (dat54@txstate.edu), Randall Henderson (rrh93@txstate.edu), Borislav Sabotinov (bss64@txstate.edu).\033[0m" << std::endl;
    std::cout << "Re-run this program with -h or -help CLI argument to see a help menu or refer to README for instructions." <<std::endl;
    std::cout << std::endl;

    // display help menu
    if (argc == 2 && (strcmp(argv[1], "-h") || strcmp(argv[1], "-help"))) 
    {
        std::cout << "To use this program, please read the instructions below and re-launch." << std::endl;
        std::cout << "Additional details for building and execution are also available in the README.md file." << std::endl;
        std::cout << std::endl;

        std::cout << "When executing the program, you will be prompted to enter the algorithm and evaluation function for the simulation." << std::endl;
        std::cout << "Please follow the instructions on the screen - if you do not care for any specific custom configuration, simply select Run All." << std::endl;
        std::cout << "This will bypass everything and simply run the simulation for all algorithms and all evaluation functions sequentially." 
            << "\nThis is the most common and preferred option." << std::endl;

        return EXIT_SUCCESS;
    }

    char userInputRunAll; 
    std::cout << "NOTE: If Y is selected below, you will NOT be prompted further for any eval function or alg. All will be simulated in order." << std::endl;
    std::cout << "Run a full simulation (recommended)? \033[0;30;46m(Y/n)\033[0m: ";
    std::cin >> userInputRunAll;

    Simulation* simulation = new Simulation(); 

    if (userInputRunAll == 'Y' || userInputRunAll == 'y') 
    {
        simulation->runFullSimulation();
    } 
    else 
    {
        char algSelected, evalFunctSelected; 
        std::cout << "Please select the type of simulation you wish to run by entering in it's number." << std::endl;
        std::cout << "1. Run Minimax-A-B algorithm" << std::endl;
        std::cout << "2. Run AB-Prune algorithm" << std::endl;
        std::cout << "Your choice (1 or 2): " << std::endl;
        std::cin >> algSelected; 
        std::cout << std::endl;
        std::cout << "Now select the evaluation by entering 1, 2, or 3 (total 3 choices available): " << std::endl;
        std::cin >> evalFunctSelected;
    }

    
	
	return EXIT_SUCCESS;
}