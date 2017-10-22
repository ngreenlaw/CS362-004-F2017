/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
  This file is based off of the update coins file given in the course materials
 */

  /*Test the function ______________*/
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

// set TEST_FUNCTION to whatever thing is being tested
#define TEST_FUNCTION "Village"

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
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int maxBonus = 10;
    int p, r, handCount;
	int handpos = 0;
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int bonus = 0;
	int player = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, testG;
    int maxHandCount = 5;
	
	
	// Begin the testing
    printf ("TESTING %s\n", TEST_FUNCTION);
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	G.whoseTurn = 0;
	G.deckCount[0] = 5;
	// TEST 1
	printf("Village card played\n"); //Test being run
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	cardEffect(village,choice1,choice2,choice3,&testG,handpos,&bonus);
	
	printf("Drew cards from correct pile?\n");
	assertTrue(testG.deckCount[player] == (G.deckCount[player]-1));
	
	printf("Drew 1 card?\n");
	assertTrue(testG.handCount[player] == G.handCount[player]);
	
	printf("Actions correct?\n");
	assertTrue(testG.numActions == (G.numActions+2));
	
	printf("Supply unchanged?\n");
	int j = 0;
	for (i = 0; i < 25; i++)
	  {
		if (testG.supplyCount[i] != G.supplyCount[i])
		  {
			j++;
		  }
	  }
	assertTrue( j == 0);

	printf("Other player hand unchanged?\n");
	assertTrue(testG.handCount[1] == G.handCount[1]);

	//End the testing
    printf("End Testing for %s\n",TEST_FUNCTION);

    return 0;
}
