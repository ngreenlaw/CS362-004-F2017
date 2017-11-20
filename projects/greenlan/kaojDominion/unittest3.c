// Unit test for updateCoins() function in dominion.c
//
// 		Usage:
// 			int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)
//
// 		Description:
// 			-adds cards to player based on toFlag
// 				-toFlag = 0: add to player's discard
// 				-toFlag = 1: add to player's deck
// 				-toFlag = 2: add to player's hand

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASS_STR "     ----- PASS ----- "
#define FAIL_STR "     ----- FAIL ----- "

typedef struct gameState gState;


void testGainCard(gState *initState, gState *postState, int supplyPos, int currPlayer, int toFlag) {
	int initCount, postCount, expectedCount;
	char to[32];
	memset(to, 0, 32);
	int cardToCompare, prevCard, expectedCard;
	int expectedSupply;
	
	switch(toFlag) {
		case 0 : // Discard
			initCount = initState->discardCount[currPlayer];
			postCount = postState->discardCount[currPlayer];
			cardToCompare = postState->discard[currPlayer][postCount-1];
			prevCard = initState->discard[currPlayer][initCount-1];
			strcpy(to, "Discard");
			break;
		case 1 : // Deck
			initCount = initState->deckCount[currPlayer];
			postCount = postState->deckCount[currPlayer];
			cardToCompare = postState->deck[currPlayer][postCount-1];
			prevCard = initState->deck[currPlayer][initCount-1];
			strcpy(to, "Deck");
			break;
		case 2 : // Hand
			initCount = initState->handCount[currPlayer];
			postCount = postState->handCount[currPlayer];
			cardToCompare = postState->hand[currPlayer][postCount-1];
			prevCard = initState->hand[currPlayer][initCount-1];
			strcpy(to, "Hand");
			break;
	}

	// Check if supply count of card is empty
	if (initState->supplyCount[supplyPos] == 0) {
		expectedCount = initCount;
		expectedCard = prevCard;
		expectedSupply = 0;
	} else {
		expectedCount = initCount + 1;
		expectedCard = supplyPos;
		expectedSupply = initState->supplyCount[supplyPos]-1;
	}


	// Compare counts
	printf("%s count: %d, expected: %d", to, postCount, expectedCount);
	if (postCount == expectedCount) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}

	// Compare top of destination
	printf("Top of %s: %d, expected: %d", to, cardToCompare, expectedCard);
	if (cardToCompare == expectedCard) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}

	// Compare supply count
	printf("Supply count of card: %d, expected: %d", postState->supplyCount[supplyPos],
		expectedSupply);
	if (postState->supplyCount[supplyPos] == expectedSupply) { 
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}

	printf("\n");
}



int main(int argc, char *argv[]) {
	int numPlayers = 2;
	int seedNum = 1000;
	int supplyPos = 0;
	gState G, postG;

	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};
	printf("Initializing game.\n");
	initializeGame(numPlayers, k, seedNum, &G);
	memcpy(&postG, &G, sizeof(gState));		// Copy game state
	int currPlayer = G.whoseTurn;
	printf("TEST 1 of gainCard() --- supplyPos: 0, toFlag: 0\n");
	gainCard(supplyPos, &postG, 0, currPlayer);
	testGainCard(&G, &postG, supplyPos, currPlayer, 0);	

	supplyPos = 1;
	memcpy(&G, &postG, sizeof(gState));		// Update G
	printf("TEST 2 of gainCard() --- supplyPos: 1, toFlag: 1\n");
	gainCard(supplyPos, &postG, 1, currPlayer);
	testGainCard(&G, &postG, supplyPos, currPlayer, 1);

	supplyPos = 2;
	memcpy(&G, &postG, sizeof(gState));		// Update G
	printf("TEST 3 of gainCard() --- supplyPos: 2, toFlag: 2\n");
	gainCard(supplyPos, &postG, 2, currPlayer);
	testGainCard(&G, &postG, supplyPos, currPlayer, 2);

	// Empty out silver supply
	postG.supplyCount[silver] = 0;
	memcpy(&G, &postG, sizeof(gState));
	supplyPos = 5;
	printf("TEST 4 of gainCard() --- supplyPos: 5, toFlag: 0, supply quantity: 0\n");
	gainCard(supplyPos, &postG, 0, currPlayer);
	testGainCard(&G, &postG, supplyPos, currPlayer, 0);

	return 0;
}
