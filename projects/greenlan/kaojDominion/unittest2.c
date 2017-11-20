//
//		Usage:
//			fullDeckCount(int player, int card, struct gameState *state)
//
//

#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASS_STR "     ----- PASS ----- "
#define FAIL_STR "     ----- FAIL ----- "
#define CHARLENGTH 256

typedef struct gameState gState;

void testCardCount(int currPlayer, int card, gState *G, int expectedCount, char cardName[]) {
	int cardCount = fullDeckCount(currPlayer, card, G); // Get card count
	memset(cardName, '\0', CHARLENGTH);	// Clear out char array
	cardNumToName(card, cardName);	// From interface.c
	printf("%s count: %d, expected: %d", cardName, cardCount, expectedCount);
	if (cardCount == expectedCount) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	int numPlayers = 2;
	int seedNum = 500;
	int card = copper;
	char cardName[CHARLENGTH];	

	struct gameState G;

	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};

	printf("Initializing game.\n");
	initializeGame(numPlayers, k, seedNum, &G);
	int currPlayer = G.whoseTurn;

	printf("TEST 1 of fullDeckCount() --- numPlayers: 2, card: copper, status: new game\n");
	testCardCount(currPlayer, card, &G, 7, cardName);

	card = estate;
	printf("TEST 2 of fullDeckCount() --- numPlayers: 2, card: estate, status: new game\n");	
	testCardCount(currPlayer, card, &G, 3, cardName);

	card = gold;
	printf("TEST 3 of fullDeckCount() --- numPlayers: 2, card: gold, status: new game\n");
	testCardCount(currPlayer, card, &G, 0, cardName);
	
	// Add a card to player's hand
	G.hand[currPlayer][G.handCount[currPlayer]] = gold;
	G.handCount[currPlayer]++;
	printf("TEST 4 of fullDeckCount() --- numPlayers: 2, card: gold, status: added gold to player's hand\n");
	testCardCount(currPlayer, card, &G, 1, cardName);

	// Add a card to player's deck
	G.deck[currPlayer][G.deckCount[currPlayer]] = estate;
	G.deckCount[currPlayer]++;
	card = estate;
	printf("TEST 5 of fullDeckCount() --- numPlayers: 2, card: estate, status: added estate to player's deck\n");
	testCardCount(currPlayer, card, &G, 4, cardName);

	// Add a card to player's discard
	G.discard[currPlayer][G.discardCount[currPlayer]] = copper;
	G.discardCount[currPlayer]++;
	card = copper;
	printf("TEST 6 of fullDeckCount() --- numPlayers: 2, card: copper, status: added copper to player's discard\n");
	testCardCount(currPlayer, card, &G, 8, cardName);

	return 0;
}

