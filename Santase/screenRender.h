#pragma once
#include "structs.h"

void renderPossibleCommands(const Commands*);

Instruction userInput(const Commands*);

void renderCard(const card);

void renderHand(const card*, int);

void renderSuit(const cardSuit);

void renderGameState(gameInfo);
