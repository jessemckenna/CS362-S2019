// Name: randomtestadventurer.c
// Description: Random-test generator for Village card implementation in
// dominion.c.
// Author: Jesse McKenna
// Date: 5/19/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "randomtest_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define TESTCARDNAME "Village"
#define TESTCARD village
#define NUM_TEST_RUNS 3
#define MAX_NUM_ACTIONS 20

int TestRandomized();

int main() {
  srand(time(NULL));

  PrintRandomTestHeader(NUM_TEST_RUNS, TESTCARDNAME);
  int didAllTestsPass = TRUE;

  // Run random tests.
  int didTestPass;
  for (int i = 0; i < NUM_TEST_RUNS; i++) {
    didTestPass = TestRandomized();
    didAllTestsPass = didAllTestsPass && didTestPass;
  }

  if (didAllTestsPass) {
    PrintSuccess();
  }
  return 0;
}

int TestRandomized() {
  int didTestPass = TRUE;

  // Randomize game state.
  struct gameState initialState = GetRandomInitialState();
  struct gameInputs inputs = InitializeGameInputs(&initialState, TESTCARD);
  const int currentPlayer = whoseTurn(&initialState);

  // Randomize card-specific variables.
  initialState.numActions = GetRandomBetween(1, MAX_NUM_ACTIONS);

  // Set expectations for card counts after call to cardEffect().
  const int cardsPlayed = 1;
  const int cardsAdded = 1;
  const int cardsShuffled = initialState.deckCount[currentPlayer] == 0 ?
                                initialState.discardCount[currentPlayer] :
                                0;
  const int actionsAdded = 2;

  // Run dominion code, saving results in |state|.
  struct gameState state = initialState;
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  // Check expectations against actual results.
  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] + cardsAdded - cardsPlayed,
               &didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               &didTestPass);
  ExpectEquals(
      "Deck count",
      state.deckCount[currentPlayer],
      initialState.deckCount[currentPlayer] - cardsAdded + cardsShuffled,
      &didTestPass);
  ExpectEquals("Discard count",
               state.discardCount[currentPlayer],
               initialState.discardCount[currentPlayer] - cardsShuffled,
               &didTestPass);
  ExpectEquals("Number of actions",
               state.numActions,
               initialState.numActions + actionsAdded,
               &didTestPass);
  ExpectCoinsUnchanged(state, initialState, &didTestPass);
  ExpectOtherPlayersUnchanged(state, initialState, currentPlayer, &didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, &didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, &didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, &didTestPass);

  if (!didTestPass) {
    PrintTestInformation(initialState);
  }
  return didTestPass;
}