/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
  This file is based off of the update coins file given in the course materials
 */

 /*Test the function isGameOver*/
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

// set TEST_FUNCTION to whatever thing is being tested
#define TEST_FUNCTION "isGameOver"

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
	
	// TEST 1
	printf("Testing: Game is Not Over\n"); //Test being run
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	/* Do the test material here where b is boolean to see if test succeeded*/
	int result = isGameOver(&testG); // run the function
	
	printf("Province count unchanged?\n");
	assertTrue(testG.supplyCount[province] == G.supplyCount[province]); // test the province count
	
	printf("Province count > 0?\n");
	assertTrue(testG.supplyCount[province] > 0); // Test greater than 0
	
	printf("No more than 2 piles > 0?\n");
	int j = 0;
	int i;
	for (i = 0; i < 25; i++)
	  {
		if (testG.supplyCount[i] == 0)
		  {
			j++;
		  }
	  }
	assertTrue( j < 3);
	
	printf("Returned 0 as result?\n");
    assertTrue(result == 0);

	// TEST 2
	printf("Testing: Game is Over\n"); //Test being run
	G.supplyCount[province] = 0;
	G.supplyCount[0] = 0;
	G.supplyCount[1] = 0;
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	/* Do the test material here where b is boolean to see if test succeeded*/
	result = isGameOver(&testG); // run the function
	
	printf("Province count unchanged?\n");
	assertTrue(testG.supplyCount[province] == G.supplyCount[province]); // test the province count
	
	printf("Province count == 0?\n");
	assertTrue(testG.supplyCount[province] == 0); // Test greater than 0
	
	printf("At least 3 piles > 0?\n");
	j = 0;
	for (i = 0; i < 25; i++)
	  {
		if (testG.supplyCount[i] == 0)
		  {
			j++;
		  }
	  }
	assertTrue( j >= 3);
	
	printf("Returned 1 as result?\n");
    assertTrue(result == 1);
	
	//End the testing
    printf("End Testing for %s\n",TEST_FUNCTION);

    return 0;
}