#include "../Include/Game.hpp"


Game::Game()
{
}

Game::~Game()
{
}

Game::Game( bool player1MinMax, int evalVersionP1, bool player2MinMax, int evalVersionP2, int depth )
{
    state =  Board();
    redPlayer = Player( player1MinMax, Color::RED );
    blackPlayer = Player( player2MinMax, Color::BLACK );
    useEvalP1 = evalVersionP1;
    useEvalP2 = evalVersionP2;
    this->depth = depth;
}

Game::GameOver Game::startGame()
{
    Color currentPlayer = Color::BLACK;
    GameOver gameStatus = GameOver::NOT_DONE;
    while ( gameStatus == GameOver::NOT_DONE )
    {


        currentPlayer = changePlayer( currentPlayer);
        gameStatus = gameOver();
    }
return  gameStatus;
}

Color Game::changePlayer(Color currentPlayer)
{
if (currentPlayer == Color::BLACK)
    return Color::RED;
else
    return  Color::BLACK;
}

Game::GameOver Game::gameOver()
{
    int UtilityValue = 0;
    if ( UtilityValue == 0 )
        return GameOver::BLACK_WINS;
    else if ( UtilityValue == 1 )
        return GameOver::DRAW;
    else
        return GameOver::RED_WINS;
}

