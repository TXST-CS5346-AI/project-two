#include "Game.hpp"

#include <iostream>

Game::Game()
{
}

Game::~Game()
{
}

Game::Game(bool player1MinMax, int evalVersionP1, bool player2MinMax, int evalVersionP2, int depth)
{
    state = Board();
    state.InitializeMoveTable();
    redPlayer = Player(player1MinMax, Color::RED, depth, evalVersionP1);
    blackPlayer = Player(player2MinMax, Color::BLACK, depth, evalVersionP2);
}

Game::GameOver Game::startGame()
{
    int piecesTaken;

    while (true)
    {
        std::cout << "Round " << blackPlayer.getNumTurns() + 1  << " Black's Move..." << std::endl;

        piecesTaken = blackPlayer.takeTurn(state);
        blackPlayer.increaseNumPiecesTaken(piecesTaken);
        redPlayer.decreaseNumPieces(piecesTaken);

        if (doesBlackWin())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::BLACK_WINS;
        }
            

        std::cout << "Round " << redPlayer.getNumTurns() + 1 << " Red's Move..." << std::endl;

        piecesTaken = redPlayer.takeTurn(state);
        redPlayer.increaseNumPiecesTaken(piecesTaken);
        blackPlayer.decreaseNumPieces(piecesTaken);

        if (doesRedWin())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::RED_WINS;
        }

        if (isItADraw())
        {
            printNodes(redPlayer, "RED");
            printNodes(blackPlayer, "BLACK");
            return GameOver::DRAW;
        }
    }
}

void Game::printNodes(Player player, std::string colorText)
{
    std::cout << std::endl;
    if (player.isMinimax)
    {
        std::cout << colorText << " Leaf Nodes: " << player.minimaxLeafNodes << std::endl;
        std::cout << colorText << " Expanded Nodes: " << player.minimaxExpandedNodes << std::endl;
        std::cout << colorText << " Total Nodes: "<< player.getMinimaxTotalNodes() << std::endl;
    }
    else 
    {
        std::cout << colorText << " Leaf Nodes: " << player.absearchLeafNodes << std::endl;
        std::cout << colorText << " Expanded Nodes: " << player.absearchExpandedNodes << std::endl;
        std::cout << colorText << " Total Nodes: "<< player.getAbSearchTotalNodes() << std::endl;
    }
    std::cout << std::endl;
}

Color Game::changePlayer(Color currentPlayer)
{
    if (currentPlayer == Color::BLACK)
        return Color::RED;
    else
        return Color::BLACK;
}

bool Game::doesBlackWin()
{
    std::vector<Board::Move> redMoves = state.moveGen(Color::RED);
    return (redMoves.size() == 0);
}

bool Game::doesRedWin()
{
    std::vector<Board::Move> blackMoves = state.moveGen(Color::BLACK);
    return (blackMoves.size() == 0);
}

bool Game::isItADraw()
{
    if (redPlayer.getNumTurns() >= MAX_ALLOWED_TURNS ||
        blackPlayer.getNumTurns() >= MAX_ALLOWED_TURNS)
        return true;
    return false;
}