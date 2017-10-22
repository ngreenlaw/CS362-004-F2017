/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
  This file is based off of the update coins file given in the course materials
 */

 /*Test the function updateCoins*/
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

// set TEST_FUNCTION to whatever thing is being tested
#define TEST_FUNCTION "updateCoins"

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
	//Initialize the hands of the players
	G.coins = 0;
	G.handCount[0] = maxHandCount;
	G.handCount[1] = maxHandCount;
	G.hand[0][0] = copper;
	G.hand[0][1] = province;
	G.hand[0][2] = province;
	G.hand[0][3] = province;
	G.hand[0][4] = province;
	G.hand[1][0] = silver;
	G.hand[1][1] = province;
	G.hand[1][2] = province;
	G.hand[1][3] = province;
	G.hand[1][4] = province;
	
	// TEST 1
	printf("Test no bonus coins and one copper\n"); //Test being run
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	updateCoins(0,&testG, 0);
	/* Do the test material here where b is boolean to see if test succeeded*/
	printf("Test that coins is 1\n");
	assertTrue(testG.coins == 1);
	
	// TEST 2
	printf("Test no bonus coins and one silver\n"); //Test being run
	G.coins = 0;
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	updateCoins(1,&testG, 0);
	/* Do the test material here where b is boolean to see if test succeeded*/
	printf("Test that coins is 2\n");
	assertTrue(testG.coins == 2);
	
	// TEST 3
	printf("Test no bonus coins and one gold\n"); //Test being run
	G.coins = 0;
	G.hand[0][0] = gold;
	G.hand[0][1] = province;
	G.hand[0][2] = province;
	G.hand[0][3] = province;
	G.hand[0][4] = province;
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	updateCoins(0,&testG, 0);
	/* Do the test material here where b is boolean to see if test succeeded*/
	printf("Test that coins is 3\n");
	assertTrue(testG.coins == 3);

	// TEST 4
	printf("Test 2 bonus coins and one silver\n"); //Test being run
	G.coins = 0;
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	updateCoins(1,&testG, 2);
	/* Do the test material here where b is boolean to see if test succeeded*/
	printf("Test that coins is 4\n");
	assertTrue(testG.coins == 4);
	
	//End the testing
    printf("End Testing for %s\n",TEST_FUNCTION);

    return 0;
}
