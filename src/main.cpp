#include <iostream>
#include <string.h> // used by strcmp method

#include "Simulation.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "Pieces.hpp"
#include "Algorithm.hpp"

/**
 * Main entry way into the application via main() method. 
 * @author Borislav Sabotinov
 * 
 * The user can display a help menu.
 * The user will be prompted to select how they wish to interact with the program. 
 * Available options are: 
 *    1. a full simulation, 
 *    2. partial (single game) simulation, 
 *    3. player vs. player, or 
 *    4. player vs. AI. 
 */



// helper functions to make main() more readable and conscise
void printWelcomeMsg();
void printHelpMenu();
void printMainMenuOptions();
void executeRunBasedOnUserInput(int userInput, bool &isInputValid);
void getCustomSimUserInput(int &computerPlayerAlg, int &computerPlayerEval, int &depth);
void getCustomSimUserInput(int &playerOneAlg, int &playerOneEvalFunct, int &playerTwoAlg, int &playerTwoEvalFunct, int &depth);
void runManualGame();
void goodbye();

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
int main(int argc, char *argv[])
{
    printWelcomeMsg();

    // display help menu
    if (argc == 2)
    {
        std::string cliArg = argv[1]; 

        if (cliArg == "-h" || cliArg == "-help")
        {
            printHelpMenu();
            return EXIT_SUCCESS;
        }
        else if (cliArg == "-nc")  // disable color
        {
            // ANSII codes for colored text, to improve UI and readability
            Pieces::ANSII_BLUE_START     = "";
            Pieces::ANSII_RED_START      = "";
            Pieces::ANSII_RED_HIGH       = "";
            Pieces::ANSII_END            = "";
            Pieces::ANSII_GREEN_START    = "";
            Pieces::ANSII_BLUE_COUT      = "";
            Pieces::ANSII_RED_COUT       = "";
            Pieces::ANSII_GREEN_COUT     = "";
            Pieces::ANSII_YELLOW_COUT    = "";
        }
    }

    printMainMenuOptions();

    bool isInputValid = false;
    while (!isInputValid)
    {
        int userInput;
        std::cout << "Your choice " << Pieces::ANSII_BLUE_START << "(1, 2, 3, or 4)" << Pieces::ANSII_END << ": ";
        std::cin >> userInput;

        executeRunBasedOnUserInput(userInput, isInputValid);
    }

    goodbye(); 

    return EXIT_SUCCESS;
}

/**
 * Prints a welcome message to the console, along with the authors' names and emails. 
 */
void printWelcomeMsg()
{
    std::cout << Pieces::ANSII_GREEN_START << "Welcome to the Checkers AI Program." << Pieces::ANSII_END << std::endl;
    std::cout << "Authors: " << Pieces::ANSII_RED_START << " David Torrente (dat54@txstate.edu), Randall Henderson (rrh93@txstate.edu), "
              << "Borislav Sabotinov (bss64@txstate.edu)." << Pieces::ANSII_END << std::endl;
    std::cout << "Re-run this program with -h or -help CLI argument to see a help menu or refer to README for instructions."
              << std::endl;
    std::cout << std::endl;
}

/**
 * Prints a help message to the console if -h or -help are provided as CLI arguments when invoking the program
 */
void printHelpMenu()
{
    std::cout << "To use this program, please read the instructions below and re-launch." << std::endl;
    std::cout << "Additional details for building and execution are also available in the README.md file." << std::endl;
    std::cout << std::endl;

    std::cout << "When executing the program, you will be prompted to enter the algorithm and evaluation "
              << "function for the simulation." << std::endl;
    std::cout << "Please follow the instructions on the screen - if you do not care for any specific custom "
              << "configuration, simply select Run All." << std::endl;
    std::cout << "This will bypass everything and simply run the simulation for all algorithms and all "
              << "evaluation functions sequentially."
              << "\nThis is the most common and preferred option." << std::endl;
}

/**
 * Prints the main menu with option codes. 
 */
void printMainMenuOptions()
{
    std::cout << "NOTE: If 1 is selected below, you will NOT be prompted further for any eval function or algorithm. "
              << "All will be simulated in order." << std::endl;
    std::cout << std::endl;

    std::cout << "Choose a game mode below: " << std::endl;
    std::cout << "     1. Full Simulation (recommended)" << std::endl;
    std::cout << "     2. Single Custom Simulation" << std::endl;
    std::cout << "     3. Player vs Player (manual game)" << std::endl;
    std::cout << "     4. Player vs AI (will be asked to select AI playstyle)" << std::endl;
    std::cout << "     Ctrl + C to terminate program at any time." << std::endl;
    std::cout << std::endl;
}

/**
 * Given the user's choice in the main menu, execute the program accordingly.
 */
void executeRunBasedOnUserInput(int userInput, bool &isInputValid)
{
    Simulation *simulation = new Simulation();
    switch (userInput)
    {
    case 1: // full sim
        isInputValid = true; 
        simulation->runFullSimulation();
        break;
    case 2: // one custom sim
        isInputValid = true; 
        int playerOneAlg, playerOneEvalFunct, playerTwoAlg, playerTwoEvalFunct, depth;
        getCustomSimUserInput(playerOneAlg, playerOneEvalFunct, playerTwoAlg, playerTwoEvalFunct, depth);
        simulation->runSpecificSimulation(playerOneAlg, playerOneEvalFunct, playerTwoAlg, playerTwoEvalFunct, depth);
        break;
    case 3: // player vs. player
        isInputValid = true; 
        runManualGame();
        break;
    case 4: // player vs. ai
        isInputValid = true; 
        getCustomSimUserInput(playerOneAlg, playerOneEvalFunct, depth); 
        simulation->runPlayerVsAISimulation(playerOneAlg, playerOneEvalFunct, depth); 
        break;
    default:
        std::cerr << "Invalid option selected! Valid choices are 1, 2, 3, and 4" << std::endl;
    }
}

void getCustomSimUserInput(int &computerPlayerAlg, int &computerPlayerEval, int &depth)
{
    std::cout << "Please select the type of simulation you wish to run by entering in it's number." << std::endl;
    std::cout << "1. Run Minimax-A-B algorithm" << std::endl;
    std::cout << "0. Run Alpha-Beta-Search algorithm" << std::endl;

    // PLAYER CHOICES
    std::cout << "Algorithm for RED - Player 1 " << Pieces::ANSII_BLUE_START << "(1 for minimax, 0 for ab-Search)" << Pieces::ANSII_END << ":";
    std::cin >> computerPlayerAlg;
    std::cout << std::endl;
    std::cout << "Evaluation for RED - Player 1 " << Pieces::ANSII_BLUE_START << "(1 (David's), 2 (Randy's), 3 (Boris'), 4 (returns 1st available move))" << Pieces::ANSII_END << ": ";
    std::cin >> computerPlayerEval;

    // DEPTH
    std::cout << "Enter the depth for the search tree " << Pieces::ANSII_BLUE_START << "(2 or 4 recommended; min = 1, max = 15)" << Pieces::ANSII_END << ": ";
    std::cin >> depth; 
}

/**
 * getCustomSimUserInput is a helper function to obtain the algorithm and eval function for Player 1 and Player 2
 * One game will be simulated only using this input. 
 */
void getCustomSimUserInput(int &playerOneAlg, int &playerOneEvalFunct, int &playerTwoAlg, int &playerTwoEvalFunct, int &depth)
{
    std::cout << "Please select the type of simulation you wish to run by entering in it's number." << std::endl;
    std::cout << "1. Run Minimax-A-B algorithm" << std::endl;
    std::cout << "0. Run Alpha-Beta-Search algorithm" << std::endl;

    // PLAYER ONE CHOICES
    std::cout << "Algorithm for RED - Player 1 " << Pieces::ANSII_BLUE_START << "(1 for minimax, 0 for ab-Search)" << Pieces::ANSII_END << ":";
    std::cin >> playerOneAlg;
    std::cout << std::endl;
    std::cout << "Evaluation for RED - Player 1 " << Pieces::ANSII_BLUE_START << "(1 (David's), 2 (Randy's), 3 (Boris'), 4 (returns 1st available move))" << Pieces::ANSII_END << ": ";
    std::cin >> playerOneEvalFunct;

    // PLAYER TWO CHOICES
    std::cout << "Algorithm for BLACK - Player 2 " << Pieces::ANSII_BLUE_START << "(1 for minimax, 0 for ab-Search)" << Pieces::ANSII_END << ": ";
    std::cin >> playerTwoAlg;
    std::cout << std::endl;
    std::cout << "Evaluation for BLACK - Player 2 " << Pieces::ANSII_BLUE_START << "(1 (David's), 2 (Randy's), 3 (Boris'), 4 (returns 1st available move))" << Pieces::ANSII_END << ": ";
    std::cin >> playerTwoEvalFunct;

    // DEPTH
    std::cout << "Enter the depth for the search tree " << Pieces::ANSII_BLUE_START << "(2 or 4 recommended; min = 1, max = 15)" << Pieces::ANSII_END << ": ";
    std::cin >> depth; 
}

/**
 * runManualGame function provides a human user the ability to play checkers with another human. 
 * It is a manual, input based game where a player must enter the number of the turn they wish to execute.
 * @author David Torrente
 * @author Borislav Sabotinov 
 */
void runManualGame()
{
    bool gameOver = false;
    int moveSelection;
    Color currentPlayer = Color::RED;
    std::vector<Board::Move> redMoves;
    std::vector<Board::Move> blackMoves;
    Board board;
    board.InitializeMoveTable();

    while (!gameOver)
    {
        board.printBoard();
        redMoves = board.moveGen(Color::RED);
        blackMoves = board.moveGen(Color::BLACK);

        if (blackMoves.size() == 0)
        {
            gameOver = true; 
            std::cout << "\nRED WINS!!!" << std::endl; 
            std::cout << "RED Player: ᕙ(-_-')ᕗ" << std::endl;
            std::cout << "But most importantly, BLACK looooses (boooo!)" << std::endl;
            std::cout << "BLACK Player: (╯°□°）╯︵ ┻━┻" << std::endl; 

            break; 
        } 
        else if (redMoves.size() == 0)
        {
            gameOver = true; 
            std::cout << "\nBLACK WINS!!!" << std::endl; 
            std::cout << "BLACK Player: ᕙ(-_-')ᕗ" << std::endl;
            std::cout << "But most importantly, RED looooses (boooo!)" << std::endl;
            std::cout << "RED Player: (╯°□°）╯ ︵ ┻━┻" << std::endl; 
            break; 
        }

        std::cout << "Red(X) moves: ";
        for (int redMoveIter = 0; redMoveIter < redMoves.size(); redMoveIter++)
        {
            std::cout << "<" << redMoveIter + 1 << "> " << redMoves.at(redMoveIter).startSquare;
            for (int destinationIter = 0; destinationIter < redMoves.at(redMoveIter).destinationSquare.size(); destinationIter++)
            {
                std::cout << " to " << redMoves.at(redMoveIter).destinationSquare.at(destinationIter);
            }
            std::cout << ", ";
        }
        std::cout << std::endl;

        std::cout << "Black(O) moves: ";
        for (int blackMoveIter = 0; blackMoveIter < blackMoves.size(); blackMoveIter++)
        {
            std::cout << "<" << blackMoveIter + 1 << "> " << blackMoves.at(blackMoveIter).startSquare;
            for (int destinationIter = 0; destinationIter < blackMoves.at(blackMoveIter).destinationSquare.size(); destinationIter++)
            {
                std::cout << " to " << blackMoves.at(blackMoveIter).destinationSquare.at(destinationIter);
            }
            std::cout << ", ";
        }
        std::cout << std::endl;

        bool isSelectionValid = false; 
        if (currentPlayer == Color::RED)
        {
            while (!isSelectionValid)
            {
                std::cout << "Select RED move: ";
                std::cin >> moveSelection;
                if (moveSelection > redMoves.size() || moveSelection < 0) 
                {
                    std::cerr << "Out of range; please enter a valid choice!" << std::endl;
                } else {
                    board = board.updateBoard(redMoves.at(moveSelection - 1), Color::RED);
                    currentPlayer = Color::BLACK;
                    isSelectionValid = true; 
                }
            }
        }
        else
        {
            isSelectionValid = false; 
            while (!isSelectionValid)
            {
                std::cout << "Select BLACK move: ";
                std::cin >> moveSelection;
                if (moveSelection > blackMoves.size() || moveSelection < 0) 
                {
                    std::cerr << "Out of range; please enter a valid choice!" << std::endl;
                } else {
                    board = board.updateBoard(blackMoves.at(moveSelection - 1), Color::BLACK);
                    currentPlayer = Color::RED;
                    isSelectionValid = true; 
                }
            }
        }
    }
}

void goodbye()
{
    std::cout << std::endl; 
    std::cout << Pieces::ANSII_BLUE_START << " __  __  __  __  __     __  " << Pieces::ANSII_END << std::endl;
    std::cout << Pieces::ANSII_BLUE_START << "/ _ /  \\/  \\|  \\|__)\\_/|_ | " << Pieces::ANSII_END << std::endl;
    std::cout << Pieces::ANSII_BLUE_START << "\\__)\\__/\\__/|__/|__) | |__. " << Pieces::ANSII_END << std::endl;
    std::cout << Pieces::ANSII_BLUE_START << "                            " << Pieces::ANSII_END << std::endl;
    std::cout << std::endl; 
}