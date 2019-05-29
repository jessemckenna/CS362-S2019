// Name: cardtest2.c
// Description: 
// Author: Jesse McKenna
// Date: 5/5/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <stdio.h>

#define TESTCARDNAME "Village"
#define TESTCARD village

void TestDrawCardGainTwoActions(int* didTestPass);

int main() {
  PrintCardTestHeader(TESTCARDNAME);
  int didTestPass = TRUE;

  // Run positive tests.
  TestDrawCardGainTwoActions(&didTestPass);

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void TestDrawCardGainTwoActions(int* didTestPass) {
  printf("TestDrawCardGainTwoActions\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  struct gameState state = initialState;

  // Set expectations for card counts after call to cardEffect().
  const int cardsAdded = 1;
  const int cardsPlayed = 1;
  const int actionsAdded = 2;

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
               initialState.handCount[currentPlayer] + cardsAdded - cardsPlayed,
               didTestPass);
  ExpectEquals("Deck count",
               state.deckCount[currentPlayer],
               initialState.deckCount[currentPlayer] - cardsAdded,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);
  ExpectEquals("Number of actions",
               state.numActions,
               initialState.numActions + actionsAdded,
               didTestPass);
  ExpectDiscardUnchanged(state, initialState, currentPlayer, didTestPass);
  ExpectOtherPlayersUnchanged(
      state, initialState, currentPlayer, didTestPass);
  ExpectCoinsUnchanged(state, initialState, didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, didTestPass);
}