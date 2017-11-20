//	Performs random testing on the adventurer card in dominion.c
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
#include <time.h>

#define MAXCARD treasure_map
#define MAXERROR 20				// Maximum number of errors to print

typedef struct gameState gState;


// Outputs testing parameter which produced the error and the original gameState
// prior to playing the card
void printError(gState *G, char *countStr, int actual, int expected) {
	printf("%s: %d, expected: %d     FAILED\n", countStr, actual, expected);
	int currP = G->whoseTurn;
	printf("Game state --- numPlayers: %d, handCount: %d, deckCount: %d, discardCount: %d, playedCardCount: %d\n",
		G->numPlayers, G->handCount[currP], G->deckCount[currP],
		G->discardCount[currP], G->playedCardCount);
	printf("\n");
}

// Gets the card count for a particular card from an input
// stack (player's hand, deck, or discard)
int getCardCount(int stack[], int stackCount, int card) {
	int i, count = 0;
	for (i = 0; i < stackCount; i++) {
		if (stack[i] == card) {
			count++;
		}
	}
	return count;
}

// Sets random player, hand count, deck count, discard count
// and fills them with random cards
void setRandomGState(gState *G) {
	int currP, i, j, k;

	currP = floor(Random() * G->numPlayers);	// Assign random current player
	G->whoseTurn = currP; 					
	G->handCount[currP] = floor(Random() * MAX_HAND);	// Set random hand count
	G->deckCount[currP] = 10 + floor(Random() * (MAX_DECK-10));	// Set random deck count, have at least 10 cards
	G->discardCount[currP] = floor(Random() * MAX_DECK);	// Set random discard count

	for (i = 0; i < G->handCount[currP]; i++) {			// Fill hand with random cards
		G->hand[currP][i] = floor(Random() * MAXCARD);
	}
	for (j = 0; j < G->deckCount[currP]; j++) {			// Fill deck with random cards
		G->deck[currP][i] = floor(Random() * MAXCARD);
	}
	for (k = 0; k < G->discardCount[currP]; k++) {		// Fill discard with random cards
		G->discard[currP][k] = floor(Random() * MAXCARD);
	}
}


int testAdventurer(gState *G, gState *postG, int *errorCount) {
	int treasureCards[2];
	int discarded[MAX_DECK] = { -1 };
	int card, i, discardCount = 0, drawCount = 0, treasureCount = 0;
	int errored = 0;		// Function prints only first encountered error
	int currP = G->whoseTurn;
	int errMaxMet = 0;		// Set if maximum number of errors to print has been reached
	if ((*errorCount) >= MAXERROR)	errMaxMet = 1;

	// Determine which cards are drawn from the deck
	while (treasureCount < 2) {
		card = G->deck[currP][G->deckCount[currP]-1-drawCount];
		if (card == copper || card == silver || card == gold) {
			treasureCards[treasureCount++] = card;
		} else {
			discarded[discardCount++] = card;
		}
		drawCount++;
	}
	
	// Handcount should have increased by one - gain two treasure cards, lose played adventurer card
	if (postG->handCount[currP] != G->handCount[currP] + 1) {
		if (!errored && !errMaxMet) {
			printError(G, "Handcount", postG->handCount[currP], G->handCount[currP]+1);
		}
		errored = 1;
	}


	// Played card count should have increased by one
	if (postG->playedCardCount != G->playedCardCount + 1) {
		if (!errored && !errMaxMet) {
			printError(G, "Played card count", postG->playedCardCount, G->playedCardCount + 1);
		}
		errored = 1;
	}

	
	// Top of played cards should be adventurer
	if (postG->playedCardCount > 0) {
		if (postG->playedCards[postG->playedCardCount-1] != adventurer) {
			if (!errored && !errMaxMet) {
				printError(G, "Top of played cards", postG->playedCards[postG->playedCardCount-1], adventurer);
			}
			errored = 1;
		}
	}
	

	// Hand should have the two treasures drawn from the deck
	int initCard1Count = getCardCount(G->hand[currP], G->handCount[currP], treasureCards[0]);
	int postCard1Count = getCardCount(postG->hand[currP], postG->handCount[currP], treasureCards[0]);
	int initCard2Count = getCardCount(G->hand[currP], G->handCount[currP], treasureCards[1]);
	int postCard2Count = getCardCount(postG->hand[currP], postG->handCount[currP], treasureCards[1]);
	
	if (treasureCards[0] != treasureCards[1]) {		// Treasure cards are different
		if (postCard1Count != initCard1Count + 1) {
			if (!errored && !errMaxMet) {
				printError(G, "Treasure card 1 count", postCard1Count, initCard1Count + 1);
			}
			errored = 1;
		}
		if (postCard2Count != initCard2Count + 1) {
			if (!errored && !errMaxMet) {
				printError(G, "Treasure card 2 count", postCard2Count, initCard2Count + 1);
			}
			errored = 1;
		}
	} else if (treasureCards[0] == treasureCards[1]) {		// Treasure cards are the same
		if (postCard1Count != initCard1Count + 2) {
			if (!errored && !errMaxMet) {
				printError(G, "Treasure card count", postCard1Count, initCard1Count + 2);
			}
			errored = 1;
		}
	}

	// Deck count should have decreased by drawCount
	if (postG->deckCount[currP] != G->deckCount[currP] - drawCount) {
		if (!errored && !errMaxMet) {
			printError(G, "Deck count", postG->deckCount[currP], G->deckCount[currP] - drawCount);
		}
		errored = 1;
	}

	// Discard pile count should have increased by discardCount
	if (postG->discardCount[currP] != G->discardCount[currP] + discardCount) {
		if (!errored && !errMaxMet) {
			printError(G, "Discard count", postG->discardCount[currP], G->discardCount[currP] + discardCount);
		}
		errored = 1;
	}


	// Check for changes in game state
	if (postG->numPlayers != G->numPlayers) {
		if (!errored && !errMaxMet) {
			printError(G, "Number of players", postG->numPlayers, G->numPlayers);
		}
		errored = 1;
	}
	if (postG->whoseTurn != G->whoseTurn) {
		if (!errored && !errMaxMet) {
			printError(G, "Whose turn", postG->whoseTurn, G->whoseTurn);
		}
		errored = 1;
	}
	if (postG->phase != G->phase) {
		if (!errored && !errMaxMet) {
			printError(G, "Phase", postG->phase, G->phase);
		}
		errored = 1;
	}
	if (postG->numBuys != G->numBuys) {
		if (!errored && !errMaxMet) {
			printError(G, "Number of buys", postG->numBuys, G->numBuys);
		}
		errored = 1;
	}

	// Check for changes in supply count
	i = 0;
	
	while (i < MAXCARD+1) {
		if (postG->supplyCount[i] != G->supplyCount[i]) {
			if (!errored && !errMaxMet) {
				printError(G, "Supply count does not match", postG->supplyCount[i], G->supplyCount[i]);
			}
			errored = 1;
		}
		i++;
	}
	
	if (errored) {
		return 1;
	} else {
		return 0;
	}

}


int main(int argc, char *argv[]) {
	srand(time(NULL));

	int numPlayers, i, currP;
	int seedNum;
	int errorCount = 0;
	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};
	gState G, postG;
	printf("Random testing for the adventurer card.\n\n");

	for (i = 0; i < 500; i++) {
		seedNum = rand();
		memset(&G, 0, sizeof(gState));			// Clear game state
		memset(&postG, 0, sizeof(gState));
		numPlayers = floor(Random()*3) + 2;		// Set random number of players 2-4
		initializeGame(numPlayers, k, seedNum, &G);		// Initialize game
		setRandomGState(&G);						// Set random game state
		currP = G.whoseTurn;
		G.hand[currP][G.handCount[currP]] = adventurer;		// Add adventurer to player's hand
		G.handCount[currP]++;		// Increment player's hand count
		memcpy(&postG, &G, sizeof(gState));		// Copy game state
		playCard(G.handCount[currP]-1, 0, 0, 0, &postG);	// Play adventurer card
		if (testAdventurer(&G, &postG, &errorCount) == 1) {
			errorCount++;
		}
	}
	if (errorCount == 0) {
		printf("All tests passed\n");
	}

	printf("Testing finished.\n");

	return 0;
}

