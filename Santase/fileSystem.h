#pragma once

#include "structs.h"

bool saveGame(const gameInfo& game, const char* filename);

bool loadGame(gameInfo& game, const char* filename);