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
#include <stdlib.h>
#include <time.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

// set TEST_FUNCTION to whatever thing is being tested
#define TEST_FUNCTION "Adventurer"

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
    int deckCount = 0;
    int discardCount = 0;
// randomly generate the game state values 
	
	srand(time(NULL));
	
	// Begin the testing
    printf ("----------------------------TESTING %s---------------------------------------\n", TEST_FUNCTION);
  
int test = 0;
for(test=0; test < 2000; test++)
{
	//Start randomly generating inputs
	seed = rand()%3000;
	handCount = rand()%5;
	choice1 = rand()%2;
	choice2 = rand()%2;
	choice3 = rand()%2;
	player = rand()%2;
	//handpos = rand()%(handCount+1);
	//bonus = rand()%(maxBonus+1);
	deckCount = rand()%21;
	discardCount = rand()%10;

	memset(&G, 23, sizeof(struct gameState));   // clear the game state
   	r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	G.whoseTurn = rand()%2;
	
	//Randomize the hands of the player and set the handpos of the card to be played equal to the test card
	int h = 0;
	for(h = 0; h < handCount; h++)
	{
		G.hand[0][h] = rand()%28;
	}
	G.hand[0][handpos] = adventurer;

	for(h = 0; h < handCount; h++)
	{
		G.hand[1][h] = rand()%28;
	}
	G.hand[1][handpos] = adventurer;

	//Randomize the decks if there is no treasure than there is a seq fault
	for(h = 0; h < deckCount; h++)
	{
		G.deck[0][h] = rand()%28;
	}
 	r = rand()%(deckCount+1);
	G.deck[0][r] = copper;
	p = rand()%(deckCount+1);
	if(p == r)
	{
		if(p != 0)
		{
			p=0;
		}
		else
		{
			p=1;
		}
	}
	G.deck[0][p] = copper;

	for(h = 0; h < deckCount; h++)
	{
		G.deck[1][h] = rand()%28;
	}
	r = rand()%(deckCount+1);
	G.deck[1][r] = copper;
	p = rand()%(deckCount+1);
	if(p == r)
	{
		if(p != 0)
		{
			p=0;
		}
		else
		{
			p=1;
		}
	}
	G.deck[1][p] = copper;

	//Randomize the discard piles
	for(h = 0; h < discardCount; h++)
	{
		G.discard[0][h] = rand()%28;
	}
	
	for(h = 0; h < discardCount; h++)
	{
		G.discard[0][h] = rand()%28;
	}

    	printf ("-------------Seed %d---------------\n", seed);

	printf("%d, %d, %d, %d \n", handCount, deckCount, discardCount,test);
// TEST 1
	printf("Adventurer card played\n"); //Test being run
	memcpy(&testG, &G, sizeof(struct gameState)); //save game state to compare
	cardEffect(adventurer,choice1,choice2,choice3,&testG,handpos,&bonus);
	
	printf("Took 2 cards?\n");
	assertTrue(testG.handCount[player] == (G.handCount[player]+2));
	
	
	printf("Supply unchanged?\n");
	int j = 0;
	int i =0;
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
}	
	//End the testing
    printf("----------------------------End Testing for %s--------------------------------\n",TEST_FUNCTION);

    return 0;
}
