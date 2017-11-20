// Tests the discardCard function
// Usage:
// 		int discardCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag)
//
// 		Assertions:
//			-if the trashFlag is not set
//				-player's handcount should be decremented
//				-card which was discarded should no longer be in player's hand
//				-current player's discardCount should be decremented by one
//				-top of discard pile of current player should be the same card which was discarded
//			-if the trashFlag is set
//				-player's handcount should be decremented
//				-card which was trashed should no longer be in player's hand
//				-player's discardCount should remain the same
//				-top of discard pile should be the same as before
//			-other conditions should not change
//
// 			-cases: 
// 				-player discards last card in their hand
// 				-player discards last card in hand array
// 				-player discards a card (not meeting conditions above)
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

int getCardDistribution(int arr[], int currPlayer, gState *state) {
	int i, card;
	for (i = 0; i < state->handCount[currPlayer]; i++) {
		card = state->hand[currPlayer][i];
		arr[card]++;
	}
	return 0;
}


void testDiscard(gState *initState, gState *postState, int handPos, int initCardDist[], int postCardDist[], int trashFlag) {
	int currPlayer = initState->whoseTurn;
	int card = initState->hand[currPlayer][handPos];
	int i;

	// Handcount should be decremented by one
	printf("handcount: %d, expected: %d", postState->handCount[currPlayer], initState->handCount[currPlayer] - 1);
	if (postState->handCount[currPlayer] == initState->handCount[currPlayer] - 1) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}
	// Count for other cards in player's hand should remain the same	
	for (i = 0; i < MAXCARD; i++) {
		if (i == card) {
			printf("card %d --- count: %d, expected %d", i, postCardDist[i], initCardDist[i]-1);
			if (postCardDist[i] == initCardDist[i] - 1) {
				printf("%s\n", PASS_STR);
			} else {
				printf("%s\n", FAIL_STR);
			}
		} else {
			printf("card %d --- count: %d, expected %d", i, postCardDist[i], initCardDist[i]);
			if (postCardDist[i] == initCardDist[i]) {
				printf("%s\n", PASS_STR);
			} else {
				printf("%s\n", FAIL_STR);
			}
		}
	}

	if (trashFlag) {
		// Discard count should remain the same
		printf("discard count: %d, expected: %d", postState->discardCount[currPlayer], initState->discardCount[currPlayer]);
		if (postState->discardCount[currPlayer] == initState->discardCount[currPlayer]) {
			printf("%s\n", PASS_STR);
		} else {
			printf("%s\n", FAIL_STR);
		}
		// Top of discard pile should be the same as before
		printf("top of discard pile: %d, expected: %d",
			postState->discard[currPlayer][postState->discardCount[currPlayer]-1],
			initState->discard[currPlayer][initState->discardCount[currPlayer]-1]);
		if (postState->discard[currPlayer][postState->discardCount[currPlayer]-1] == 
			initState->discard[currPlayer][initState->discardCount[currPlayer]-1]) {
			printf("%s\n", PASS_STR);
		} else {
			printf("%s\n", FAIL_STR);
		}
	} else {
		// Discard count should increase by one
		printf("discard count: %d, expected: %d", postState->discardCount[currPlayer], initState->discardCount[currPlayer] + 1);
		if (postState->discardCount[currPlayer] == initState->discardCount[currPlayer] + 1) {
			printf("%s\n", PASS_STR);
		} else {
			printf("%s\n", FAIL_STR);
		}
		// Top of discard pile should be the card which was discarded
		printf("top of discard pile: %d, expected: %d",
			postState->discard[currPlayer][postState->discardCount[currPlayer]-1], card);
		if (postState->discard[currPlayer][postState->discardCount[currPlayer]-1] == card) {
			printf("%s\n", PASS_STR);
		} else {
			printf("%s\n", FAIL_STR);
		}
	}
}


int main(int argc, char *argv[]) {
	int numPlayers = 2;
	int seedNum = 100;
	int handPos = 0;
	int trashFlag = 0;
	int initCardDist[MAXCARD] = { 0 };	// counts number of each type of card in player's hand
	int postCardDist[MAXCARD] = { 0 };

	gState G, postDiscardG;
	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};

	printf("Initializing game.\n");
	initializeGame(numPlayers, k, seedNum, &G);
	int currPlayer = G.whoseTurn;
	memcpy(&postDiscardG, &G, sizeof(gState));	// Copy game state

	printf("TEST 1 of discardCard() --- handpos: 0, trashFlag: 0\n\n");
	discardCard(handPos, currPlayer, &postDiscardG, trashFlag);
	getCardDistribution(initCardDist, currPlayer, &G);
	getCardDistribution(postCardDist, currPlayer, &postDiscardG);
	testDiscard(&G, &postDiscardG, handPos, initCardDist, postCardDist, 0);
	printf("\n");

	memset(initCardDist, 0, sizeof(int)*MAXCARD);
	memset(postCardDist, 0, sizeof(int)*MAXCARD);
	memcpy(&postDiscardG, &G, sizeof(gState));
	handPos = G.hand[currPlayer][G.handCount[currPlayer]-1];
	printf("TEST 2 of discardCard() --- handpos: handCount-1, trashFlag: 0\n\n");
	discardCard(handPos, currPlayer, &postDiscardG, trashFlag);
	getCardDistribution(initCardDist, currPlayer, &G);
	getCardDistribution(postCardDist, currPlayer, &postDiscardG);
	testDiscard(&G, &postDiscardG, handPos, initCardDist, postCardDist, 0);
	printf("\n");

	memset(initCardDist, 0, sizeof(int)*MAXCARD);
	memset(postCardDist, 0, sizeof(int)*MAXCARD);
	memcpy(&postDiscardG, &G, sizeof(gState));
	handPos = 3;
	printf("TEST 3 of discardCard() --- handpos: 3, trashFlag: 0\n\n");
	discardCard(handPos, currPlayer, &postDiscardG, trashFlag);
	getCardDistribution(initCardDist, currPlayer, &G);
	getCardDistribution(postCardDist, currPlayer, &postDiscardG);
	testDiscard(&G, &postDiscardG, handPos, initCardDist, postCardDist, 0);
	printf("\n");

	memset(initCardDist, 0, sizeof(int)*MAXCARD);
	memset(postCardDist, 0, sizeof(int)*MAXCARD);
	memcpy(&postDiscardG, &G, sizeof(gState));
	printf("TEST 4 of discardCard() --- handpos: 0, trashFlag: 1\n\n");
	discardCard(handPos, currPlayer, &postDiscardG, trashFlag);
	getCardDistribution(initCardDist, currPlayer, &G);
	getCardDistribution(postCardDist, currPlayer, &postDiscardG);
	testDiscard(&G, &postDiscardG, handPos, initCardDist, postCardDist, 1);
	printf("\n");


	return 0;
}
