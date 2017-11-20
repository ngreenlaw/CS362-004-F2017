// Tests the council_room card in dominion.c
//
// 		Effect of council room: 
// 				-player draws four additional cards and gets one additional buy
// 				-each other player draws a card
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

void testCouncilRoom(gState *G, gState *postG, int currPlayer, int numPlayers) {
	int initCountCurrP = G->handCount[currPlayer];	// Initial hand count
	int postCountCurrP = postG->handCount[currPlayer];	// Hand count after playing council room
	
	// Hand count should have increased by three: gain four cards, remove played council room from hand
	printf("Current player hand count: %d, expected: %d", postCountCurrP, initCountCurrP + 3);
	printPassFail(postCountCurrP, initCountCurrP + 3);	
	int i, cardsMatch = 1;
	int handDist[MAXCARD] = { 0 };
	int expectedHandDist[MAXCARD] = { 0 };
	
	// Check if the cards in hand match the added cards from deck
	printf("Current hand: ");
	for (i = 0; i < postG->handCount[currPlayer]; i++) {
		printf("%d ", postG->hand[currPlayer][i]);
		handDist[postG->hand[currPlayer][i]]++;
	}
	printf(", expected hand: ");
	// Cards previous in hand, excluding council room card
	for (i = 0; i < G->handCount[currPlayer]-1; i++) {
		printf("%d ", G->hand[currPlayer][i]);
		expectedHandDist[G->hand[currPlayer][i]]++;
	}
	// Cards previously on top of deck
	for (i = 0; i < 4; i++) {
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

	// Check if number of buys increased by one
	printf("Number of buys: %d, expected: %d", postG->numBuys, G->numBuys + 1);
	printPassFail(postG->numBuys, G->numBuys + 1);

	// Check if other players drew a card
	for (i = 0; i < numPlayers; i++) {
		if (i != currPlayer) {
			printf("Player %d hand count: %d, expected: %d", i, postG->handCount[i], G->handCount[i]+1);
			printPassFail(postG->handCount[i], G->handCount[i]+1);
		}
	}
	printf("\n");

	printf("Checking for changes in gameState --- Error may be from dependent functions such as playCard, updateCoins, drawCard, etc\n");

	// Check for changes in gameState
	printf("Number of players: %d, expected: %d", postG->numPlayers, G->numPlayers);
	printPassFail(postG->numPlayers, G->numPlayers);
	printf("Whose turn: %d, expected: %d", postG->whoseTurn, G->whoseTurn);
	printPassFail(postG->whoseTurn, G->whoseTurn);
	printf("Phase: %d, expected: %d", postG->phase, G->phase);
	printPassFail(postG->phase, G->phase);
	
	// Check for changes in supply count
	i = 0;
	int supplyMatch = 1;
	while (supplyMatch && i < MAXCARD+1) {
		if (postG->supplyCount[i] != G->supplyCount[i]) {
			supplyMatch = 0;
		}
		i++;
	}
	printf("Supply count remains the same");
	printPassFail(supplyMatch, 1);

}

int main(int argc, char *argv[]) {
	int numPlayers = 2;
	int seedNum = 1000;
	int result;
	gState G, postG;

	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};
	printf("Initializing game.\n");
	initializeGame(numPlayers, k, seedNum, &G);
	int currPlayer = G.whoseTurn;

	// Test 1 - two players

	// Add council room card to player's hand
	G.hand[currPlayer][G.handCount[currPlayer]] = council_room;
	G.handCount[currPlayer]++;

	// Copy game state
	memcpy(&postG, &G, sizeof(gState));
	// Play council room card
	result = playCard(postG.handCount[currPlayer]-1, 0, 0, 0, &postG);
	printf("TEST 1 of council room card --- Two players, new game\n");
	testCouncilRoom(&G, &postG, currPlayer, numPlayers);

	printf("\n\n");
	// Test 2 - four players
	numPlayers = 4;
	printf("Initializing new game with four players.\n");
	memset(&G, 0, sizeof(gState));
	memset(&postG, 0, sizeof(gState));
	initializeGame(numPlayers, k, seedNum, &G);
	G.hand[currPlayer][G.handCount[currPlayer]] = council_room;
	G.handCount[currPlayer]++;
	memcpy(&postG, &G, sizeof(gState));
	result = playCard(postG.handCount[currPlayer]-1, 0, 0, 0, &postG);
	printf("TEST 2 of council room card --- Four players, new game\n");
	testCouncilRoom(&G, &postG, currPlayer, numPlayers);
	return 0;

}
