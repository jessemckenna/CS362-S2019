// Name: cardtest1.c
// Description: Combination test for Steward card implementation in dominion.c.
// Author: Jesse McKenna
// Date: 5/5/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define TESTCARDNAME "Steward"
#define TESTCARD steward

void TestDrawTwoCards(int* didTestPass);
void TestGainTwoCoins(int* didTestPass);
void TestTrashTwoCards(int* didTestPass);
void TestTrashTwoCardsWhenHandIsEmpty(int* didTestPass);
void TestTrashOneCard(int* didTestPass);

int main() {
  PrintCardTestHeader(TESTCARDNAME);
  int didTestPass = TRUE;

  // Run positive tests.
  TestDrawTwoCards(&didTestPass);
  TestGainTwoCoins(&didTestPass);
  TestTrashTwoCards(&didTestPass);

  // Run negative tests.
  TestTrashTwoCardsWhenHandIsEmpty(&didTestPass);

  // Run boundary tests.
  TestTrashOneCard(&didTestPass);

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void TestDrawTwoCards(int* didTestPass) {
  printf("TestDrawTwoCards\n");

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

void TestGainTwoCoins(int* didTestPass) {
  printf("TestGainTwoCoins\n");

  struct gameState initialState = GetInitialState();
  struct gameState state = initialState;

  // Set expectations for card counts after call to cardEffect().
  const int cardsAdded = 0;
  const int cardsPlayed = 1;
  const int coinsAdded = 2;

  // Run dominion code, saving results in |state|.
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  inputs.choice1 = 2;
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  // Check expectations against actual results.
  const int currentPlayer = whoseTurn(&initialState);
  AssertEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] +
                  cardsAdded -
                  cardsPlayed);
  AssertEquals("Deck count",
               state.deckCount[currentPlayer],
               initialState.deckCount[currentPlayer] - cardsAdded);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);
  AssertEquals("Coins", state.coins, initialState.coins + coinsAdded);
}

void TestTrashTwoCards(int* didTestPass) {
  printf("TestTrashTwoCards\n");

  struct gameState initialState = GetInitialState();
  const int currentPlayer = whoseTurn(&initialState);
  initialState.hand[currentPlayer][0] = steward;
  initialState.hand[currentPlayer][1] = copper;
  initialState.hand[currentPlayer][2] = duchy;
  initialState.hand[currentPlayer][3] = estate;
  initialState.hand[currentPlayer][4] = feast;
  struct gameState state;

  // Set expectations for card counts after call to cardEffect().
  const int cardsAdded = 0;
  const int cardsPlayed = 1;
  const int cardsTrashed = 2;
  int cardRemoved1, cardRemoved2;

  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  inputs.choice1 = 3;

  // Cycle through each eligible combination of two cards to trash.
  const int initialHandCount =
      initialState.handCount[currentPlayer];
  for (int i = 1; i < initialHandCount; i++) {
    for (int j = i + 1; j < initialHandCount; j++) {
      // Reset |state| on each loop.
      state = initialState;

      inputs.choice2 = j;
      inputs.choice3 = i;
      cardRemoved1 = state.hand[currentPlayer][i];
      cardRemoved2 = state.hand[currentPlayer][j];

      // Run dominion code, saving results in |state|.
      cardEffect(TESTCARD,
                 inputs.choice1,
                 inputs.choice2,
                 inputs.choice3,
                 &state,
                 inputs.handPos,
                 &inputs.bonus);

      // Check that the removed cards are no longer in the player's hand.
      for (int k = 0; k < state.handCount[currentPlayer]; k++) {
        AssertTrue("First card removed not in player's hand",
                   state.hand[currentPlayer][k] != cardRemoved1);
        AssertTrue("Second card removed not in player's hand",
                   state.hand[currentPlayer][k] != cardRemoved2);
      }

      // Check that remaining card counts match expectations.
      ExpectEquals(
          "Hand count",
          state.handCount[currentPlayer],
          initialState.handCount[currentPlayer] +
              cardsAdded -
              cardsPlayed -
              cardsTrashed,
          didTestPass);
      ExpectEquals(
          "Deck count",
          state.deckCount[currentPlayer],
          initialState.deckCount[currentPlayer] - cardsAdded,
          didTestPass);
      ExpectEquals("Played cards count",
                   state.playedCardCount,
                   initialState.playedCardCount + cardsPlayed,
                   didTestPass);
    }
  }
}

void TestTrashTwoCardsWhenHandIsEmpty(int* didTestPass) {
  printf("TestTrashTwoCardsWhenHandIsEmpty\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  inputs.choice1 = 3;

  // Empty player's hand.
  const int currentPlayer = whoseTurn(&initialState);
  EmptyHandToDiscard(&initialState, currentPlayer);

  // Run dominion code, saving results in |state|.
  struct gameState state = initialState;
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  // Check that hand is still empty.
  ExpectEquals("Hand count", state.handCount[currentPlayer], 0, didTestPass);
}

void TestTrashOneCard(int* didTestPass) {
  printf("TestTrashOneCard\n");

  struct gameState initialState = GetInitialState();
  const int currentPlayer = whoseTurn(&initialState);
  initialState.hand[currentPlayer][0] = steward;
  initialState.hand[currentPlayer][1] = copper;
  struct gameState state;

  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  inputs.choice1 = 3;

  // Remove cards until only the steward card and one other card remain.
  while (state.handCount[currentPlayer] > 2) {
    discardCard(0, currentPlayer, &state, 0);
  }

  // Run dominion code, saving results in |state|.
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  // Check that hand is empty (played steward card and one card were discarded).
  ExpectEquals("Hand count", state.handCount[currentPlayer], 0, didTestPass);
}