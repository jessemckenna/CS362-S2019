CFLAGS= -Wall -fpic -coverage -lm -std=c99
GCOVFLAGS = -fprofile-arcs -ftest-coverage

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g $(CFLAGS) $(GCOVFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)


#########

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

unittest_helpers.o: unittest_helpers.h unittest_helpers.c dominion.o
	gcc -c unittest_helpers.c -g $(CFLAGS)

unittestresults: unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c dominion.o unittest_helpers.o
	echo "Result for running Dominion tests:" > unittestresults.out
	echo "unittest1.c:" >> unittestresults.out
	gcc -o unittest1 unittest1.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./unittest1 >> unittestresults.out
	echo >> unittestresults.out

	echo "unittest2.c:" >> unittestresults.out
	gcc -o unittest2 unittest2.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./unittest2 >> unittestresults.out
	echo >> unittestresults.out

	echo "unittest3.c:" >> unittestresults.out
	gcc -o unittest3 unittest3.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./unittest3 >> unittestresults.out
	echo >> unittestresults.out

	echo "unittest4.c:" >> unittestresults.out
	gcc -o unittest4 unittest4.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./unittest4 >> unittestresults.out
	echo >> unittestresults.out
	
	echo "cardtest1.c:" >> unittestresults.out
	gcc -o cardtest1 cardtest1.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./cardtest1 >> unittestresults.out
	echo >> unittestresults.out

	echo "cardtest2.c:" >> unittestresults.out
	gcc -o cardtest2 cardtest2.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./cardtest2 >> unittestresults.out
	echo >> unittestresults.out

	echo "cardtest3.c:" >> unittestresults.out
	gcc -o cardtest3 cardtest3.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./cardtest3 >> unittestresults.out
	echo >> unittestresults.out

	echo "cardtest4.c:" >> unittestresults.out
	gcc -o cardtest4 cardtest4.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./cardtest4 >> unittestresults.out
	echo >> unittestresults.out

	echo "gcov dominion.c:" >> unittestresults.out
	gcov dominion.c -b -f >> unittestresults.out

randomtest_helpers.o: randomtest_helpers.h randomtest_helpers.c dominion.o
	gcc -c randomtest_helpers.c -g $(CFLAGS)

randomtestresults: randomtestadventurer.c randomtestcard1.c randomtestcard2.c dominion.o rngs.o randomtest_helpers.o
	echo "Result for running Dominion tests:" > randomtestresults.out
	
	echo "randomtestadventurer.c:" >> randomtestresults.out
	gcc -o randomtestadventurer randomtestadventurer.c randomtest_helpers.o dominion.o rngs.o -g -Wall -fpic -coverage -lm -std=c99
	./randomtestadventurer >> randomtestresults.out
	echo >> randomtestresults.out
	
	echo "randomtestcard1.c:" >> randomtestresults.out
	gcc -o randomtestcard1 randomtestcard1.c randomtest_helpers.o dominion.o rngs.o -g -Wall -fpic -coverage -lm -std=c99
	./randomtestcard1 >> randomtestresults.out
	echo >> randomtestresults.out
	
	echo "randomtestcard2.c:" >> randomtestresults.out
	gcc -o randomtestcard2 randomtestcard2.c randomtest_helpers.o dominion.o rngs.o -g -Wall -fpic -coverage -lm -std=c99
	./randomtestcard2 >> randomtestresults.out
	echo >> randomtestresults.out
	
	echo "gcov dominion.c:" >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe