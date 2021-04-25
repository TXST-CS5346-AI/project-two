#include "Simulation.hpp"

#include <iostream>
#include <stdexcept>

/**
 * Simulation implementation
 * @author Borislav Sabotinov
 * 
 * Responsible for driving the simulation based on user preference provided in Main
 */

Simulation::Simulation()
{
    this->numGamesPlayed = 0;
}

Simulation::~Simulation()
{
}

/**
* Runs all games runs as delineated in Simulation.hpp
* R = redundant game simulated 
* 
* 1.  p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 1  R
* 2.  p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 2
* 3.  p1_alg: 1 p1_eval: 1 p2_alg: 1 p2_eval: 3
* 4.  p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 1  R
* 5.  p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 2  R
* 6.  p1_alg: 1 p1_eval: 2 p2_alg: 1 p2_eval: 3
* 7.  p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 1  R
* 8.  p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 2  R
* 9.  p1_alg: 1 p1_eval: 3 p2_alg: 1 p2_eval: 3
* 10. p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 1
* 11. p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 2
* 12. p1_alg: 1 p1_eval: 1 p2_alg: 2 p2_eval: 3
* 13. p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 1
* 14. p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 2
* 15. p1_alg: 1 p1_eval: 2 p2_alg: 2 p2_eval: 3
* 16. p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 1
* 17. p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 2
* 18. p1_alg: 1 p1_eval: 3 p2_alg: 2 p2_eval: 3
* 19. p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 1  R
* 20. p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 2  R
* 21. p1_alg: 2 p1_eval: 1 p2_alg: 2 p2_eval: 3
* 22. p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 1
* 23. p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 2
* 24. p1_alg: 2 p1_eval: 2 p2_alg: 2 p2_eval: 3
* 25. p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 1  R
* 26. p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 2  
* 27. p1_alg: 2 p1_eval: 3 p2_alg: 2 p2_eval: 3  R
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
                        numGamesPlayed++;
                        delete game;

                    } // p2_eval
                }     // p1_eval
            }         // p2_alg
        }             // p1_alg
    }                 // depth
}

/**
 * Allows the user to run a specific, custom simulation based on their preference
 * @param int redPlayerAlg - If 1, minimax; if 0, AB Prune
 * @param int redPlayerEvalFunct - 1,2, 3, or 4
 * @param int blackPlayerAlg - If 1, minimax; if 0, AB Prune
 * @param int blackPlayerEvalFunct - 1,2, 3, or 4
 * @param int depth - 2 to 15, preferrably 2 or 4 as per project requirements
 */
void Simulation::runSpecificSimulation(int redPlayerAlg, int redPlayerEvalFunct, int blackPlayerAlg, int blackPlayerEvalFunct, int depth)
{
    std::cout << Pieces::ANSII_GREEN_START << "Running a SINGLE game, specific simulation!" << Pieces::ANSII_END << std::endl;

    // Validate algorithm selections
    if ((redPlayerAlg < 0 || redPlayerAlg > 1) && (blackPlayerAlg < 0 || blackPlayerAlg > 1))
        throw std::runtime_error("Error: algorithm may only be 1 (minimax-a-b) or 0 (ab-prune)!");

    // Validate evaluation function selections
    if ((redPlayerEvalFunct <= 0 || redPlayerEvalFunct > 4) && (blackPlayerEvalFunct <= 0 || blackPlayerEvalFunct > 4))
        throw std::runtime_error("Error: evalFunction may only be 1, 2, 3, or 4!");

    // Validate depth
    if (depth <= 1 || depth > 15)
        throw std::runtime_error("Error: depth must be > 1 and <= 15. ");

    Game *game = new Game(redPlayerAlg, redPlayerEvalFunct, blackPlayerAlg, blackPlayerEvalFunct, depth);
    Game::GameOver endGameStatus = game->startGame();

    printGameResults(endGameStatus);
    printGameConfig(redPlayerAlg, redPlayerEvalFunct, blackPlayerAlg, blackPlayerEvalFunct, depth);

    delete game;
}

/**
 * Helper function to print overall Game results, depending on who won
 * @param Game::GameOver endGameStatus - the game will return to us who was the winner (if any)
 */
void Simulation::printGameResults(Game::GameOver endGameStatus)
{
    if (endGameStatus == Game::GameOver::BLACK_WINS)
        printBlackWins();
    else if (endGameStatus == Game::GameOver::RED_WINS)
        printRedWins();
    else if (endGameStatus == Game::GameOver::DRAW)
        printDraw();
    else
        std::cout << "Oops, something went wrong!" << std::endl;
}

/**
 * runPlayerVsAISimulation - play a game with a human against a computer player
 * This is a fun option, it allows a person to test themselves against the AI
 * @param int playerAlg - what alg will the AI use
 * @param int playerEvalFunct - what eval function will AI use
 * @param int depth - what depth will AI use
 */
void Simulation::runPlayerVsAISimulation(int playerAlg, int playerEvalFunct, int depth)
{

    Player computerPlayer = Player(playerAlg, Color::RED, depth, playerEvalFunct);
    bool gameOver = false;
    int moveSelection;
    Color computerPlayerColor = Color::RED;
    Color humanPlayerColor = Color::BLACK;
    Color currentPlayerColor = humanPlayerColor;
    Board board;
    board.printBoard();

    while (!gameOver)
    {
        if (currentPlayerColor == humanPlayerColor) // BLACK
        {
            std::vector<Board::Move> blackMoves = board.moveGen(humanPlayerColor);
            // PRINT OUT BLACK'S MOVES
            std::cout << "Black's moves (b/B): ";
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

            // GET HUMAN PLAYER MOVE
            bool isSelectionValid = false;
            while (!isSelectionValid)
            {
                std::cout << "Select BLACK (Human) move: ";
                std::cin >> moveSelection;
                if (moveSelection > blackMoves.size() || moveSelection < 0)
                {
                    std::cerr << "Out of range; please enter a valid choice!" << std::endl;
                }
                else
                {
                    board = board.updateBoard(blackMoves.at(moveSelection - 1), Color::BLACK);
                    currentPlayerColor = computerPlayerColor; // RED
                    isSelectionValid = true;
                }
            }
        }
        else if (currentPlayerColor == computerPlayerColor) // RED
        {
            // AI TAKES TURN AND PRINTS BOARD
            int numPiecesTakenByAI = computerPlayer.takeTurn(board);
            currentPlayerColor = humanPlayerColor; // BLACK
        }

        // CHECK WIN-LOSS CONDITIONS
        gameOver = didSomeoneWin(board); // if true, game will end
    }
    board.printBoard(); // print final board after someone wins
}

/**
 * At the end of the game, print the game configuration the user provided for ease of reference
 * @param int redPlayerAlg - If 1, minimax; if 0, AB Prune
 * @param int redPlayerEvalFunct - 1,2, 3, or 4
 * @param int blackPlayerAlg - If 1, minimax; if 0, AB Prune
 * @param int blackPlayerEvalFunct - 1,2, 3, or 4
 * @param int depth - 2 to 15, preferrably 2 or 4 as per project requirements
 */
void Simulation::printGameConfig(int redPlayerAlg, int redPlayerEvalFunct, int blackPlayerAlg, int blackPlayerEvalFunct, int depth)
{
    std::string algs[2] = {"Alpha-Beta-Search", "Minimax-Alpha-Beta"};
    std::cout << "Red player alg: " << algs[redPlayerAlg] << ", eval: " << redPlayerEvalFunct << std::endl;
    std::cout << "Black player alg: " << algs[blackPlayerAlg] << ", eval: " << blackPlayerEvalFunct << std::endl;
    std::cout << "Depth: " << depth << std::endl;
}

/**
 * didSomeoneWin - returns true if one player won, to break out of game loops
 * @param Board board
 * 
 * @return true if someone won, otherwise false
 */
bool Simulation::didSomeoneWin(Board board)
{
    bool isGameOver = false;
    std::vector<Board::Move> redMoves = board.moveGen(Color::RED);
    std::vector<Board::Move> blackMoves = board.moveGen(Color::BLACK);

    if (blackMoves.size() == 0)
    {
        isGameOver = true;
        printRedWins();
    }
    else if (redMoves.size() == 0)
    {
        isGameOver = true;
        printBlackWins();
    }

    return isGameOver;
}

/**
 * Helper function to display if Red wins
 */
void Simulation::printRedWins()
{
    std::cout << "\nRED WINS!!!" << std::endl;
    std::cout << "RED Player: ᕙ(-_-')ᕗ" << std::endl;
    std::cout << "But most importantly, BLACK looooses (boooo!)" << std::endl;
    std::cout << "BLACK Player: (╯°□°）╯︵ ┻━┻" << std::endl;
}

/**
 * Helper function to display if Black wins
 */
void Simulation::printBlackWins()
{
    std::cout << "\nBLACK WINS!!!" << std::endl;
    std::cout << "BLACK Player: ᕙ(-_-')ᕗ" << std::endl;
    std::cout << "But most importantly, RED looooses (boooo!)" << std::endl;
    std::cout << "RED Player: (╯°□°）╯ ︵ ┻━┻" << std::endl;
}

/**
 * Helper function to display if we run out of permitted turns and get a draw
 */
void Simulation::printDraw()
{
    std::cout << "DRAW!!!" << std::endl;
    std::cout << "Red - (ง •̀_•́)ง   ლ( `Д’ ლ) - Black" << std::endl;
    std::cout << "Mission FAILED...We'll get em next time!" << std::endl;
}

/** 
 * getNumGamesPlayed - returns a count of the number of games played in a simulation
 *
 */
int Simulation::getNumGamesPlayed()
{
    return numGamesPlayed;
}

/**
 * generateAnalsysisResults creates a table with results for analysis how many nodes were created, etc.
 */
void Simulation::generateAnalysisResults()
{
}