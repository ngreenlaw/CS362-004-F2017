/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
  This file is based off of the update coins file given in the course materials
 */

  /*Test the function fullDeckCount*/
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

// set TEST_FUNCTION to whatever thing is being tested
#define TEST_FUNCTION "fullDeckCount"

// Self created assert function for testing functions
void assertTrue(int b)
{
	if(b == 0)
	{
		printf("Test Failed\n");
	}
	else
	{
		printf("Test Successful\n");
	}
}

int main() {
	//Initialize game parameters
	int b;
    int seed = 1000;
    int numPlayer = 2;
    int maxBonus = 10;
    int p, r, handCount;
    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, testG;
    int maxHandCount = 5;
    int player = 0;
	// Begin the testing
    printf ("TESTING %s\n", TEST_FUNCTION);
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	G.deckCount[0] = 1;
	G.handCount[0] = 2;
	G.discardCount[0] = 3;
	G.deck[player][0] = village;
	G.hand[player][0] = village;
	G.hand[player][1] = village;
	G.discard[player][0] = village;
	G.discard[player][1] = village;
	G.discard[player][2] = copper;
	
	int result;
	
	// TEST 1
	printf("Given 6 cards in between hand, deck and discard.\n"); //Test being run
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	result = fullDeckCount(0,village,&testG);
	printf("Check to see that the result is 5 village cards\n");
    assertTrue(result == 5); //Test result

	//End the testing
    printf("End Testing for %s\n",TEST_FUNCTION);

    return 0;
}
