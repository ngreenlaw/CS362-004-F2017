// Tests the village card in dominion.c
//
//  Effect of playing village:
//  	-player gets one additional card from deck
//  	-player gets +2 actions
//

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCARD treasure_map
#define PASS_STR "     ----- PASS ----- "
#define FAIL_STR "     ----- FAIL ----- "

typedef struct gameState gState;

void printPassFail(int param1, int param2) {
	if (param1 == param2) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}
}

int main(int argc, char *argv[]) {
	int numPlayers = 2;
	int seedNum = 3000;
	int i, result;
	gState G, postG;

	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};
	printf("Initializing game.\n");
	initializeGame(numPlayers, k, seedNum, &G);
	int currPlayer = G.whoseTurn;

	// Add village card to player's hand
	G.hand[currPlayer][G.handCount[currPlayer]] = village;
	G.handCount[currPlayer]++;

	// Copy state
	memcpy(&postG, &G, sizeof(gState));
	result = playCard(G.handCount[currPlayer]-1, 0, 0, 0, &postG);

	printf("TEST of village card --- numPlayers: 2, status: new game\n");

	// Playing village should return 0
	printf("Return value of playCard with village: %d, expected: %d", result, 0);
	printPassFail(result, 0);

	// Player's hand count should have remained the same: +1 card, then remove village card from hand
	int initHandCount = G.handCount[currPlayer];
	int postHandCount = postG.handCount[currPlayer];
	printf("Hand count: %d, expected: %d", initHandCount, postHandCount);
	printPassFail(initHandCount, postHandCount);

	// Added card should be the previous top card of deck
	int addedCard = postG.hand[currPlayer][postG.handCount[currPlayer]-1];
	int prevDeckCard = G.deck[currPlayer][G.deckCount[currPlayer]-1];
	printf("Added card: %d, expected: %d", addedCard, prevDeckCard);
	printPassFail(addedCard, prevDeckCard);
	
	// Deck count should have decreased by one
	printf("Deck count: %d, expected: %d", postG.deckCount[currPlayer], G.deckCount[currPlayer]-1);
	printPassFail(postG.deckCount[currPlayer], G.deckCount[currPlayer]-1);
	
	// Number of actions should have increased by one: consumed one by playing village, then +2
	printf("Number of actions: %d, expected: %d", postG.numActions, G.numActions+1);
	printPassFail(postG.numActions, G.numActions+1);
	
	// Top of playedCards should be village
	printf("Top of played cards: %d, expected: %d", postG.playedCards[postG.playedCardCount-1],
		village);
	printPassFail(postG.playedCards[postG.playedCardCount-1], village);
	
	// Played card count should have increased by one
	printf("Played card count: %d, expected: %d", postG.playedCardCount, G.playedCardCount + 1);
	printPassFail(postG.playedCardCount, G.playedCardCount + 1);

	// Check for changes in gameState
	printf("Number of players: %d, expected: %d", postG.numPlayers, G.numPlayers);
	printPassFail(postG.numPlayers, G.numPlayers);
	printf("Whose turn: %d, expected: %d", postG.whoseTurn, G.whoseTurn);
	printPassFail(postG.whoseTurn, G.whoseTurn);
	printf("Phase: %d, expected: %d", postG.phase, G.phase);
	printPassFail(postG.phase, G.phase);
	printf("Number of buys: %d, expected: %d", postG.numBuys, G.numBuys);
	printPassFail(postG.numBuys, G.numBuys);
	
	// Check for changes in supply count
	i = 0;
	int supplyMatch = 1;
	while (supplyMatch && i < MAXCARD+1) {
		if (postG.supplyCount[i] != G.supplyCount[i]) {
			supplyMatch = 0;
		}
		i++;
	}
	printf("Supply count remains the same");
	printPassFail(supplyMatch, 1);
	
	// Check card counts of other player
	for (i = 0; i < numPlayers; i++) {
		if (i != currPlayer) {
			printf("Player %d hand count: %d, expected: %d", i, postG.handCount[i], G.handCount[i]);
			printPassFail(postG.handCount[i], G.handCount[i]);
			printf("Player %d deck count: %d, expected: %d", i, postG.deckCount[i], G.deckCount[i]);
			printPassFail(postG.deckCount[i], G.deckCount[i]);
			printf("Player %d discard count: %d, expected: %d", i, postG.discardCount[i], G.discardCount[i]);
			printPassFail(postG.discardCount[i], G.discardCount[i]);
		}
	}

	return 0;
}








