#include "Player.hpp"
#include "Algorithm.hpp"

#include <iostream>


duration<double, std::milli> Player::abs_time; 
duration<double, std::milli> Player::mini_time;

Player::Player()
{
}
Player::~Player()
{
}

Player::Player(bool minMaxState, Color color, int depth, int evalVersion)
{
    this->color = color;
    numPieces = 12;     // how many pieces does Player have left
    numPiecesTaken = 0; // Player's current score based on captured enemy pieces
    numTurnsTaken = 0;  // counter for Player's turns taken
    isMinimax = minMaxState;
    this->depth = depth;
    this->evalVersion = evalVersion;

    this->minimaxExpandedNodes = 0;
    this->minimaxLeafNodes = 0;
    this->absearchExpandedNodes = 0;
    this->absearchLeafNodes = 0;
}

int Player::takeTurn(Board &state)
{
    Algorithm::Result result;
    Algorithm *algorithm = new Algorithm(evalVersion, depth, *this);

    if (isMinimax)
    {
        auto t1 = high_resolution_clock::now();
        result = algorithm->minimax_a_b(state, this->depth, this->color, 9000000, -8000000);
        auto t2 = high_resolution_clock::now();

        Player::mini_time += (t2 - t1); 
        
        this->minimaxExpandedNodes += algorithm->minimaxExpandedNodes;
        this->minimaxLeafNodes += algorithm->minimaxLeafNodes;
    }
    else
    {
        auto t1 = high_resolution_clock::now();
        result = algorithm->alphaBetaSearch(state);
        auto t2 = high_resolution_clock::now();

        Player::abs_time += (t2 - t1); 

        this->absearchExpandedNodes += algorithm->absearchExpandedNodes;
        this->absearchLeafNodes += algorithm->absearchLeafNodes;
    }

    if (result.bestMove.destinationSquare.size() == 0)
    {
        didPlayerMove = false; // Player did not make a turn
    }
    else
    {
        state = state.updateBoard(result.bestMove, this->color);
        printMove(result.bestMove, this->color, true);
        numTurnsTaken++;      // incremente Player's own turn counter
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

void Player::printMove(Board::Move move, Color color, bool alwaysPrint)
{
    std::string colorStr;
    if (color == Color::BLACK)
        colorStr = "Black";
    else
        colorStr = "Red";

    int conditionalPrint;
    if (alwaysPrint)
        conditionalPrint = 1;
    else
        conditionalPrint = Pieces::ouputDebugData;

    if (conditionalPrint)
    {
        std::cout << colorStr << " moves from " << move.startSquare << " to destination (in sequence): ";
        for (int i = 0; i < move.destinationSquare.size(); i++)
            std::cout << "dest: " << move.destinationSquare.at(i) << std::endl;
    }
}
