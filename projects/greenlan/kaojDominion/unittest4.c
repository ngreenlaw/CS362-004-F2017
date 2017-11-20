// Unit test for isGameOver() function in dominion.c
//
// 		Usage:
//			int isGameOver(struct gameState *state);
//
//

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASS_STR "     ----- PASS ----- "
#define FAIL_STR "     ----- FAIL ----- "

typedef struct gameState gState;

int main(int argc, char *argv[]) {
	int numPlayers = 2;
	int seedNum = 1000;
	int gameOver;
	gState G;
	int k[10] = {adventurer, council_room, gardens, mine, smithy, village, baron,
	great_hall, minion, salvager};

	printf("Initializing game.\n");
	initializeGame(numPlayers, k, seedNum, &G);

	printf("TEST 1 of isGameOver() --- status: new game\n");
	gameOver = isGameOver(&G);
	printf("isGameOver: %d, expected: %d", gameOver, 0);
	if (gameOver == 0) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}
	printf("\n");

	// Empty the Province pile
	G.supplyCount[province] = 0;
	printf("TEST 2 of isGameOver() --- status: province pile empty\n");
	gameOver = isGameOver(&G);
	printf("isGameOver: %d, expected: %d", gameOver, 1);
	if (gameOver == 1) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}
	printf("\n");

	G.supplyCount[province] = 8;	// Restore province count
	// Empty the silver and gold piles
	G.supplyCount[silver] = 0;
	G.supplyCount[gold] = 0;
	printf("TEST 3 of isGameOver() --- status: silver and gold piles empty\n");
	gameOver = isGameOver(&G);
	printf("isGameOver: %d, expected: %d", gameOver, 0);
	if (gameOver == 0) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}
	printf("\n");

	// Empty the Duchy pile
	G.supplyCount[duchy] = 0;
	printf("TEST 4 of isGameOver() --- status: silver, gold, and duchy piles empty\n");
	gameOver = isGameOver(&G);
	printf("isGameOver: %d, expected: %d", gameOver, 1);
	if (gameOver == 1) {
		printf("%s\n", PASS_STR);
	} else {
		printf("%s\n", FAIL_STR);
	}
	printf("\n");

	return 0;
}
