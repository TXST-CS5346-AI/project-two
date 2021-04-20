#ifndef GAME_H
#define GAME_H

#include "Player.hpp"
#include "Board.hpp"

/**
 * Header definition for class Game.
 *
 * The Game class represents a checkers Game.
 *
 * A game consists of a board, two players (red and black), and
 * 12 checkers pieces for each player.
 *
 * A game ends when either player loses all their pieces or is blocked and
 * has no further moves available.
 *
 * The startGame() function will trigger the process and no input from the user is required.
 * Red and Black players will be created. Each player will be given 12 checker pieces.
 * The pieces will be appropriately placed on the board.
 * Each player will execute their strategy.
 *

 */

class Game
{

private:
    Board state;
    Player redPlayer;
    Player blackPlayer;
    const int MAX_ALLOWED_TURNS = 6;

public:
    Game(); // constructor
    ~Game(); // destructor
    Game( bool, int, bool, int, int); // player1 algo, eval version, player2 algo, eval version, depth

    enum class GameOver {
        BLACK_WINS = 0,
        RED_WINS = 1,
        DRAW = 2,
        NOT_DONE = 3
    };

    // The only initialization function needed, as the game will
    // be played automatically by 2 AI players (MIN and MAX).
    // while gameOver == NOT_DONE keep playing
    GameOver startGame();

    // simple function to invert the enum value, thus determine who's turn is it next.
    // E.g., if currentPlayer is RED (1), function returns BLACK (-1)
    Color changePlayer(Color currentPlayer);

    GameOver gameOver();  // Have end game conditions been met?
    bool doesRedWin();
    bool doesBlackWin();
    bool isItADraw();
};

#endif // !GAME_H
