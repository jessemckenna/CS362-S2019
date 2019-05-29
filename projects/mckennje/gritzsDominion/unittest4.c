// Name: unittest4.c
// Description: Positive test for method smithyCardEffect() in dominion.c.
// Author: Jesse McKenna
// Date: 5/5/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <stdio.h>

#define TESTFUNCTION "smithyCardEffect"
#define TESTCARD smithy

void TestThreeCardsAdded(int* didTestPass);

int main() {
  PrintFunctionTestHeader(TESTFUNCTION);
  int didTestPass = TRUE;

  // Run positive tests.
  TestThreeCardsAdded(&didTestPass);

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void TestThreeCardsAdded(int* didTestPass) {
  printf("TestThreeCardsAdded\n");

  struct gameState initialState = GetInitialState();
  struct gameState state = initialState;
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);

  // Set expectations for card counts after call to card function.
  const int cardsAdded = 3;
  const int cardsPlayed = 1;

  // Run dominion code, saving results in |state|.
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  // Check expectations against actual results.
  const int currentPlayer = whoseTurn(&initialState);
  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] +
                  cardsAdded -
                  cardsPlayed,
               didTestPass);
  ExpectEquals("Deck count",
               state.deckCount[currentPlayer],
               initialState.deckCount[currentPlayer] - cardsAdded,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);
  ExpectOtherPlayersUnchanged(
      state, initialState, currentPlayer, didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, didTestPass);
}