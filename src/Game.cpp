#include "Game.hpp"


Game::Game()
{
}

Game::~Game()
{
}

Game::Game( bool player1MinMax, int evalVersionP1, bool player2MinMax, int evalVersionP2, int depth )
{
    state =  Board();
    redPlayer = Player( player1MinMax, Color::RED, depth );
    blackPlayer = Player( player2MinMax, Color::BLACK, depth );
}

Game::GameOver Game::startGame()
{
    while ( true )
    {
        if ( doesBlackWin() )
            return GameOver::BLACK_WINS;
        else blackPlayer.takeTurn();

        if ( doesRedWin() )
            return GameOver::RED_WINS;
        else redPlayer.takeTurn();

        if ( isItADraw() )
            return GameOver::DRAW;
    }

}

Color Game::changePlayer(Color currentPlayer)
{
if (currentPlayer == Color::BLACK)
    return Color::RED;
else
    return  Color::BLACK;
}

bool Game::doesBlackWin()
{
    if ( redPlayer.getNumPieces() == 0 || !redPlayer.getDidPlayerMove() )
        return true;
    return false;
}

bool Game::doesRedWin()
{
    if ( blackPlayer.getNumPieces() == 0 || !blackPlayer.getDidPlayerMove() )
        return true;
    return false;
}

bool Game::isItADraw()
{
    if ( redPlayer.getNumTurns() >= MAX_ALLOWED_TURNS ||
        blackPlayer.getNumTurns() >= MAX_ALLOWED_TURNS )
        return true;
    return false;
}