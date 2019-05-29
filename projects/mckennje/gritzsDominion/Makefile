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

randomtest_helpers.o: randomtest_helpers.h randomtest_helpers.c dominion.o
	gcc -c randomtest_helpers.c -g $(CFLAGS)

unittest_helpers.o: unittest_helpers.h unittest_helpers.c dominion.o
	gcc -c unittest_helpers.c -g $(CFLAGS)

alltestresults: cardtest1.c cardtest2.c cardtest3.c cardtest4.c \
								randomtestadventurer.c randomtestcard1.c randomtestcard2.c \
								unittest1.c unittest2.c unittest3.c unittest4.c \
								dominion.o rngs.o randomtest_helpers.o unittest_helpers.o
	echo "Result for running Dominion tests:" > alltestresults.out
	
	echo "cardtest1.c:" >> alltestresults.out
	gcc -o cardtest1 cardtest1.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./cardtest1 >> alltestresults.out
	echo >> alltestresults.out

	echo "cardtest2.c:" >> alltestresults.out
	gcc -o cardtest2 cardtest2.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./cardtest2 >> alltestresults.out
	echo >> alltestresults.out

	echo "cardtest3.c:" >> alltestresults.out
	gcc -o cardtest3 cardtest3.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./cardtest3 >> alltestresults.out
	echo >> alltestresults.out

	echo "cardtest4.c:" >> alltestresults.out
	gcc -o cardtest4 cardtest4.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./cardtest4 >> alltestresults.out
	echo >> alltestresults.out

	echo "randomtestadventurer.c:" >> alltestresults.out
	gcc -o randomtestadventurer randomtestadventurer.c randomtest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./randomtestadventurer >> alltestresults.out
	echo >> alltestresults.out
	
	echo "randomtestcard1.c:" >> alltestresults.out
	gcc -o randomtestcard1 randomtestcard1.c randomtest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./randomtestcard1 >> alltestresults.out
	echo >> alltestresults.out
	
	echo "randomtestcard2.c:" >> alltestresults.out
	gcc -o randomtestcard2 randomtestcard2.c randomtest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./randomtestcard2 >> alltestresults.out
	echo >> alltestresults.out

	echo "unittest1.c:" >> alltestresults.out
	gcc -o unittest1 unittest1.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./unittest1 >> alltestresults.out
	echo >> alltestresults.out

	echo "unittest2.c:" >> alltestresults.out
	gcc -o unittest2 unittest2.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./unittest2 >> alltestresults.out
	echo >> alltestresults.out

	echo "unittest3.c:" >> alltestresults.out
	gcc -o unittest3 unittest3.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./unittest3 >> alltestresults.out
	echo >> alltestresults.out

	echo "unittest4.c:" >> alltestresults.out
	gcc -o unittest4 unittest4.c unittest_helpers.o dominion.o rngs.o -g $(CFLAGS)
	./unittest4 >> alltestresults.out
	echo >> alltestresults.out

	echo "gcov dominion.c:" >> alltestresults.out
	gcov dominion.c -b -f >> alltestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe \
				*.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe \
				cardtest1 cardtest2 cardtest3 cardtest4 \
				randomtestadventurer randomtestcard1 randomtestcard2 \
				unittest1 unittest2 unittest3 unittest4