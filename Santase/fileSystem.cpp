#include<iostream>
#include<fstream>

#include "structs.h"
#include "fileSystem.h"
#include "stringUtils.h"

void saveSuit(std::ofstream& out, cardSuit suit) {
    switch (suit) {
    case cardSuit::clubs:    out << "clubs "; break;
    case cardSuit::diamonds: out << "diamonds "; break;
    case cardSuit::hearts:   out << "hearts "; break;
    case cardSuit::spades:   out << "spades "; break;
    }
}

void loadSuit(std::ifstream& in, cardSuit& suit) {
    std::string s;
    in >> s;

    if (s == "clubs")       suit = cardSuit::clubs;
    else if (s == "diamonds") suit = cardSuit::diamonds;
    else if (s == "hearts")   suit = cardSuit::hearts;
    else if (s == "spades")   suit = cardSuit::spades;
}

void saveCard(std::ofstream& out, const card& c) {
    out << c.number << ' ';
    saveSuit(out, c.suit);
    out << c.pts << ' ';
}

void loadCard(std::ifstream& in, card& c) {
    in >> c.number;
    loadSuit(in, c.suit);
    in >> c.pts;
}

void saveHistory(std::ofstream& out,const gamesHistory* h, int playedGames) {
    out << playedGames << '\n';
    for (int i = 0; i < playedGames; i++) {
        out << h[i].player1Points << ' ';
        out << h[i].player2Points << ' ';
        out << h[i].playerWhoWon << ' ';
        out << h[i].wonPoints << '\n';
    }
}

void loadHistory(std::ifstream& in, gamesHistory* h, int& playedGames) {
    in >> playedGames;
    for (int i = 0; i < playedGames; i++) {
        in >> h[i].player1Points;
        in >> h[i].player2Points;
        in >> h[i].playerWhoWon;
        in >> h[i].wonPoints;
    }
}

void saveTrickHistory(std::ofstream& out, const trickHistory& h) {
    saveCard(out, h.cardPlayedP1);
    saveCard(out, h.cardPlayedP2);
    saveCard(out, h.cardDrawnP1);
    saveCard(out, h.cardDrawnP2);
    out << h.playerWhoWon << '\n';
}

void loadTrickHistory(std::ifstream& in, trickHistory& h) {
    loadCard(in, h.cardPlayedP1);
    loadCard(in, h.cardPlayedP2);
    loadCard(in, h.cardDrawnP1);
    loadCard(in, h.cardDrawnP2);
    in >> h.playerWhoWon;
}

void saveSettings(std::ofstream& out, const settings& s) {
    out << s.neededPointsToWin << ' '
        << s.marriagePoints << ' '
        << s.trumpMarriagePoints << ' '
        << s.showPoints << ' '
        << s.lastTrickWins << '\n';
}

void loadSettings(std::ifstream& in, settings& s) {
    in >> s.neededPointsToWin
        >> s.marriagePoints
        >> s.trumpMarriagePoints
        >> s.showPoints
        >> s.lastTrickWins;
}

bool saveGame(const gameInfo& game, const char* filename) {
    char file[MAX_PARAM_LENGTH + 4];
    strCopy(file, filename);
    for (int i = 0; ; i++) {
        if (file[i] == '\0') {
            file[i] = '.';
            file[i + 1] = 't';
            file[i + 2] = 'x';
            file[i + 3] = 't';
            file[i + 4] = '\0';
            break;
        }
    }
    std::ofstream out(file);
    if (!out) return false;

    out << game.cardsLeft << '\n';
    for (int i = 0; i < game.cardsLeft; i++)
        saveCard(out, game.deck[i]);
    out << '\n';

    out << game.player1CardsLeft << '\n';
    for (int i = 0; i < game.player1CardsLeft; i++)
        saveCard(out, game.player1Hand[i]);
    out << '\n';

    out << game.player2CardsLeft << '\n';
    for (int i = 0; i < game.player2CardsLeft; i++)
        saveCard(out, game.player2Hand[i]);
    out << '\n';

    saveSuit(out, game.trump);
    saveCard(out, game.playedCard);

    out << game.player1Pts << ' '
        << game.player2Pts << ' '
        << game.player1WonGamePts << ' '
        << game.player2WonGamePts << ' '
        << game.playerTurn << ' '
        << game.gameIsClosed << ' '
        << game.playerWhoCloedTheGame << '\n';

    saveTrickHistory(out, game.lastTrick);
    saveHistory(out, game.gameHistory, game.playedGames);
    saveSettings(out, game.setting);
    return true;
}

bool loadGame(gameInfo& game, const char* filename) {
    char file[MAX_PARAM_LENGTH + 4];
    strCopy(file, filename);
    for (int i = 0; ; i++) {
        if (file[i] == '\0') {
            file[i] = '.';
            file[i + 1] = 't';
            file[i + 2] = 'x';
            file[i + 3] = 't';
            file[i + 4] = '\0';
            break;
        }
    }
    std::ifstream in(file);
    if (!in) return false;

    in >> game.cardsLeft;
    for (int i = 0; i < game.cardsLeft; i++)
        loadCard(in, game.deck[i]);

    in >> game.player1CardsLeft;
    for (int i = 0; i < game.player1CardsLeft; i++)
        loadCard(in, game.player1Hand[i]);

    in >> game.player2CardsLeft;
    for (int i = 0; i < game.player2CardsLeft; i++)
        loadCard(in, game.player2Hand[i]);

    loadSuit(in, game.trump);
    loadCard(in, game.playedCard);

    in >> game.player1Pts
        >> game.player2Pts
        >> game.player1WonGamePts
        >> game.player2WonGamePts
        >> game.playerTurn
        >> game.gameIsClosed
        >> game.playerWhoCloedTheGame;

    loadTrickHistory(in, game.lastTrick);
    loadHistory(in, game.gameHistory, game.playedGames);
    loadSettings(in, game.setting);

    return true;
}

