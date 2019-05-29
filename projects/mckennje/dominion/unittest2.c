// Name: unittest2.c
// Description: Negative tests for method seaHagCardEffect() in dominion.c.
// Author: Jesse McKenna
// Date: 5/5/2019

#include "dominion.h"
#include "dominion_helpers.h"
#include "unittest_helpers.h"
#include <stdio.h>

#define TESTFUNCTION "seaHagCardEffect"
#define TESTCARD sea_hag

void TestNoOtherPlayers(int* didTestPass);
void TestNoCurseCardsLeft(int* didTestPass);
void TestOtherPlayerDeckIsEmpty(int* didTestPass);

int main() {
  PrintFunctionTestHeader(TESTFUNCTION);
  int didTestPass = TRUE;

  // Run negative tests.
  TestNoOtherPlayers(&didTestPass);
  TestNoCurseCardsLeft(&didTestPass);
  TestOtherPlayerDeckIsEmpty(&didTestPass);

  if (didTestPass) {
    PrintSuccess();
  }
  return 0;
}

void TestNoOtherPlayers(int* didTestPass) {
  printf("TestNoOtherPlayers\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  initialState.numPlayers = 1;
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

  // Nothing should have changed, except the Sea Hag card itself was played.
  const int currentPlayer = whoseTurn(&initialState);
  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] - cardsPlayed,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);
  ExpectDeckUnchanged(state, initialState, currentPlayer, didTestPass);
  ExpectCoinsUnchanged(state, initialState, didTestPass);
  ExpectVictoryPileUnchanged(state, initialState, didTestPass);
  ExpectTreasurePileUnchanged(state, initialState, didTestPass);
  ExpectKingdomPileUnchanged(state, initialState, didTestPass);
}

void TestNoCurseCardsLeft(int* didTestPass) {
  printf("TestNoCurseCardsLeft\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);
  initialState.supplyCount[curse] = 0;
  struct gameState state = initialState;

  // Set expectations for card counts after call to card function.
  const int cardsPlayed = 1;
  const int cardsDiscarded = 1;

  // Run dominion code, saving results in |state|.
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  const int currentPlayer = whoseTurn(&initialState);
  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] - cardsPlayed,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);
  ExpectDeckUnchanged(state, initialState, currentPlayer, didTestPass);

  // Each other player should have discarded a card, but not gained a curse.
  for (int i = 0; i < state.numPlayers; i++) {
    if (i != currentPlayer) {
      const int playerDeckCount = state.deckCount[i];
      ExpectEquals(
          "Other player's deck count",
          playerDeckCount,
          initialState.deckCount[i] - cardsDiscarded,
          didTestPass);
      ExpectTrue(
          "Player was not cursed",
          state.deck[playerDeckCount - 1] != curse,
          didTestPass);
    }
  }
  ExpectEquals("Curse card supply", state.supplyCount[curse], 0, didTestPass);
}

void TestOtherPlayerDeckIsEmpty(int* didTestPass) {
  printf("TestOtherPlayerDeckIsEmpty\n");

  struct gameState initialState = GetInitialState();
  struct gameInputs inputs = InitializeGameInputs(initialState, TESTCARD);

  // Set one other player's deckCount to 0.
  const int currentPlayer = whoseTurn(&initialState);
  int playerWithEmptyDeck = -1;
  for (int i = 0; i < initialState.numPlayers; i++) {
    if (i != currentPlayer) {
      playerWithEmptyDeck = i;
      break;
    }
  }
  AssertTrue("Setup: another player exists", playerWithEmptyDeck > 0);
  EmptyDeckToDiscard(&initialState, playerWithEmptyDeck);

  struct gameState state = initialState;

  // Set expectations for card counts after call to card function.
  const int cardsPlayed = 1;
  const int cardsDiscarded = 1;
  // |playerWithEmptyDeck| only:
  const int cardsShuffled = state.discardCount[playerWithEmptyDeck];

  // Run dominion code, saving results in |state|.
  cardEffect(TESTCARD,
             inputs.choice1,
             inputs.choice2,
             inputs.choice3,
             &state,
             inputs.handPos,
             &inputs.bonus);

  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer] - cardsPlayed,
               didTestPass);
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount + cardsPlayed,
               didTestPass);
  ExpectDeckUnchanged(state, initialState, currentPlayer, didTestPass);

  // Each other player should have discarded a card and gained a Curse.
  ExpectDeckUnchanged(state, initialState, currentPlayer, didTestPass);
  for (int i = 0; i < state.numPlayers; i++) {
    if (i != currentPlayer && i != playerWithEmptyDeck) {
      ExpectDeckCountUnchanged(state, initialState, i, didTestPass);
      ExpectEquals("Discard count",
                   state.discardCount[currentPlayer],
                   initialState.discardCount[currentPlayer] + cardsDiscarded,
                   didTestPass);
      ExpectTrue("Top of deck is curse",
                 state.deck[i][state.deckCount[i] - 1] == curse,
                 didTestPass);
    }
  }

  // |playerWithEmptyDeck| should have shuffled discard into deck, then
  // discarded a card and gained a Curse.
  ExpectEquals("Deck count",
               state.deckCount[playerWithEmptyDeck],
               initialState.deckCount[playerWithEmptyDeck] + cardsShuffled,
               didTestPass);
  ExpectEquals("Discard count",
               state.discardCount[playerWithEmptyDeck],
               initialState.discardCount[playerWithEmptyDeck] -
                  cardsShuffled +
                  cardsDiscarded,
               didTestPass);
  ExpectTrue(
      "Top of deck is curse",
      state.deck[playerWithEmptyDeck][state.deckCount[playerWithEmptyDeck] - 1]
          == curse,
      didTestPass);
}