#pragma once
#include "structs.h"

bool contains(const int*, int, int);

void setArray(card*, int, const card*, int);

void setArray(int*, int, const int*, int);

void swap(card*, card*);

int randomNumber(int, int);

void sortCards(card*, int);

void sortLastCard(card*, int);

void removeCard(card*, int*, int);