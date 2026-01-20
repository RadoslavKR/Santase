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
* <Game Logic>
*
*/

#include "game.h"
#include "algorithmUtils.h"
#include "stringUtils.h"
#include "errorHandler.h"
#include "screenRender.h"
#include "fileSystem.h"

#include "structs.h"
#include "env.h"

#include<iostream>

const card FULL_DECK[] = {
	{9, cardSuit::spades, 0},
	{10, cardSuit::spades, 10},
	{11, cardSuit::spades, 2},
	{12, cardSuit::spades, 3},
	{13, cardSuit::spades, 4},
	{14, cardSuit::spades, 11},

	{9, cardSuit::diamonds, 0},
	{10, cardSuit::diamonds, 10},
	{11, cardSuit::diamonds, 2},
	{12, cardSuit::diamonds, 3},
	{13, cardSuit::diamonds, 4},
	{14, cardSuit::diamonds, 11},

	{9, cardSuit::clubs, 0},
	{10, cardSuit::clubs, 10},
	{11, cardSuit::clubs, 2},
	{12, cardSuit::clubs, 3},
	{13, cardSuit::clubs, 4},
	{14, cardSuit::clubs, 11},

	{ 9, cardSuit::hearts, 0},
	{10, cardSuit::hearts, 10},
	{11, cardSuit::hearts, 2},
	{12, cardSuit::hearts, 3},
	{13, cardSuit::hearts, 4},
	{14, cardSuit::hearts, 11}
};
const int CARDS_IN_DECK_AT_START = 12;

card drawCard(card* hand, int* cardsInHand, card* deck, int* cardsInDeck) {
	if (hand == nullptr || cardsInHand == nullptr || deck == nullptr || cardsInDeck == nullptr) {
		errorHandler(ERROR::error_0);
		return {};
	}
	if (*cardsInHand >= MAX_CARDS_IN_HAND || *cardsInDeck <= 0) {
		errorHandler(ERROR::error_2);
		return {};
	}
	hand[*cardsInHand] = deck[*cardsInDeck - 1];
	(*cardsInHand)++;
	(*cardsInDeck)--;
	card res = hand[*cardsInHand - 1];
	sortLastCard(hand, *cardsInHand);
	return res;
}

//false card2 wins, true card1 wins
bool checkWhoGetsTrick(card card1, card card2, cardSuit trumpSuit) {
	if (card1.suit == card2.suit) {
		return card1.pts > card2.pts;
	}
	else {
		if (card2.suit == trumpSuit) return false;
		else return true;
	}
}

//returns negative if player 1 wins and positive if player 2 wins
int checkWinner(gameInfo& game) {
	if (!game.setting.lastTrickWins) {
		if (game.playerTurn == 1) game.player1Pts += 10;
		else game.player2Pts += 10;
	}
	else if (game.player1CardsLeft == 0) {
		if (game.playerTurn == 1) game.player1Pts += 1000;
		else game.player2Pts += 1000;
	}

	if (game.cardsLeft == 0 && !game.gameIsClosed) {
		if (game.player1Pts > game.player2Pts) {
			if (game.player2Pts >= 33) {
				game.player1WonGamePts += 1;
				return -1;
			}
			else if (game.player2Pts == 0) {
				game.player1WonGamePts += 3;
				return -3;
			}
			else {
				game.player1WonGamePts += 2;
				return -2;
			}
		}
		else if (game.player2Pts < game.player1Pts) {
			if (game.player1Pts >= 33) {
				game.player2WonGamePts += 1;
				return 1;
			}
			else if (game.player1Pts == 0) {
				game.player2WonGamePts += 3;
				return 3;
			}
			else {
				game.player2WonGamePts += 2;
				return 2;
			}
		}
	}
	else {
		if (game.playerWhoCloedTheGame == 1) {
			if (game.player1Pts >= 66) {
				if (game.player2Pts >= 33) {
					game.player1WonGamePts += 1;
					return -1;
				}
				else if (game.player2Pts == 0) {
					game.player1WonGamePts += 3;
					return -3;
				}
				else {
					game.player2WonGamePts += 2;
					return -2;
				}
			}
			else {
				game.player2WonGamePts += 3;
				return 3;
			}
		}
		else {
			if (game.player2Pts >= 66) {
				if (game.player1Pts >= 33) {
					game.player2WonGamePts += 1;
					return 1;
				}
				else if (game.player1Pts == 0) {
					game.player1WonGamePts += 3;
					return 3;
				}
				else {
					game.player2WonGamePts += 2;
					return 2;
				}
			}
			else {
				game.player1WonGamePts += 3;
				return -3;
			}
		}
	}
}

bool checkForMarriage(const card* hand, int cardsInHand, int playCardId) {
	if (hand == nullptr) {
		errorHandler(ERROR::error_0);
		return false;
	}
	if (cardsInHand <= 0 || playCardId < 0 || playCardId >= cardsInHand) {
		errorHandler(ERROR::error_2);
		return false;
	}

	if (hand[playCardId].number == 12 && hand[playCardId + 1].number == 13 && hand[playCardId].suit == hand[playCardId + 1].suit) {
		return true;
	}
	else if (hand[playCardId].number == 13 && hand[playCardId - 1].number == 12 && hand[playCardId].suit == hand[playCardId - 1].suit) {
		return true;
	}

	return false;
}

void declareMarriage(gameInfo& game,const card* hand, int cardsInHand, int playCardId) {
	std::cout << "Marriage declared." << std::endl;;
	if (hand[playCardId].suit == game.trump) {
		if (game.playerTurn == 1) game.player1Pts += game.setting.trumpMarriagePoints;
		else game.player2Pts += game.setting.trumpMarriagePoints;
		std::cout << "You won " << game.setting.trumpMarriagePoints << "pts" << std::endl << std::endl;
	}
	else {
		if (game.playerTurn == 1) game.player1Pts += game.setting.marriagePoints;
		else game.player2Pts += game.setting.marriagePoints;
		std::cout << "You won " << game.setting.marriagePoints << "pts" << std::endl << std::endl;
	}
}

void playCard(gameInfo& game, int playCardId) {
	int* cardsInHand = (game.playerTurn == 1 ? &game.player1CardsLeft : &game.player2CardsLeft);
	card* hand = (game.playerTurn == 1 ? game.player1Hand : game.player2Hand);

	if (checkForMarriage(hand, *cardsInHand, playCardId)) {
		declareMarriage(game, hand, *cardsInHand, playCardId);
	}

	if (game.player1CardsLeft != game.player2CardsLeft) {
		game.lastTrick.cardPlayedP1 = (game.playerTurn == 1 ? hand[playCardId] : game.playedCard);
		game.lastTrick.cardPlayedP2 = (game.playerTurn == 2 ? hand[playCardId] : game.playedCard);
		bool whoWon = checkWhoGetsTrick(game.playedCard, hand[playCardId], game.trump);
		if (game.playerTurn == 1) {
			if (whoWon) game.lastTrick.playerWhoWon = 2;
			else game.lastTrick.playerWhoWon = 1;
		}
		else {
			if (whoWon) game.lastTrick.playerWhoWon = 1;
			else game.lastTrick.playerWhoWon = 2;
		}
		if (whoWon) {
			game.playerTurn = (game.playerTurn == 1 ? 2 : 1);
		}

		if (game.playerTurn == 1)  game.player1Pts += game.playedCard.pts + hand[playCardId].pts;
		else game.player2Pts += game.playedCard.pts + hand[playCardId].pts;

		removeCard(hand, cardsInHand, playCardId);
		if (game.playerTurn == 1 && !game.gameIsClosed) {
			game.lastTrick.cardDrawnP1 = drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
			game.lastTrick.cardDrawnP2 = drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
		}
		else if (!game.gameIsClosed) {
			game.lastTrick.cardDrawnP2 = drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
			game.lastTrick.cardDrawnP1 = drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
		}
	}
	else {
		game.playedCard = hand[playCardId];
		removeCard(hand, cardsInHand, playCardId);
		game.playerTurn = (game.playerTurn == 1 ? 2 : 1);
	}

	if (game.cardsLeft == 0) game.gameIsClosed = true;
}

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

void dealCards(gameInfo &game) {
	int otherPlayer = game.playerTurn == 1 ? 2 : 1;
	if (game.playerTurn == 1) {
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
	else {
		drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
		drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
		drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);

		drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
		drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
		drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);

		drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
		drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);
		drawCard(game.player2Hand, &game.player2CardsLeft, game.deck, &game.cardsLeft);

		drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
		drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
		drawCard(game.player1Hand, &game.player1CardsLeft, game.deck, &game.cardsLeft);
	}

}

void checkPossibleMoves(gameInfo& game, int* params, int* paramCounter) {
	int* cardsInHand = (game.playerTurn == 1 ? &game.player1CardsLeft : &game.player2CardsLeft);
	card* hand = (game.playerTurn == 1 ? game.player1Hand : game.player2Hand);

	if (game.gameIsClosed && (game.player1CardsLeft < game.player2CardsLeft || game.player2CardsLeft < game.player1CardsLeft)) {
		cardSuit playableSuit = game.playedCard.suit;
		bool hasSuit = false;
		for (int i = 0; i < *cardsInHand; i++) {
			if (playableSuit == hand[i].suit) {
				hasSuit = true;
				params[(*paramCounter)++] = i;
			}
		}
		if (!hasSuit) {
			bool hasTrump = false;
			for (int i = 0; i < *cardsInHand; i++) {
				if (hand[i].suit == game.trump) {
					params[(*paramCounter)++] = i;
					hasTrump = true;
				}
			}
			
			if (!hasTrump) {
				for (int i = 0; i < *cardsInHand; i++) {
					params[(*paramCounter)++] = i;
				}
			}
		}
	}
	else {
		for (int i = 0; i < *cardsInHand; i++) {
			params[i] = i;
		}
		*paramCounter = *cardsInHand;
	}
}

bool checkForNine(gameInfo &game) {
	int* cardsInHand = (game.playerTurn == 1 ? &game.player1CardsLeft : &game.player2CardsLeft);
	card* hand = (game.playerTurn == 1 ? game.player1Hand : game.player2Hand);

	if (game.player1CardsLeft != game.player2CardsLeft) return false;
	if (game.cardsLeft <= 2) return false;
	if (game.playerTurn == 1 && game.cardsLeft == CARDS_IN_DECK_AT_START) return false;
	if (game.playerTurn == 2 && game.cardsLeft == CARDS_IN_DECK_AT_START) return false;

	for (int i = 0; i < *cardsInHand; i++) {
		if (hand[i].suit == game.trump && hand[i].number == 9) return true;
	}

	return false;
}

bool checkForClose(gameInfo& game) {
	if (game.player1CardsLeft != game.player2CardsLeft) return false;
	if (game.cardsLeft == CARDS_IN_DECK_AT_START) return false;
	if (game.cardsLeft <= 2) return false;
	if (game.gameIsClosed) return false;

	return true;
}

bool checkForStop(gameInfo& game) {
	if (game.player1CardsLeft != game.player2CardsLeft) return false;
	return true;
}

void close(gameInfo& game) {
	std::cout << "Stock closed. No more cards will be drawn." << std::endl;
	std::cout << "Strict rules are now in effect" << std::endl << std::endl << std::endl;
	game.gameIsClosed = true;
	game.playerWhoCloedTheGame = game.playerTurn;
}

void switchNine(gameInfo &game) {
	int* cardsInHand = (game.playerTurn == 1 ? &game.player1CardsLeft : &game.player2CardsLeft);
	card* hand = (game.playerTurn == 1 ? game.player1Hand : game.player2Hand);
	std::cout << "You exchanged ";
	int i;
	for (i = 0; i < *cardsInHand; i++) {
		if (hand[i].suit == game.trump && hand[i].number == 9) break;
	}
	renderCard(hand[i]);
	std::cout << " for ";
	renderCard(game.deck[0]);
	std::cout << std::endl;

	swap(&game.deck[0], &hand[i]);
	sortCards(hand, *cardsInHand);
}

void stop(gameInfo& game) {
	game.player1CardsLeft = 0;
	game.player2CardsLeft = 0;
	game.gameIsClosed = true;
	game.playerWhoCloedTheGame = game.playerTurn;
}

void surrenderFunc(gameInfo& game) {
	game.player1CardsLeft = 0;
	game.player2CardsLeft = 0;
	if (game.playerTurn == 1) {
		game.player2Pts = 66;
		if (!game.gameIsClosed) game.playerWhoCloedTheGame = 2;
	}
	else {
		game.player1Pts = 66;
		if (!game.gameIsClosed) game.playerWhoCloedTheGame = 1;
	}
	game.gameIsClosed = true;
}

void surrenderForeverFunc(gameInfo& game) {
	if (game.playerTurn == 1) {
		game.player2WonGamePts = game.setting.neededPointsToWin;
		surrenderFunc(game);
	}
	else {
		game.player1WonGamePts = game.setting.neededPointsToWin;
		surrenderFunc(game);
	}
}

void showHistoryFunc(gameInfo& game) {
	for (int i = 0; i < game.playedGames; i++) {
		std::cout << "Round " << i + 1 << ": Winner - Player " << (game.gameHistory[i].playerWhoWon);
		std::cout << "(+" << game.gameHistory[i].wonPoints << ") | Player 1: ";
		std::cout << game.gameHistory[i].player1Points << " points | Player 2: " << game.gameHistory[i].player2Points;
		std::cout << " points" << std :: endl;
	}
	std::cout << "Round " << game.playedGames + 1 << ": Ongoing" << std::endl;
	std::cout << "Overall: Player 1 - " << game.player1WonGamePts << "| Player 2 - " << game.player2WonGamePts << std::endl;

}

//return the type of victory 1 no victory 2 surrender 3 foerver surrender
int showPlayerOptions(gameInfo& game) {
	Command play = { "play" };
	Command surrender = { "surrender" };
	Command surrenderForever = { "surrender-forever" };
	Command save = { "save", 0, {}, true, true };
	Command load = { "load", 0, {}, true, true };
	Command showHistory = { "history" };
	checkPossibleMoves(game, play.params, &play.numOfPossibleParams);
	Commands commands = {
		{showHistory, save, load, play, surrender, surrenderForever},
		6
	};

	if (checkForNine(game)) {
		Command switchNine = { "switch-nine" };
		commands.commands[commands.numOfCommands++] = switchNine;
	}

	if (checkForClose(game)) {
		Command close = { "close" };
		commands.commands[commands.numOfCommands++] = close;
	}

	if (checkForStop(game)) {
		Command stop = { "stop" };
		commands.commands[commands.numOfCommands++] = stop;
	}

	renderPossibleCommands(&commands);
	Instruction instr = userInput(&commands);
	if (strCompare(commands.commands[instr.commandId].name, "play")) {
		playCard(game, instr.commandParam);
	}
	else if (strCompare(commands.commands[instr.commandId].name, "switch-nine")) {
		switchNine(game);
	}
	else if (strCompare(commands.commands[instr.commandId].name, "close")) {
		close(game);
	}
	else if (strCompare(commands.commands[instr.commandId].name, "stop")) {
		stop(game);
	}
	else if (strCompare(commands.commands[instr.commandId].name, "surrender")) {
		surrenderFunc(game);
		return 2;
	}
	else if (strCompare(commands.commands[instr.commandId].name, "surrender-forever")) {
		surrenderForeverFunc(game);
		return 3;
	}
	else if (strCompare(commands.commands[instr.commandId].name, "save")) {
		if (saveGame(game, instr.strParam)) {
			std::cout << "Game Saved in " << instr.strParam << ".txt" << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
		}
		else {
			std::cout << "Unsuccessful save" << std::endl << std::endl;
		}
	}
	else if (strCompare(commands.commands[instr.commandId].name, "load")) {
		if (loadGame(game, instr.strParam)) {
			std::cout << "Loaded game from " << instr.strParam << ".txt" << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
		}
		else {
			std::cout << "Unseccessfully loaded " << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
		}

	}
	else if (strCompare(commands.commands[instr.commandId].name, "history")) {
		showHistoryFunc(game);
	}
	return 1;
}

//return the type of victory 1 no victory 2 surrender 3 foerver surrender
int playTurn(gameInfo& game, bool shouldDraw = true) {
	int* cardsInHand = (game.playerTurn == 1 ? &game.player1CardsLeft : &game.player2CardsLeft);
	card* hand = (game.playerTurn == 1 ? game.player1Hand : game.player2Hand);

	std::cout << (game.playerTurn == 1 ? "P1's" : "P2's") << " turn" << std::endl;
	
	if (game.cardsLeft < CARDS_IN_DECK_AT_START && game.player1CardsLeft == MAX_CARDS_IN_HAND) {
		std::cout << "You drew: ";
		if (game.playerTurn == 1) renderCard(game.lastTrick.cardDrawnP1);
		else renderCard(game.lastTrick.cardDrawnP2);
		std::cout << std::endl;
	}

	renderHand(hand, *cardsInHand);
	std::cout << std::endl << std :: endl;
	std::cout << "Trum suit is: ";
	renderSuit(game.trump);
	std::cout << std::endl;

	std::cout << "Bottom card: ";
	renderCard(game.deck[0]);
	std::cout << std::endl;

	std::cout << "Cards left in deck: " << game.cardsLeft << std :: endl;

	std::cout << std::endl;

	if (game.setting.showPoints) {
		std::cout << "Your points are: " << (game.playerTurn == 1 ? game.player1Pts : game.player2Pts) << std::endl << std::endl;
	}

	if (game.cardsLeft < CARDS_IN_DECK_AT_START) {
		std::cout << "Last trick: " << std::endl;
		std::cout << "Player 1: "; 
		renderCard(game.lastTrick.cardPlayedP1);
		std::cout << std::endl;
		std::cout << "Player 2: "; 
		renderCard(game.lastTrick.cardPlayedP2);
		std::cout << std::endl;
		std::cout << "Winner: " << (game.lastTrick.playerWhoWon == 1 ? "Player 1" : "Player 2") << std::endl << std :: endl;
	}
	
	if (game.player1CardsLeft != game.player2CardsLeft) {
		std::cout << "Your oponent played: ";
		renderCard(game.playedCard);
		std::cout << std::endl;
	}

	return showPlayerOptions(game);
}

void moveTopCardToBottom(card* deck, const int cardsInDeck) {
	if (deck == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	}

	for (int i = cardsInDeck - 2; i >= 0; i--) {
		swap(&deck[i + 1], &deck[i]);
	}
}

void clearBeforeRound(gameInfo& game) {
	game.player1Pts = 0;
	game.player2Pts = 0;
	game.player1CardsLeft = 0;
	game.player2CardsLeft = 0;
	game.gameIsClosed = false;
	setArray(game.deck, MAX_CARDS_IN_DECK, FULL_DECK, MAX_CARDS_IN_DECK);
	game.cardsLeft = MAX_CARDS_IN_DECK;
	shuffleCards(game.deck, game.cardsLeft);
	shuffleCards(game.deck, game.cardsLeft);
	shuffleCards(game.deck, game.cardsLeft);
	dealCards(game);
	moveTopCardToBottom(game.deck, game.cardsLeft);
	game.trump = game.deck[0].suit;
}

void enterHistory(gameInfo& game, int winner) {
	game.gameHistory[game.playedGames].player1Points = game.player1Pts;
	game.gameHistory[game.playedGames].player2Points = game.player2Pts;
	game.gameHistory[game.playedGames].playerWhoWon = (winner > 0 ? 2 : 1);
	game.gameHistory[game.playedGames].wonPoints = (winner > 0 ? winner : -winner);
	game.playedGames++;
}

void startRound(gameInfo& game) {
	std::cout << "Starting new round." << std::endl << std::endl;
	clearBeforeRound(game);

	int victoryType = 1;
	while (true) {
		if (game.player1CardsLeft == 0 && game.player2CardsLeft == 0) {
			int winner = checkWinner(game);
			enterHistory(game, winner);
			std::cout << std::endl;
			if (victoryType == 1 && ((game.setting.lastTrickWins && game.player1CardsLeft != 0) || !game.setting.lastTrickWins)) {
				std::cout << "Plaeyr 1 has " << game.player1Pts << "pts" << std::endl;
				std::cout << "Player 2 has " << game.player2Pts << "pts" << std::endl;
				std::cout << std::endl;
			}
			if (victoryType != 3) {
				if (winner < 0) {
					game.playerTurn = 2;
					std::cout << "The Winner is player 1 and he gets: " << -winner << "pts." << std::endl;
				}
				else {
					std::cout << "The Winner is player 2 and he gets: " << winner << "pts." << std::endl;
					game.playerTurn = 1;
				}

				std::cout << std::endl;

				std::cout << "Current Round points are:" << std::endl;
				std::cout << "Player 1: " << game.player1WonGamePts << std::endl;
				std::cout << "Player 2: " << game.player2WonGamePts << std::endl;
				std::cout << std::endl;
			}

			return;
		}
		victoryType = playTurn(game, false);
	}
	
}

void startGame(settings setting) {
	gameInfo game;
	game.setting = setting;
	while (game.player1WonGamePts < game.setting.neededPointsToWin && game.player2WonGamePts < game.setting.neededPointsToWin) {
		startRound(game);
	}
	std::cout << std::endl;
	std::cout << "The winner is " << (game.player1WonGamePts > game.player2WonGamePts ? "Player 1" : "Player 2") << std::endl << std::endl;
}