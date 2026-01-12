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

void sortCards(card* hand, int handSize) {
	for (int i = 0; i < handSize; i++) {
		for (int j = i; j < handSize - 1; j++) {
			if (hand[j].id > hand[j + 1].id) swap(&hand[j], &hand[j + 1]);
		}
	}
}