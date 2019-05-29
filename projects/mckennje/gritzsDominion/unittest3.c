// Name: unittest3.c
// Description: Negative and boundary tests for method adventurerCardEffect()
// in dominion.c.
// Author: Jesse McKenna
// Date: 5/5/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <stdio.h>

#define TESTFUNCTION "adventurerCardEffect"
#define TESTCARD adventurer

void TestDeckEmpty(int* didTestPass);
void TestDeckAndDiscardEmpty(int* didTestPass);

int main() {
  PrintFunctionTestHeader(TESTFUNCTION);
  int didTestPass = TRUE;

  // Run boundary tests.
  TestDeckEmpty(&didTestPass);

  // Run negative tests.
  TestDeckAndDiscardEmpty(&didTestPass);

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void TestDeckEmpty(int* didTestPass) {
  printf("TestDeckEmpty\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);

  // Empty player deck into player discard pile.
  const int currentPlayer = whoseTurn(&initialState);
  EmptyDeckToDiscard(&initialState, currentPlayer);

  struct gameState state = initialState;

  // Set expectations for card counts after call to card function.
  const int cardsAdded = 2;
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
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);
  const int cardsDiscarded = cardsShuffled -
                             state.deckCount[currentPlayer] -
                             cardsAdded;
  ExpectEquals("Discard count",
               state.discardCount[currentPlayer],
               cardsDiscarded,
               didTestPass);
  ExpectOtherPlayersUnchanged(state, initialState, currentPlayer, didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, didTestPass);

  // Confirm that added cards are both Treasure cards.
  const int newHandCount = state.handCount[currentPlayer];
  const int cardAdded1 = state.hand[currentPlayer][newHandCount - 1];
  const int cardAdded2 = state.hand[currentPlayer][newHandCount - 2];
  ExpectTrue("First gained card is a Treasure",
             IsTreasure(cardAdded1),
             didTestPass);
  ExpectTrue("Second gained card is a Treasure",
             IsTreasure(cardAdded2),
             didTestPass);

  // Confirm that new coin count reflects addition of new Treasure cards.
  const int coinsAdded = GetValueFromTreasure(cardAdded1) +
                         GetValueFromTreasure(cardAdded2);
  ExpectEquals(
      "Coins", state.coins, initialState.coins + coinsAdded, didTestPass);
}

void TestDeckAndDiscardEmpty(int* didTestPass) {
  printf("TestDeckAndDiscardEmpty\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);

  // Empty player deck.
  const int currentPlayer = whoseTurn(&initialState);
  for (int i = 0; i < initialState.deckCount[currentPlayer]; i++) {
    initialState.deck[currentPlayer][i] = -1;
  }
  initialState.deckCount[currentPlayer] = 0;

  // Empty player discard pile.
  for (int i = 0; i < initialState.discardCount[currentPlayer]; i++) {
    initialState.discard[currentPlayer][i] = -1;
  }
  initialState.discardCount[currentPlayer] = 0;

  struct gameState state = initialState;

  // Set expectations for card counts after call to card function.
  const int cardsPlayed = 1;

  // Run dominion code, saving results in |state|.
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  // Nothing should have changed, except for the Adventurer card itself being
  // played.
  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] - cardsPlayed,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);
  ExpectDiscardUnchanged(state, initialState, currentPlayer, didTestPass);
  ExpectDeckUnchanged(state, initialState, currentPlayer, didTestPass);
  ExpectOtherPlayersUnchanged(state, initialState, currentPlayer, didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, didTestPass);
  ExpectCoinsUnchanged(state, initialState, didTestPass);
}