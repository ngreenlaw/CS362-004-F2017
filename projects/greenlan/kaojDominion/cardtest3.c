// Tests the smithy card in dominion.c
//
//		Effect of playing smithy:
//			-player gains 3 cards
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

void testSmithy(gState *G, gState *postG, int currPlayer) {
	// Check that card count has increased by 2: +3 cards, -1 for removing played smithy
	int initHandCount = G->handCount[currPlayer];
	int postHandCount = postG->handCount[currPlayer];
	printf("Hand count: %d, expected: %d", postHandCount, initHandCount+2);
	printPassFail(postHandCount, initHandCount+2);

	// Check that player's deck count has decreased by 3, unless initial deck count was less than 3
	if (G->deckCount[currPlayer] >= 3) {
		int initDeckCount = G->deckCount[currPlayer];
		int postDeckCount = postG->deckCount[currPlayer];
		printf("Deck count: %d, expected: %d", postDeckCount, initDeckCount-3);
		printPassFail(postDeckCount, initDeckCount-3);
	}

	// Check that player's added cards came from top of player's deck
	int i, cardsMatch = 1;
	int handDist[MAXCARD] = { 0 };
	int expectedHandDist[MAXCARD] = { 0 };
	// Check if the cards in hand match added cards from deck
	printf("Current hand: ");
	for (i = 0; i < postG->handCount[currPlayer]; i++) {
		printf("%d ", postG->hand[currPlayer][i]);
		handDist[postG->hand[currPlayer][i]]++;
	}
	printf(", expected hand: ");
	// Cards previous in hand, excluding smithy card
	for (i = 0; i < G->handCount[currPlayer]-1; i++) {
		printf("%d ", G->hand[currPlayer][i]);
		expectedHandDist[G->hand[currPlayer][i]]++;
	}
	// Cards previously on top of deck
	for (i = 0; i < 3; i++) {
		printf("%d ", G->deck[currPlayer][G->deckCount[currPlayer]-1-i]);
		expectedHandDist[G->deck[currPlayer][G->deckCount[currPlayer]-1-i]]++;
	}
	i = 0;
	// Check if cards match
	while (cardsMatch && i < MAXCARD) {
		if (handDist[i] != expectedHandDist[i]) {
			cardsMatch = 0;
		}
		i++;
	}
	printPassFail(cardsMatch, 1);

	// Check for changes in gameState
	// Check for changes in supplyCount
	printf("Supply count stays the same");
	int supplySame = 1;
	i = 0;
	while (supplySame && i < MAXCARD+1) {
		if (G->supplyCount[i] != postG->supplyCount[i]) {
			supplySame = 0;
		}
		i++;
	}
	printPassFail(supplySame, 1);


	// Check for changes in outpost variables
	printf("Outpost played: %d, expected: %d", postG->outpostPlayed, G->outpostPlayed);
	printPassFail(postG->outpostPlayed, G->outpostPlayed);
	printf("Outpost turn: %d, expected: %d", postG->outpostTurn, G->outpostTurn);
	printPassFail(postG->outpostTurn, G->outpostTurn);
	// Hand, deck, and discard counts should remain the same for other players
	int numPlayers = G->numPlayers;
	for (i = 0; i < numPlayers; i++) {
		if (i != currPlayer) {
			printf("Player %d hand count: %d, expected: %d", i, postG->handCount[i], G->handCount[i]);
			printPassFail(postG->handCount[i], G->handCount[i]);
			printf("Player %d deck count: %d, expected: %d", i, postG->deckCount[i], G->deckCount[i]);
			printPassFail(postG->deckCount[i], G->deckCount[i]);
			printf("Player %d discard count: %d, expected: %d", i, postG->discardCount[i], G->discardCount[i]);
			printPassFail(postG->discardCount[i], G->discardCount[i]);
		}
	}
}

int main(int argc, char *argv[]) {
	int numPlayers = 2;
	int seedNum = 2000;
	int result;
	gState G, postG;

	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};
	printf("Initializing game.\n");
	initializeGame(numPlayers, k, seedNum, &G);
	int currPlayer = G.whoseTurn;

	// Add smithy card to player's hand
	G.hand[currPlayer][G.handCount[currPlayer]] = smithy;
	G.handCount[currPlayer]++;
	memcpy(&postG, &G, sizeof(gState));
	result = playCard(G.handCount[currPlayer]-1, 0, 0, 0, &postG);
	testSmithy(&G, &postG, currPlayer);

	return 0;


}
