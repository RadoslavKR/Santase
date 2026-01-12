#include "game.h"
#include "algorithmUtils.h"
#include "errorHandler.h"
#include "screenRender.h"

#include "structs.h"
#include "env.h"

#include<iostream>

const card FULL_DECK[] = {
	{9, cardSuit::spades, 0},
	{10, cardSuit::spades, 1},
	{11, cardSuit::spades, 2},
	{12, cardSuit::spades, 3},
	{13, cardSuit::spades, 4},
	{14, cardSuit::spades, 5},

	{9, cardSuit::diamonds, 6},
	{10, cardSuit::diamonds, 7},
	{11, cardSuit::diamonds, 8},
	{12, cardSuit::diamonds, 9},
	{13, cardSuit::diamonds, 10},
	{14, cardSuit::diamonds, 11},

	{9, cardSuit::clubs, 12},
	{10, cardSuit::clubs, 13},
	{11, cardSuit::clubs, 14},
	{12, cardSuit::clubs, 15},
	{13, cardSuit::clubs, 16},
	{14, cardSuit::clubs, 17},

	{ 9, cardSuit::hearts, 18},
	{10, cardSuit::hearts, 19},
	{11, cardSuit::hearts, 20},
	{12, cardSuit::hearts, 21},
	{13, cardSuit::hearts, 22},
	{14, cardSuit::hearts, 23}
};

void shuffleCards(card *deck, int cardsInDeck) {
	if (deck == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}
	if (cardsInDeck < 0) {
		errorHandler(ERROR::error_2);
		return;
	}

	for (int i = 0; i < cardsInDeck; i++) {
		int randomIndex = randomNumber(0, cardsInDeck - 1);
		swap(&deck[i], &deck[randomIndex]);
	}
}

void drawCard(card* hand, int* cardsInHand, card* deck, int* cardsInDeck) {
	if (hand == nullptr || cardsInHand == nullptr || deck == nullptr || cardsInDeck == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}
	if (*cardsInHand >= MAX_CARDS_IN_HAND || *cardsInDeck <= 0) {
		errorHandler(ERROR::error_2);
		return;
	}
	hand[*cardsInHand] = deck[*cardsInDeck - 1];
	(*cardsInHand)++;
	(*cardsInDeck)--;
	sortCards(hand, *cardsInHand);
	renderHand(hand, *cardsInHand);
	std::cout << std::endl;
}

void dealCards(gameInfo &game) {
	int otherPlayer = game.playerTurn == 1 ? 2 : 1;
	drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
	drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
	drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
	
	drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
	drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
	drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
	
	drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
	drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
	drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);

	drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
	drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
	drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
}

void playTurn(gameInfo& game, bool shouldDraw = true) {
	int* cardsInHand = (game.playerTurn == 1 ? &game.player1CardsLeft : &game.player2CardsLeft);
	card hand[MAX_CARDS_IN_HAND];
	setArray(hand, MAX_CARDS_IN_DECK, (game.playerTurn == 1 ? game.player1Hand : game.player2Hand), (game.playerTurn == 1 ? game.player1CardsLeft : game.player2CardsLeft));

	if (shouldDraw) {
		drawCard(hand, cardsInHand, game.deck, &game.cardsLeft);
		std::cout << "You drew ";
		renderCard(hand[*cardsInHand - 1]);
		std::cout << std::endl;
	}

	std::cout << (game.playerTurn == 1 ? "P1's" : "P2's") << " turn" << std::endl;
	
	renderHand(hand, *cardsInHand);
	std::cout << std::endl;

	std::cout << "Trum suit is: ";
	renderSuit(game.trump);
	std::cout << std::endl;

	std::cout << "Bottom card: ";
	renderCard(game.deck[0]);
	std::cout << std::endl;

	std::cout << "Cards left in deck: " << game.cardsLeft << std :: endl;
}

void startRound(gameInfo& game) {
	setArray(game.deck, MAX_CARDS_IN_DECK, FULL_DECK, MAX_CARDS_IN_DECK);
	game.cardsLeft = MAX_CARDS_IN_DECK;
	shuffleCards(game.deck, game.cardsLeft);
	shuffleCards(game.deck, game.cardsLeft);
	dealCards(game);
	game.trump = game.deck[0].suit;
	playTurn(game, false);

}

void startGame() {
	gameInfo game;
	startRound(game);
}