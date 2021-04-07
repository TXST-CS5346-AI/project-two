#include "Game.hpp"


Game::Game() // constructor
{


}
Game::~Game()  // destructor
{



}

// The only initialization function needed, as the game will 
// be played automatically by 2 AI players (MIN and MAX). 
// while gameOver == NOT_DONE keep playing

void Game::startGame()
{



}

// simple function to invert the enum value, thus determine who's turn is it next. 
// E.g., if currentPlayer is RED (1), function returns BLACK (-1)

Color Game::changePlayer(Color currentPlayer)
{
  

    return Color::RED;

}


Game::GameOver Game::gameOver()
{

 


    return GameOver::DRAW;

}

