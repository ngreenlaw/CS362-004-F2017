/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
  This file is based off of the update coins file given in the course materials
 */

  /*Test the function whoseTurn*/
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

// set TEST_FUNCTION to whatever thing is being tested
#define TEST_FUNCTION "whoseTurn"

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
	
	// Begin the testing
    printf ("TESTING %s\n", TEST_FUNCTION);
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	G.whoseTurn = 0;
	
	int turn;
	
	// TEST 1
	printf("Test player 0 turn\n"); //Test being run
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	turn = whoseTurn(&testG);
    assertTrue(turn == 0); //Test result
	
	// TEST 2
	G.whoseTurn = 1;
	printf("Test player 1 turn\n"); //Test being run
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	turn = whoseTurn(&testG);
    assertTrue(turn == 1); //Test result

	//End the testing
    printf("End Testing for %s\n",TEST_FUNCTION);

    return 0;
}