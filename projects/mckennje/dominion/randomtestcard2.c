// Name: randomtestcard2.c
// Description: TODO
// Author: Jesse McKenna
// Date: 5/19/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define TESTCARDNAME "Smithy"
#define TESTCARD smithy

void TestTemplate(int* didTestPass);

int main() {
  PrintCardTestHeader(TESTCARDNAME);
  int didTestPass = TRUE;

  // Run random tests.

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void TestTemplate(int* didTestPass) {
  printf("TestTemplate\n");

  struct gameState initialState = GetInitialState();
  struct gameState state = initialState;

  // Set expectations for card counts after call to cardEffect().
  const int cardsAdded = 2;
  const int cardsPlayed = 1;
  const int coinsAdded = 0;

  // Run dominion code, saving results in |state|.
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  inputs.choice1 = 1;
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  // Check expectations against actual results.
  const int currentPlayer = whoseTurn(&initialState);
  AssertEquals(
      "Hand count",
      state.handCount[currentPlayer],
      initialState.handCount[currentPlayer] +
          cardsAdded -
          cardsPlayed);
  AssertEquals("Deck count",
               state.deckCount[currentPlayer],
               initialState.deckCount[currentPlayer] - cardsAdded);
  AssertEquals("Coins", state.coins, initialState.coins + coinsAdded);
}