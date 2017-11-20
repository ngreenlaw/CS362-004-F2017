CFLAGS = -Wall -fpic -coverage -lm

default: unittestresults.out

randomtestcard1.out: randomtestadventurer.c dominion.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
	randomtestadventurer > randomtestcard1.out
	gcov -b -f dominion.c >> randomtestcard1.out

randomtestcard2.out: randomtestcouncil_room.c dominion.o rngs.o
	gcc -o randomtestcouncil_room randomtestcouncil_room.c dominion.o rngs.o $(CFLAGS)
	randomtestcouncil_room > randomtestcard2.out
	gcov -b -f dominion.c >> randomtestcard2.out

randomtestcard3.out: randomtestsmithy.c dominion.o rngs.o
	gcc -o randomtestsmithy randomtestsmithy.c dominion.o rngs.o $(CFLAGS)
	randomtestsmithy > randomtestcard3.out
	gcov -b -f dominion.c >> randomtestcard3.out

unittestresults.out: unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c dominion.o rngs.o interface.o
	echo "Result of units tests on functions and cards in dominion.c" > unittestresults.out
	
	echo "unittest1.c:" >> unittestresults.out
	gcc -o unittest1 unittest1.c dominion.o rngs.o $(CFLAGS)
	unittest1 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out

	echo "unittest2.c:" >> unittestresults.out
	gcc -o unittest2 unittest2.c dominion.o rngs.o interface.o $(CFLAGS)
	unittest2 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	
	echo "unittest3.c:" >> unittestresults.out
	gcc -o unittest3 unittest3.c dominion.o rngs.o $(CFLAGS)
	unittest3 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	
	echo "unittest4.c:" >> unittestresults.out
	gcc -o unittest4 unittest4.c dominion.o rngs.o $(CFLAGS)
	unittest4 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out

	echo "cardtest1.c:" >> unittestresults.out
	gcc -o cardtest1 cardtest1.c dominion.o rngs.o $(CFLAGS)
	cardtest1 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out

	echo "cardtest2.c:" >> unittestresults.out
	gcc -o cardtest2 cardtest2.c dominion.o rngs.o $(CFLAGS)
	cardtest2 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out

	echo "cardtest3.c:" >> unittestresults.out
	gcc -o cardtest3 cardtest3.c dominion.o rngs.o $(CFLAGS)
	cardtest3 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out

	echo "cardtest4.c:" >> unittestresults.out
	gcc -o cardtest4 cardtest4.c dominion.o rngs.o $(CFLAGS)
	cardtest4 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittestresults.out
	rm -f unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 randomtestadventurer randomtestcouncil_room randomtestsmithy
