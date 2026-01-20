#pragma once
#include "env.h"

struct Command {
    char name[MAX_COMMAND_LENGTH] = "";
    int numOfPossibleParams = 0;
    int params[MAX_PARAM_OPTIONS];
    bool writableParam = false;
    bool writableIsString = false;
};

struct Commands {
    Command commands[MAX_COMMANDS];
    int numOfCommands;
};

struct Instruction {
    int commandId = -1;
    int commandParam = -1;
    char strParam[MAX_PARAM_LENGTH];
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
    int pts;
};

struct trickHistory {
    card cardPlayedP1;
    card cardPlayedP2;
    card cardDrawnP1;
    card cardDrawnP2;
    int playerWhoWon;
};

struct gamesHistory {
    int player1Points = 0;
    int player2Points = 0;
    int playerWhoWon = 0;
    int wonPoints = 0;
};

struct settings {
    int neededPointsToWin = 11;
    int marriagePoints = 20;
    int trumpMarriagePoints = 40;
    bool showPoints = true;
    bool lastTrickWins = false;
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
    int playerWhoCloedTheGame = 0;
    trickHistory lastTrick;
    settings setting;
    gamesHistory gameHistory[MAX_GAMES];
    int playedGames = 0;
};

//errors
enum class ERROR {
    error_0, // nullptr error
    error_1, // input error
    error_2, //invalid argument error
};
