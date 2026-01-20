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
* <General helping algorithms>
*
*/


#include "algorithmUtils.h"
#include "errorHandler.h"

#include "structs.h"

#include<cstdlib>
#include<ctime>
#include<iostream>

bool contains(const int* arr, int size, int x) {
	if (arr == nullptr) {
		errorHandler(ERROR::error_0);
		return false;
	}
	if (size < 0) {
		errorHandler(ERROR::error_2);
		return false;
	}

	for (int i = 0; i < size; i++) {
		if (arr[i] == x) return true;
	}

	return false;
}

void setArray(card* target, int target_size, const card* src, int src_size) {
	if (target == nullptr || src == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}
	if (target_size < src_size) {
		errorHandler(ERROR::error_2);
		return;
	}

	for (int i = 0, j = 0; j < src_size; i++, j++) {
		target[i] = src[j];
	}
}

void setArray(int* target, int target_size, const int* src, int src_size) {
	if (target == nullptr || src == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}
	if (target_size < src_size) {
		errorHandler(ERROR::error_2);
		return;
	}

	for (int i = 0, j = 0; j < src_size; i++, j++) {
		target[i] = src[j];
	}
}

void swap(card* a, card* b) {
	if (a == nullptr || b == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}

	card tmp = *a;
	*a = *b;
	*b = tmp;
}

int randomNumber(int mini, int maxi) {
	return (rand() % (maxi - mini + 1)) + mini;
}

// false - balck true - red
bool getColorOfCardSuit(const card xCard) {
	if (xCard.suit == cardSuit::clubs || xCard.suit == cardSuit::spades) {
		return false;
	}
	else return true;
}

void sortCards(card* hand, int handSize) {
	if (hand == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}

	for (int i = 1; i <= handSize; i++) {
		sortLastCard(hand, i);
	}
}

void sortLastCard(card* hand, int handSize) {
	if (hand == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}
	if (handSize <= 0) {
		errorHandler(ERROR::error_2);
		return;
	}
	card sortCard = hand[handSize - 1];

	bool stop = false;
	//check if there is where we can put the card
	for (int i = handSize - 2; i >= 0; i--) {
		if (sortCard.suit == hand[i].suit) {
			stop = true;
			if (sortCard.pts < hand[i].pts) swap(&hand[i + 1], &hand[i]);
			else return;
		}
		else if (!stop) swap(&hand[i + 1], &hand[i]);
		else return;
	}
	if (stop) return;

	//we havent found a suit in the hand
	for (int i = 1; i < handSize; i++) {
		if (getColorOfCardSuit(hand[i]) != getColorOfCardSuit(sortCard)) {
			if (i - 2 < 0) return;
			else if (getColorOfCardSuit(hand[i - 2]) != getColorOfCardSuit(sortCard) && hand[i - 2].suit != hand[i].suit) {
				return;
			}
		}
		swap(&hand[i - 1], &hand[i]);
	}
}

void removeCard(card* hand, int* handSize, int cardIdToRemove) {
	if (hand == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}
	if (*handSize < 0 || cardIdToRemove < 0 || cardIdToRemove >= *handSize) {
		errorHandler(ERROR::error_2);
		return;
	}

	for (int i = cardIdToRemove + 1; i < *handSize; i++) {
		hand[i - 1] = hand[i];
	}
	(*handSize)--;

	sortCards(hand, *handSize);

}