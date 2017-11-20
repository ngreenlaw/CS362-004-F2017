//	Tests the adventurer card in dominion.c
//
//	Effect of adventurer: 
//		-reveal cards from your deck until you reveal two treasure cards
//		-place treasure cards in your hand, and discard the other cards

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

void addCardToDeck(gState *G, int currPlayer, int card) {
	G->deck[currPlayer][G->deckCount[currPlayer]] = card;
	G->deckCount[currPlayer]++;
}

int getCardCount(int stack[], int stackCount, int card) {
	int i, count = 0;
	for (i = 0; i < stackCount; i++) {
		if (stack[i] == card) {
			count++;
		}
	}
	return count;
}

int main(int argc, char *argv[]) {
	int numPlayers = 2;
	int seedNum = 1000;
	gState G, postG;

	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};
	printf("Initializing game.\n");
	initializeGame(numPlayers, k, seedNum, &G);
	int currPlayer = G.whoseTurn;

	// Test 1
	// Add cards to player's deck
	addCardToDeck(&G, currPlayer, duchy);
	addCardToDeck(&G, currPlayer, silver);
	addCardToDeck(&G, currPlayer, smithy);
	addCardToDeck(&G, currPlayer, adventurer);
	addCardToDeck(&G, currPlayer, estate);
	addCardToDeck(&G, currPlayer, copper);
	addCardToDeck(&G, currPlayer, council_room);

	// Add adventurer card to player's hand
	G.hand[currPlayer][G.handCount[currPlayer]] = adventurer;
	G.handCount[currPlayer]++;

	// Copy game state
	memcpy(&postG, &G, sizeof(gState));
	// Play adventurer card
	int result = playCard(G.handCount[currPlayer]-1, 0, 0, 0, &postG);
	printf("TEST of adventurer card --- Top of deck (in order) before playing card: council room, copper, estate, adventurer, smithy, silver, duchy\n");
	printf("Result of playCard with adventurer: %d, expected: %d", result, 0);
	printPassFail(result, 0);

	// Hand should have an additional silver and copper
	int initCopperCount = getCardCount(G.hand[currPlayer], G.handCount[currPlayer], copper);
	int postCopperCount = getCardCount(postG.hand[currPlayer], postG.handCount[currPlayer], copper);
	int initSilverCount = getCardCount(G.hand[currPlayer], G.handCount[currPlayer], silver);
	int postSilverCount = getCardCount(postG.hand[currPlayer], postG.handCount[currPlayer], silver);
	printf("Copper count: %d, expected: %d", postCopperCount, initCopperCount+1);
	printPassFail(postCopperCount, initCopperCount+1); 
	printf("Silver count: %d, expected: %d", postSilverCount, initSilverCount+1);
	printPassFail(postSilverCount, initSilverCount+1); 
	
	// Handcount should have increased by one - gain two treasure cards, remove adventurer card from hand
	printf("Handcount: %d, expected: %d", postG.handCount[currPlayer], G.handCount[currPlayer]+1);
	printPassFail(postG.handCount[currPlayer], G.handCount[currPlayer]+1);

	// Top of deck should be duchy
	printf("Top of deck: %d, expected: %d", postG.deck[currPlayer][postG.deckCount[currPlayer]-1], duchy);
	printPassFail(postG.deck[currPlayer][postG.deckCount[currPlayer]-1], duchy);

	// Deck count should have decreased by six
	printf("Deck count: %d, expected: %d", postG.deckCount[currPlayer], G.deckCount[currPlayer]-6);
	printPassFail(postG.deckCount[currPlayer], G.deckCount[currPlayer]-6);
	
	// Top four cards of discard pile should match discarded cards
	printf("Top four cards of discard pile: ");
	int i, discardCard, discardMatch = 1;
	int discardTop[MAXCARD] = { 0 };
	int expectedDiscard[MAXCARD] = { 0 };
	for (i = 1; i < 5; i++) {
		discardCard = postG.discard[currPlayer][postG.discardCount[currPlayer]-i];
		printf("%d ", discardCard);
		discardTop[discardCard]++;
	}
	printf(", expected: %d %d %d %d", council_room, estate, adventurer, smithy);
	expectedDiscard[council_room]++;
	expectedDiscard[estate]++;
	expectedDiscard[adventurer]++;
	expectedDiscard[smithy]++;

	i = 0;
	while (discardMatch && i < MAXCARD) {
		if (discardTop[i] != expectedDiscard[i]) {
			discardMatch = 0;
		}
		i++;
	}
	printPassFail(discardMatch, 1);
	
	// Discard count should increase by four
	printf("Discard count: %d, expected: %d", postG.discardCount[currPlayer],
		G.discardCount[currPlayer] + 4);
	printPassFail(postG.discardCount[currPlayer], G.discardCount[currPlayer] + 4);

	printf("\n");
	// Check for changes in gameState
	printf("Checking for changes in gameState --- Error may be from dependent functions such as playCard, updateCoins, drawCard, etc\n");
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
	return 0;
}

