// Name: cardtest4.c
// Description: Positive tests for Adventurer card implementation in dominion.c.
// Author: Jesse McKenna
// Date: 5/5/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <stdio.h>

#define TESTCARDNAME "Adventurer"
#define TESTCARD adventurer

void FindTwoTreasures(int* didTestPass);
void FindOneTreasure(int* didTestPass);
void FindNoTreasures(int* didTestPass);

int main() {
  PrintCardTestHeader(TESTCARDNAME);
  int didTestPass = TRUE;

  // Run positive tests.
  FindTwoTreasures(&didTestPass);
  FindOneTreasure(&didTestPass);
  FindNoTreasures(&didTestPass);

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void FindTwoTreasures(int* didTestPass) {
  printf("FindTwoTreasures\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  struct gameState state = initialState;

  // Set expectations for card counts after call to card function.
  const int cardsAdded = 2;
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
               initialState.handCount[currentPlayer] + cardsAdded - cardsPlayed,
               didTestPass);
  const int cardsDiscarded = initialState.deckCount[currentPlayer] -
                             state.deckCount[currentPlayer] -
                             cardsAdded;
  ExpectEquals("Discard count",
               state.discardCount[currentPlayer],
               initialState.discardCount[currentPlayer] + cardsDiscarded,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
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

void FindOneTreasure(int* didTestPass) {
  printf("FindOneTreasure\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  struct gameState state = initialState;

  // Remove all Treasure cards from deck except one.
  const int currentPlayer = whoseTurn(&initialState);
  const int deckCount = state.deckCount[currentPlayer];
  int oneTreasureFound = FALSE;
  for (int i = 0; i < deckCount; i++) {
    if (IsTreasure(state.deck[currentPlayer][i])) {
      if (!oneTreasureFound) {
        // Skip the first Treasure found in order to leave one in the deck.
        oneTreasureFound = TRUE;
        continue;
      }
      state.deck[currentPlayer][i] = -1;
    }
  }

  // Set expectations for card counts after call to card function.
  const int cardsAdded = 1;
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
  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] + cardsAdded - cardsPlayed,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);

  // Entire deck should have been moved to discard pile.
  ExpectEquals("Discard count",
               state.discardCount[currentPlayer],
               initialState.deckCount[currentPlayer] - cardsAdded,
               didTestPass);

  ExpectOtherPlayersUnchanged(
      state, initialState, currentPlayer, didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, didTestPass);

  // Confirm that added card is a Treasure card.
  const int newHandCount = state.handCount[currentPlayer];
  const int cardAdded = state.hand[currentPlayer][newHandCount - 1];
  ExpectTrue("Gained card is a Treasure",
             IsTreasure(cardAdded),
             didTestPass);

  // Confirm that new coin count reflects addition of new Treasure card.
  const int coinsAdded = GetValueFromTreasure(cardAdded);
  ExpectEquals(
      "Coins", state.coins, initialState.coins + coinsAdded, didTestPass);
}

void FindNoTreasures(int* didTestPass) {
  printf("FindNoTreasures\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  struct gameState state = initialState;

  // Remove all Treasure cards from deck.
  const int currentPlayer = whoseTurn(&initialState);
  const int deckCount = state.deckCount[currentPlayer];
  for (int i = 0; i < deckCount; i++) {
    if (IsTreasure(state.deck[currentPlayer][i])) {
      state.deck[currentPlayer][i] = -1;
    }
  }

  // Set expectations for card counts after call to card function.
  const int cardsAdded = 0;
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
  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] + cardsAdded - cardsPlayed,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);

  // Entire deck should have been moved to discard pile.
  ExpectEquals("Discard count",
               state.discardCount[currentPlayer],
               initialState.deckCount[currentPlayer],
               didTestPass);

  ExpectOtherPlayersUnchanged(
      state, initialState, currentPlayer, didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, didTestPass);
  ExpectCoinsUnchanged(state, initialState, didTestPass);
}