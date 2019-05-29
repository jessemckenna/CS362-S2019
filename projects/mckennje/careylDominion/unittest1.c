// Name: unittest1.c
// Description: Positive test for method feastCardEffect() in dominion.c.
// Author: Jesse McKenna
// Date: 5/5/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <stdio.h>

#define TESTFUNCTION "feastCardEffect"
#define TESTCARD feast

void TestGainCardWorthMaxFiveGold(int* didTestPass);

int main() {
  PrintFunctionTestHeader(TESTFUNCTION);
  int didTestPass = TRUE;

  // Run positive tests.
  TestGainCardWorthMaxFiveGold(&didTestPass);

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void TestGainCardWorthMaxFiveGold(int* didTestPass) {
  printf("TestGainCardWorthMaxFiveGold\n");

  struct gameState initialState = GetInitialState();
  struct gameState state = initialState;
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  
  // Set expectations for card counts after call to card function.
  const int cardAddedMaxCost = 5;

  // Run dominion code, saving results in |state|.
  feastCardEffect(inputs.choice1, &state);
  
  // Check expectations against actual results.
  const int currentPlayer = whoseTurn(&initialState);
  ExpectHandCountUnchanged(state, initialState, currentPlayer, didTestPass);
  ExpectDeckCountUnchanged(state, initialState, currentPlayer, didTestPass);
  ExpectCoinsUnchanged(state, initialState, didTestPass);
  ExpectOtherPlayersUnchanged(state, initialState, currentPlayer, didTestPass);

  // Played pile should be unchanged, because Feast card is trashed when played
  ExpectPlayedPileUnchanged(state, initialState, didTestPass);

  // New card should have cost of 5 or lower.
  const int gainedCardCost = getCost(
      state.hand[currentPlayer][state.handCount[currentPlayer] - 1]);
  ExpectTrue(
      "Gained card cost <= 5", gainedCardCost <= cardAddedMaxCost, didTestPass);
  
  // Number of held Feast cards should have decreased by 1.
  int numFeastCardsBeforePlay = 0;
  int numFeastCardsAfterPlay = 0;
  for (int i = 0; i < state.handCount[currentPlayer]; i++) {
    if (initialState.hand[currentPlayer][i] == feast) {
      numFeastCardsBeforePlay++;
    }
    if (state.hand[currentPlayer][i] == feast) {
      numFeastCardsAfterPlay++;
    }
  }
  
  ExpectTrue("Number of Feast cards decreased by 1",
             numFeastCardsAfterPlay = numFeastCardsBeforePlay - 1,
             didTestPass);
}