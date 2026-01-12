#pragma once
#include "env.h"

struct Command {
    char name[MAX_COMMAND_LENGTH] = "";
    int numOfPossibleParams = 0;
    int params[MAX_PARAM_OPTIONS];
    bool writableParam = false;
};

struct Commands {
    Command commands[MAX_COMMANDS];
    int numOfCommands;
};

struct Instruction {
    int commandId = -1;
    int commandParam = -1;
};

enum class cardSuit {
    clubs,
    diamonds,
    hearts,
    spades
};

struct card {
    int number;
    cardSuit suit;
    int id;
};

struct gameInfo {
    card deck[MAX_CARDS_IN_DECK];
    int cardsLeft = 0;
    card player1Hand[MAX_CARDS_IN_HAND];
    int player1CardsLeft = 0;
    card player2Hand[MAX_CARDS_IN_HAND];
    int player2CardsLeft = 0;
    cardSuit trump;
    card playedCard;
    int player1Pts = 0;
    int player2Pts = 0;
    int player1WonGamePts = 0;
    int player2WonGamePts = 0;
    int playerTurn = 1;
    bool gameIsClosed = false;
};

//errors
enum class ERROR {
    error_0, // nullptr error
    error_1, // input error
    error_2, //invalid argument error
};
