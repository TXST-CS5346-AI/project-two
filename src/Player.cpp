#include "Player.hpp"

Player::Player() // constructor
{


}
Player::~Player() // destructor
{


}

Player::Player(Color color) // overloaded constructor to set player color, which is IMMUTABLE
{
    this->color = color;

}

Board Player::takeTurn()
{
    Board board;


    return board;

}

int Player::getNumPieces()
{

    return 0;
}
int Player::getNumPiecesTaken()
{

    return 0;
}
// player can not gain additional pieces during the game, only lose them
void Player::decreasePlayerPieces(int piecesToSubtract)
{



}


