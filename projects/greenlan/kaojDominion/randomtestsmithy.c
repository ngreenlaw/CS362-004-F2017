//	Performs random testing on the smithy card in dominion.c
//
//	Effect of smithy: 
//		-player gains 3 cards

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

// Sets random player, hand count, deck count, discard count
// and fills them with random cards
void setRandomGState(gState *G) {
	int currP, i, j, k;

	currP = floor(Random() * G->numPlayers);	// Assign random current player
	G->whoseTurn = currP; 					
	G->handCount[currP] = floor(Random() * MAX_HAND);	// Set random hand count
	G->deckCount[currP] = floor(Random() * MAX_DECK);	// Set random deck count
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


int testSmithy(gState *G, gState *postG, int *errorCount) {
	int errored = 0;		// Function prints only first encountered error
	int currP = G->whoseTurn;
	int errMaxMet = 0;		// Set if maximum number of errors to print has been reached
	if ((*errorCount) >= MAXERROR)	errMaxMet = 1;
	int expDeckCount, i;
	
	// Handcount should have increased by two - gain three cards, lose played smithy
	if (postG->handCount[currP] != G->handCount[currP] + 2) {
		if (!errored && !errMaxMet) {
			printError(G, "Handcount", postG->handCount[currP], G->handCount[currP]+2);
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

	
	// Top of played cards should be smithy
	if (postG->playedCardCount > 0) {
		if (postG->playedCards[postG->playedCardCount-1] != smithy) {
			if (!errored && !errMaxMet) {
				printError(G, "Top of played cards", postG->playedCards[postG->playedCardCount-1], smithy);
			}
			errored = 1;
		}
	}
	
	// Deck count should have decreased
	// If deck count >= 3, should decrease by 3
	if (G->deckCount[currP] <= 3) {
		expDeckCount = G->deckCount[currP] - 3;
	} else {  // Else, deck count = previous count + discardCount - 3
		expDeckCount = G->deckCount[currP] + G->discardCount[currP] - 3;
	}

	if (postG->deckCount[currP] != expDeckCount) {
		if (!errored && !errMaxMet) {
			printError(G, "Deck count", postG->deckCount[currP], G->deckCount[currP] - 3);
		}
		errored = 1;
	}

	char playerHand[32];
	char playerDeck[32];
	char playerDiscard[32];

	// Hand, deck, and discard counts for other players should remain the same
	for (i = 0; i < postG->numPlayers; i++) {
		if (i != currP) {
			memset(playerHand, 0, 32);
			memset(playerDeck, 0, 32);
			memset(playerDiscard, 0, 32);
			sprintf(playerHand, "Player %d handcount", i);
			sprintf(playerDeck, "Player %d deckcount", i);
			sprintf(playerDiscard, "Player %d discardcount", i);
			if (postG->handCount[i] != G->handCount[i]) {
				if (!errored && !errMaxMet) {
					printError(G, playerHand, postG->handCount[i], G->handCount[i]);
				}
				errored = 1;
			}
			if (postG->deckCount[i] != G->deckCount[i]) {
				if (!errored && !errMaxMet) {
					printError(G, playerDeck, postG->deckCount[i], G->deckCount[i]);
				}
				errored = 1;
			}
			if (postG->discardCount[i] != G->discardCount[i]) {
				if (!errored && !errMaxMet) {
					printError(G, playerDiscard, postG->discardCount[i], G->discardCount[i]);
				}
				errored = 1;
			}
		}
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
	printf("Random testing for the smithy card.\n\n");

	for (i = 0; i < 500; i++) {
		seedNum = rand();
		memset(&G, 0, sizeof(gState));			// Clear game state
		memset(&postG, 0, sizeof(gState));
		numPlayers = floor(Random()*3) + 2;		// Set random number of players 2-4
		initializeGame(numPlayers, k, seedNum, &G);		// Initialize game
		setRandomGState(&G);						// Set random game state
		currP = G.whoseTurn;
		G.hand[currP][G.handCount[currP]] = smithy;		// Add smithy to player's hand
		G.handCount[currP]++;		// Increment player's hand count
		memcpy(&postG, &G, sizeof(gState));		// Copy game state
		playCard(G.handCount[currP]-1, 0, 0, 0, &postG);	// Play smithy card
		if (testSmithy(&G, &postG, &errorCount) == 1) {
			errorCount++;
		}
	}
	if (errorCount == 0) {
		printf("All tests passed\n");
	}

	printf("Testing finished.\n");

	return 0;
}

