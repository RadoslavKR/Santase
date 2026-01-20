#include "screenRender.h"
#include "errorHandler.h"
#include "game.h"

#include "structs.h"

#include<iostream>
#include<windows.h>
#undef ERROR

void openRules(settings setting) {
    std::cout << "Santase(66) Rules" << std::endl;
    std ::cout << "The main objective of the game is to be the first one to score " << setting.neededPointsToWin << " win points. "
        "You win points by winning a round. The first player to reach 66 points in a round "
        "and to declare it is the winner of the hand.\n\n"

        "You acquire points by winning tricks.\n\n"

        "Another way to acquire points is by matching pairs of Kings and Queens, which grants "
        "you a bonus of " << setting.marriagePoints << " or " << setting.trumpMarriagePoints << " points once declared.\n\n"

        "During the deal, three cards are dealt to each player, one card is turned up as a trump, "
        "followed by dealing another three cards to each player.\n\n"

        "You are not required to follow suit until all cards of the talon are dealt.\n\n"

        "You are required to follow suit once the undealt cards are closed.\n\n"

        "A player may close the talon if he believes that the cards in his hand are strong enough "
        "to win the remaining tricks and reach 66 points. During this time the players must follow "
        "suit if possible, otherwise trump.\n\n"

        "The player who holds the lowest trump card (the 9) could exchange it for the trump that "
        "is turned up during his turn to lead.\n\n"

        "The exchange could occur as long as there are more than 2 cards remaining in the talon.\n\n"

        << (setting.lastTrickWins ? "The last trick wins you the round.\n\n" : "The last trick gives you +10pts.\n\n") <<

        "If a player believes that he has 66 points and declares it (only after winning a trick or "
        "after matching pairs of K and Q), the play stops immediately.\n\n"

        "If the player is right and has more than 66 points, he wins the hand. Otherwise, points "
        "are distributed to the opponent instead.\n\n"

        "The ranks and values of the cards are as follows:\n\n"
        "Ace - 11 points\n"
        "Ten - 10 points\n"
        "King - 4 points\n"
        "Queen - 3 points\n"
        "Jack - 2 points\n"
        "9 - 0 points\n\n"

        "Winning a round gives you:\n\n"
        "3 points if the opponent has 0 tricks won\n"
        "2 points if the opponent's score is below 33\n"
        "1 point if the opponent's score is 33 or more\n\n";
}

int changeSetting(bool trueOrFalseParam) {
    Command changeTo = { "change_to", trueOrFalseParam ? 2 : 0, {0, 1}, !trueOrFalseParam};
    Command cancel = { "cancel" };
    Commands commands = {
        {changeTo, cancel},
        2
    };
    renderPossibleCommands(&commands);
    Instruction instruction = userInput(&commands);

    if (instruction.commandId == 0) {
        return instruction.commandParam;
    }
    else {
        errorHandler(ERROR::error_1);
        return -1;
    }
}

void openSettings(settings &setting) {
    while (true) {
        std::cout << "Santase(66)" << std::endl;

        std::cout << "1) Target Points to win [" << setting.neededPointsToWin << "]" << std::endl;
        std::cout << "2) Marriage points (non-trump) [" << setting.marriagePoints << "]" << std::endl;
        std::cout << "3) Marriage points (trum) [" << setting.trumpMarriagePoints << "]" << std::endl;
        std::cout << "4) Show players' points [" << (setting.showPoints ? "on" : "off") << "]" << std::endl;
        std::cout << "5) Last trick wins or +10 pts [" << (setting.lastTrickWins ? "wins" : "+10pts") << "]" << std::endl;

        Command change = { "change", 5, {1, 2, 3, 4, 5} };
        Command back = { "back" };
        Commands commands = {
            {change, back},
            2
        };
        renderPossibleCommands(&commands);
        Instruction instruction = userInput(&commands);

        if (instruction.commandId == 0) {
            if (instruction.commandParam == 1) {
                std::cout << "Please write your desired target points to win" << std::endl;
                int tmp = changeSetting(false);
                if (tmp == -1) continue;
                else setting.neededPointsToWin = tmp;
            }
            else if (instruction.commandParam == 2) {
                std::cout << "Please write your desired non-trump marriage points" << std::endl;
                int tmp = changeSetting(false);
                if (tmp == -1) continue;
                else setting.marriagePoints = tmp;
            }
            else if (instruction.commandParam == 3) {
                std::cout << "Please write your desired trump marriage points" << std::endl;
                int tmp = changeSetting(false);
                if (tmp == -1) continue;
                else setting.trumpMarriagePoints = tmp;
            }
            else if (instruction.commandParam == 4) {
                std::cout << "Please write 1 for 'on' or 0 for 'off' for weather to show players points" << std::endl;
                int tmp = changeSetting(true);
                if (tmp == -1) continue;
                else setting.showPoints = tmp;
            }
            else {
                std::cout << "Please write 1 if you want the last trick to win the game or 0 if you want it to be +10pts." << std::endl;
                int tmp = changeSetting(true);
                if (tmp == -1) continue;
                else setting.lastTrickWins = tmp;
            }
        }
        else return;
    }
}

Instruction openMainMenu() {
    std::cout << "Welcome to Santase(66)" << std::endl;
    const Command start = { "start" };
    const Command rules = { "rules" };
    const Command settings = { "settings" };
    const Command quit = { "quit" };

    const Commands mainMenuCommands = {
        {start, rules, settings, quit},
        4
    };

    renderPossibleCommands(&mainMenuCommands);
    return userInput(&mainMenuCommands);
}

int main() {
    srand(time(nullptr));
    SetConsoleOutputCP(CP_UTF8);

    settings setting;
    while (true) {
        Instruction instruction = openMainMenu();
        if (instruction.commandId == 0) startGame(setting);
        else if (instruction.commandId == 1) openRules(setting);
        else if (instruction.commandId == 2) openSettings(setting);
        else return 0;
    }
}
