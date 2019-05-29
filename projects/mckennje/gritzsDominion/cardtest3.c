// Name: cardtest3.c
// Description: 
// Author: Jesse McKenna
// Date: 5/5/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <stdio.h>

#define TESTCARDNAME "Smithy"
#define TESTCARD smithy

void TestThreeCardsAddedWhenDeckIsEmpty(int* didTestPass);

int main() {
  PrintCardTestHeader(TESTCARDNAME);
  int didTestPass = TRUE;

  // Run boundary tests.
  TestThreeCardsAddedWhenDeckIsEmpty(&didTestPass);

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void TestThreeCardsAddedWhenDeckIsEmpty(int* didTestPass) {
  printf("TestThreeCardsAddedWhenDeckIsEmpty\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);

  // Empty player's deck into the discard pile.
  const int currentPlayer = whoseTurn(&initialState);
  while (initialState.deckCount[currentPlayer] > 0) {
    initialState.discard[currentPlayer]
        [initialState.discardCount[currentPlayer] - 1] = 
        initialState.deck[currentPlayer]
            [initialState.deckCount[currentPlayer] - 1];

    initialState.deckCount[currentPlayer]--;
    initialState.discardCount[currentPlayer]++;
  }
  struct gameState state = initialState;

  // Set expectations for card counts after call to cardEffect().
  const int cardsAdded = 3;
  const int cardsPlayed = 1;
  const int cardsShuffled = initialState.discardCount[currentPlayer];

  // Run dominion code, saving results in |state|.
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
               initialState.handCount[currentPlayer] +
                  cardsAdded -
                  cardsPlayed,
               didTestPass);
  ExpectEquals("Deck count",
               state.deckCount[currentPlayer],
               initialState.deckCount[currentPlayer] +
                  cardsShuffled -
                  cardsAdded,
               didTestPass);
  ExpectEquals("Discard count",
               state.discardCount[currentPlayer],
               initialState.discardCount[currentPlayer] - cardsShuffled,
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