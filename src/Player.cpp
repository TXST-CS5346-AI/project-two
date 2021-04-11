#include "../Include/Player.hpp"

Player::Player()
{

}
Player::~Player()
{

}

Player::Player(bool minMaxState, Color color)
{
    this->color = color;
    numPieces = 12; // how many pieces does Player have left
    numPiecesTaken = 0; // Player's current score based on captured enemy pieces
    numTurnsTaken = 0; // counter for Player's turns taken
    isMinimax = minMaxState;
}

void Player::takeTurn()
{


}

int Player::getNumPieces()
{
    return numPieces;
}
int Player::getNumPiecesTaken()
{
    return numPiecesTaken;
}

Color Player::getColor()
{
    return color;
}
// player can not gain additional pieces during the game, only lose them
void Player::decreasePlayerPieces(int piecesToSubtract)
{
    numPieces -= piecesToSubtract;
    numPiecesTaken += piecesToSubtract;
}