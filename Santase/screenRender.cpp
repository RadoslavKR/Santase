/**
*
* Solution to course project # <2>
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author <Radoslav Kolev Rachkov>
* @idnumber <6MI0600634>
* @compiler <VC>
*
* <Handels the userInput and the commands>
*
*/

#include "screenRender.h"
#include "errorHandler.h"
#include "stringUtils.h"
#include "algorithmUtils.h"

#include<iostream>

#include "structs.h"
#include "env.h"


void renderPossibleCommands(const Commands* commands) {
    std::cout << std::endl;
    std::cout << "Please write the name of the command below, plus the desired param(the numbers in <>), if it requers it, to execute." << std::endl;
    if (commands == nullptr) {
        errorHandler(ERROR::error_0);
        return;
    }

    for (int i = 0; i < commands->numOfCommands; i++) {
        std::cout  << ">" << commands->commands[i].name;
        if (commands->commands[i].numOfPossibleParams != 0) {
            std::cout << " <" << commands->commands[i].params[0];
            for (int j = 1; j < commands->commands[i].numOfPossibleParams; j++) {
                std::cout << ", " << commands->commands[i].params[j];
            }
            std::cout << ">";
        }
        if (commands->commands[i].writableParam) std::cout << "<>";
        std::cout << std::endl;
    }
}

Instruction userInput(const Commands* possibleCommands) {
    if (possibleCommands == nullptr) {
        errorHandler(ERROR::error_0);
        return {};
    }
    while (true) {
        char input[MAX_USER_INPUT];
        std::cin.getline(input, MAX_USER_INPUT);

        split(input, ' ');

        Instruction instruction;
        for (int i = 0; i < possibleCommands->numOfCommands; i++) {
            if (strCompare(input, possibleCommands->commands[i].name)) {
                instruction.commandId = i;
                break;
            }
        }

        if (instruction.commandId != -1 && possibleCommands->commands[instruction.commandId].numOfPossibleParams != 0) {
            char* inputPtr = input;
            while (*inputPtr != '\0') {
                inputPtr++;
            }

            int param = convertStrToNum(inputPtr + 1);
            if (param == -1) continue;
            if (!contains(possibleCommands->commands[instruction.commandId].params, possibleCommands->commands[instruction.commandId].numOfPossibleParams, param)) {
                errorHandler(ERROR::error_1);
                continue;
            }
            instruction.commandParam = param;
        }
        else if (instruction.commandId != -1 && possibleCommands->commands[instruction.commandId].writableParam) {
            char* inputPtr = input;
            while (*inputPtr != '\0') inputPtr++;

            if (!possibleCommands->commands[instruction.commandId].writableIsString) {
                instruction.commandParam = convertStrToNum(inputPtr + 1);
                if (instruction.commandParam == -1) continue;
            }
            else {
                if (strCompare(inputPtr + 1, "\0")) {
                    errorHandler(ERROR::error_1);
                    continue;
                }
                strCopy(instruction.strParam, inputPtr + 1);
            }
        }

        if (instruction.commandId == -1) {
            errorHandler(ERROR::error_1);
        }
        else {
            std::cout << std::endl << std::endl;
            std::cout << "\033[2J\033[H";
            return instruction;
        }
    }
}

void renderCard(const card _card) {
    if (_card.number == 11) {
        std::cout << 'J';
    }
    else if (_card.number == 12) {
        std::cout << 'Q';
    }
    else if (_card.number == 13) {
        std::cout << 'K';
    }
    else if (_card.number == 14) {
        std::cout << 'A';
    }
    else std::cout << _card.number;

    if (_card.suit == cardSuit::spades) {
        std::cout << "\033[97m" << "\xE2\x99\xA0";
    }
    else if (_card.suit == cardSuit::hearts) {
        std::cout << "\033[31m" << "\xE2\x99\xA5";
    }
    else if (_card.suit == cardSuit::clubs) {
        std::cout << "\033[97m" << "\xE2\x99\xA3";
    }
    else if (_card.suit == cardSuit::diamonds) {
        std::cout << "\033[31m" << "\xE2\x99\xA6";
    }
    std::cout << "\033[37m";
}

void renderHand(const card* hand, int handSize) {
    if (hand == nullptr) {
        errorHandler(ERROR::error_0);
        return;
    }
    if (handSize < 0) {
        errorHandler(ERROR::error_2);
        return;
    }

    std::cout << "[ ";
    renderCard(hand[0]);
    for (int i = 1; i < handSize; i++) {
        std::cout << ", ";
        renderCard(hand[i]);
    }
    std::cout << " ]";
}

void renderSuit(const cardSuit suit) {
    if (suit == cardSuit::spades) {
        std::cout << "\033[97m" << "\xE2\x99\xA0";
    }
    else if (suit == cardSuit::hearts) {
        std::cout << "\033[31m" << "\xE2\x99\xA5";
    }
    else if (suit == cardSuit::clubs) {
        std::cout << "\033[97m" << "\xE2\x99\xA3";
    }
    else if (suit == cardSuit::diamonds) {
        std::cout << "\033[31m" << "\xE2\x99\xA6";
    }
    std::cout << "\033[37m";
}

void renderGameState(const gameInfo game) {
    for (int i = 0; i < game.cardsLeft; i++) {
        renderCard(game.deck[i]);
        std::cout << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < game.player1CardsLeft; i++) {
        renderCard(game.player1Hand[i]);
        std::cout << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < game.player2CardsLeft; i++) {
        renderCard(game.player2Hand[i]);
        std::cout << " ";
    }
    std::cout << std::endl;
}
