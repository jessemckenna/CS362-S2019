// Name: unitttest_helpers.c
// Description: Helper functions to be used in unittest1-4.c and cardtest1-4.c.
// Author: Jesse McKenna
// Date: 5/5/2019

#include "unittest_helpers.h"

// --- Test setup ---
struct gameState GetInitialState() {
  struct gameState initialState;
  int kingdomCards[10] = {adventurer, embargo, village, feast, mine,
                          treasure_map, sea_hag, tribute, smithy, council_room};
  initializeGame(2, kingdomCards, 1000, &initialState);
  return initialState;
}

struct gameInputs InitializeGameInputs(struct gameState state, int card) {
  struct gameInputs inputs;
  inputs.choice1 = 0;
  inputs.choice2 = 0;
  inputs.choice3 = 0;
  inputs.bonus = 0;

  const int currentPlayer = whoseTurn(&state);
  inputs.handPos = 0;
  state.hand[currentPlayer][inputs.handPos] = card; // ensure |card| is present

  return inputs;
}

void EmptyHandToDiscard(struct gameState* state, int player) {
  const int handCount = state->handCount[player];
  for (int i = 0; i < handCount; i++) {
    state->discard[player][i] = state->hand[player][i];
    state->hand[player][i] = -1;
  }
  state->discardCount[player] = handCount;
  state->handCount[player] = 0;
}

void EmptyDeckToDiscard(struct gameState* state, int player) {
  const int deckCount = state->deckCount[player];
  for (int i = 0; i < deckCount; i++) {
    state->discard[player][i] = state->deck[player][i];
    state->deck[player][i] = -1;
  }
  state->discardCount[player] = deckCount;
  state->deckCount[player] = 0;
}

int GetValueFromTreasure(int treasureCard) {
  switch(treasureCard) {
    case copper:
      return 1;
    case silver:
      return 2;
    case gold:
      return 3;
    default:
      return 0;
  }
}

int IsTreasure(int card) {
  return card == copper || card == silver || card == gold;
}

// --- Test evaluation ---
void ExpectTrue(char* statementDescription,
                int assertedStatement,
                int* didTestPass) {
  if (!assertedStatement) {
    printf("ヽ(°ロ°)ﾉ ASSERT FAILED: %s\n", statementDescription);
    *didTestPass = FALSE;
  }
}

void AssertTrue(char* statementDescription,
                int assertedStatement) {
  int didTestPass = TRUE;
  ExpectTrue(statementDescription, assertedStatement, &didTestPass);
  if (!didTestPass) {
    exit(0);
  }
}

void ExpectEquals(char* valueDescription,
                  int value,
                  int expectedValue,
                  int* didTestPass) {
  if (value != expectedValue) {
    printf("ヽ(°ロ°)ﾉ ASSERT FAILED: %s expected %d, actual %d\n",
           valueDescription, expectedValue, value);
    *didTestPass = FALSE;  }
}

void AssertEquals(char* valueDescription, int value, int expectedValue) {
  int didTestPass = TRUE;
  ExpectEquals(valueDescription, value, expectedValue, &didTestPass);
  if (!didTestPass) {
    exit(0);
  }
}

void ExpectHandCountUnchanged(struct gameState state,
                              struct gameState initialState,
                              int currentPlayer,
                              int* didTestPass) {
  ExpectEquals("Hand count",
               state.handCount[currentPlayer],
               initialState.handCount[currentPlayer],
               didTestPass);
}

void ExpectDeckCountUnchanged(struct gameState state,
                              struct gameState initialState,
                              int currentPlayer,
                              int* didTestPass) {
  ExpectEquals("Deck count",
               state.deckCount[currentPlayer],
               initialState.deckCount[currentPlayer],
               didTestPass);
}

void ExpectDeckUnchanged(struct gameState state,
                         struct gameState initialState,
                         int currentPlayer,
                         int* didTestPass) {
  int isDeckCountUnchanged = TRUE;
  ExpectDeckCountUnchanged(
      state, initialState, currentPlayer, &isDeckCountUnchanged);
  if (!isDeckCountUnchanged) {
    *didTestPass = FALSE;
    return;
  }

  const int deckCount = state.deckCount[currentPlayer];
  for (int i = 0; i < deckCount; i++) {
    ExpectEquals("Card in deck",
                 state.deck[currentPlayer][i],
                 initialState.deck[currentPlayer][i],
                 didTestPass);
  }
}

void ExpectDiscardCountUnchanged(struct gameState state,
                                 struct gameState initialState,
                                 int currentPlayer,
                                 int* didTestPass) {
  ExpectEquals("Discard count",
               state.discardCount[currentPlayer],
               initialState.discardCount[currentPlayer],
               didTestPass);
}

void ExpectDiscardUnchanged(struct gameState state,
                            struct gameState initialState,
                            int currentPlayer,
                            int* didTestPass) {
  int isDiscardCountUnchanged = TRUE;
  ExpectDiscardCountUnchanged(
      state, initialState, currentPlayer, &isDiscardCountUnchanged);
  if (!isDiscardCountUnchanged) {
    *didTestPass = FALSE;
    return;
  }

  const int discardCount = state.discardCount[currentPlayer];
  for (int i = 0; i < discardCount; i++) {
    ExpectEquals("Card in discard",
                 state.discard[currentPlayer][i],
                 initialState.discard[currentPlayer][i],
                 didTestPass);
  }
}

void ExpectCoinsUnchanged(struct gameState state,
                          struct gameState initialState,
                          int* didTestPass) {
  ExpectEquals("Coins",
               state.coins,
               initialState.coins,
               didTestPass);
}

void ExpectOtherPlayersUnchanged(struct gameState state,
                                 struct gameState initialState,
                                 int currentPlayer,
                                 int* didTestPass) {
  // Number of players should be unchanged.
  ExpectEquals("Number of players",
               state.numPlayers,
               initialState.numPlayers,
               didTestPass);
  if (!didTestPass) {
    return;
  }

  int numPlayers = state.numPlayers;
  for (int i = 0; i < numPlayers; i++) {
    if (i != currentPlayer) {
      // Other players' hand, deck, and coin counts should be unchanged.
      ExpectEquals("Other player's hand count",
                   state.handCount[i],
                   initialState.handCount[i],
                   didTestPass);
      ExpectEquals("Other player's deck count",
                   state.deckCount[i],
                   initialState.deckCount[i],
                   didTestPass);

      // Other players' hands should be unchanged.
      if (didTestPass) {
        int playerHandCount = state.handCount[i];
        for (int j = 0; j < playerHandCount; j++) {
          ExpectEquals("Card in hand",
                       state.hand[i][j],
                       initialState.hand[i][j],
                       didTestPass);
        }
      }
    }
  }
}

void ExpectVictoryPileUnchanged(struct gameState state,
                                struct gameState initialState,
                                int* didTestPass) {
  ExpectEquals("Victory card supply: estate",
               state.supplyCount[estate],
               initialState.supplyCount[estate],
               didTestPass);
  ExpectEquals("Victory card supply: duchy",
               state.supplyCount[duchy],
               initialState.supplyCount[duchy],
               didTestPass);
  ExpectEquals("Victory card supply: province",
               state.supplyCount[province],
               initialState.supplyCount[province],
               didTestPass);
}

void ExpectTreasurePileUnchanged(struct gameState state,
                                 struct gameState initialState,
                                 int* didTestPass) {
  ExpectEquals("Treasure card supply: copper",
               state.supplyCount[copper],
               initialState.supplyCount[copper],
               didTestPass);
  ExpectEquals("Treasure card supply: silver",
               state.supplyCount[silver],
               initialState.supplyCount[silver],
               didTestPass);
  ExpectEquals("Treasure card supply: gold",
               state.supplyCount[gold],
               initialState.supplyCount[gold],
               didTestPass);
}

void ExpectKingdomPileUnchanged(struct gameState state,
                                struct gameState initialState,
                                int* didTestPass) {
  for (int i = adventurer; i <= treasure_map; i++) {
    ExpectEquals("Kingdom card supply",
                 state.supplyCount[i],
                 initialState.supplyCount[i],
                 didTestPass);
  }
}

void ExpectPlayedPileUnchanged(struct gameState state,
                                struct gameState initialState,
                                int* didTestPass) {
  ExpectEquals("Played cards count",
               state.playedCardCount,
               initialState.playedCardCount,
               didTestPass);
  for (int i = 0; i < state.playedCardCount; i++) {
    ExpectEquals("Played card",
                 state.playedCards[i],
                 initialState.playedCards[i],
                 didTestPass);
  }
}

// --- Output messages ---
void PrintCardTestHeader(char* testCard) {
  printf("Testing card: %s\n", testCard);
}

void PrintFunctionTestHeader(char* testFunction) {
  printf("Testing function: %s\n", testFunction);
}

void PrintSuccess() {
  printf("ヽ(・∀・)ﾉ ALL TESTS PASSED\n");
}