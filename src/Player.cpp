#include "Player.hpp"
#include "Algorithm.hpp"

Player::Player()
{

}
Player::~Player()
{

}

Player::Player( bool minMaxState, Color color, int depth, int evalVersion )
{
    this->color = color;
    numPieces = 12; // how many pieces does Player have left
    numPiecesTaken = 0; // Player's current score based on captured enemy pieces
    numTurnsTaken = 0; // counter for Player's turns taken
    isMinimax = minMaxState;
    this->depth = depth;
    this->evalVersion = evalVersion; 
}

int Player::takeTurn(Board &state)
{
    Algorithm::Result result; 
    Algorithm* algorithm = new Algorithm(evalVersion, depth, *this);
	
	if (isMinimax)
	{
		result = algorithm->minimax_a_b( state, this->depth, this->color, INT32_MAX, INT32_MIN );
	} else {
		result = algorithm->alphaBetaSearch(state);
	}

	if (result.bestMove.destinationSquare.size() == 0)
	{
		didPlayerMove = false; // Player did not make a turn
	} else {
        state = state.updateBoard(result.bestMove, this->color); 
        numTurnsTaken++; // incremente Player's own turn counter 
        didPlayerMove = true; // return true as player did make a turn 
        state.printBoard(); 
    }

    // return how many pieces the player took during their turn
    return result.bestMove.removalSquare.size(); 
}

int Player::getNumPieces()
{
    return numPieces;
}

int Player::getNumPiecesTaken()
{
    return numPiecesTaken;
}

int Player::getNumTurns()
{
    return numTurnsTaken;
}

bool Player::getDidPlayerMove()
{
    return didPlayerMove;
}

Color Player::getColor()
{
    return color;
}

void Player::decreaseNumPieces(int numPiecesToDecreaseCount)
{
    numPieces -= numPiecesToDecreaseCount;
}

void Player::increaseNumPiecesTaken(int numPiecesToIncreaseScore)
{
    numPiecesTaken += numPiecesToIncreaseScore;
}
