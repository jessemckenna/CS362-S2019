// Name: randomtestadventurer.c
// Description: Random-test generator for Adventurer card implementation in
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

#define TESTCARDNAME "Adventurer"
#define TESTCARD adventurer
#define NUM_TEST_RUNS 100

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

  // Set expectations for card counts after call to cardEffect().
  const int cardsPlayed = 1;

  // Count number of Treasure cards in deck, up to 2, to determine expectations
  // for |cardsAdded|, |cardsDiscarded|, and |coinsAdded|.
  int cardsAdded = 0;
  int cardsDiscarded = 0;
  int coinsAdded = 0;
  int card = 0;
  for (int i = initialState.deckCount[currentPlayer] - 1;
          i >= 0 && cardsAdded < 2;
          i--) {
    card = initialState.deck[currentPlayer][i];
    if (IsTreasure(card)) {
      cardsAdded++;
      coinsAdded += GetValueFromTreasure(card);
    }
    else {
      cardsDiscarded++;
    }
  }

  // If less than 2 Treasure cards were found in deck, expect discard pile to be
  // shuffled into deck.
  const int cardsShuffled =
      cardsAdded < 2 ? initialState.discardCount[currentPlayer] : 0;
  if (cardsShuffled) {
    for (int i = initialState.discardCount[currentPlayer] - 1;
            i >= 0 && cardsAdded < 2;
            i--) {
      card = initialState.discard[currentPlayer][i];
      if (IsTreasure(card)) {
        cardsAdded++;

        // If cards were shuffled, can't accurately predict which Treasure cards
        // will be gained; in this case, increment |coinsAdded| by 1 and use it
        // as a minimum (i.e. at least 1 coin should have been gained).
        coinsAdded++;
      }
      // If cards were shuffled, can't predict how many cards will be discarded,
      // so |cardsDiscarded| as measured up to this point will also become a
      // minimum.
    }
  }

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
  ExpectOtherPlayersUnchanged(state, initialState, currentPlayer, &didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, &didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, &didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, &didTestPass);

  if (cardsShuffled) {
    // If discard pile was shuffled into deck, |cardsDiscarded| and |coinsAdded|
    // are minimums, not exact values.
    ExpectLessThanOrEqualTo(
        "Deck count",
        state.deckCount[currentPlayer],
        initialState.deckCount[currentPlayer] +
            cardsShuffled -
            cardsAdded -
            cardsDiscarded,
        &didTestPass);
    ExpectGreaterThanOrEqualTo(
        "Discard count",
        state.discardCount[currentPlayer],
        initialState.discardCount[currentPlayer] +
            cardsDiscarded -
            cardsShuffled,
        &didTestPass);
    ExpectGreaterThanOrEqualTo(
        "Coins", state.coins, initialState.coins + coinsAdded, &didTestPass);
  }
  else {
    // If no shuffle took place, exact expected values are known.
    ExpectEquals(
        "Deck count",
        state.deckCount[currentPlayer],
        initialState.deckCount[currentPlayer] - cardsAdded - cardsDiscarded,
        &didTestPass);
    ExpectEquals(
        "Discard count",
        state.discardCount[currentPlayer],
        initialState.discardCount[currentPlayer] + cardsDiscarded,
        &didTestPass);
    ExpectEquals(
        "Coins", state.coins, initialState.coins + coinsAdded, &didTestPass);
  }

  if (!didTestPass) {
    PrintTestInformation(initialState);
  }
  return didTestPass;
}