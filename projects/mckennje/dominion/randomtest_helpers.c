// Name: randomtest_helpers.c
// Description: Helper functions to be used in randomtest*.c.
// Author: Jesse McKenna
// Date: 5/19/2019

#include "dominion_helpers.h"
#include "randomtest_helpers.h"
#include <stdlib.h>

// --- Randomization ---
int GetRandomBetween(int min, int max) {
  // Return a random number in range [min, max].
  return min + (rand() % (max + 1 - min));
}

void FillWithRandomCards(int* array, int size) {
  for (int i = 0; i < size; i++) {
    array[i] = GetRandomBetween(curse, treasure_map);
  }
}

void RandomizeHandDeckAndDiscard(struct gameState* state, int player) {
  // To ensure no array overflow, limit the size of the deck, discard, and hand
  // to |MAX_DECK| / 3 (in case hand is emptied into discard, then discard is
  // shuffled into deck).
  const int maxSize = MAX_DECK / 3;

  // Randomize deck count and contents.
  const int newDeckCount = GetRandomBetween(0, maxSize);
  state->deckCount[player] = newDeckCount;
  FillWithRandomCards(state->deck[player], newDeckCount);

  // Randomize discard count and contents.
  const int newDiscardCount = GetRandomBetween(0, maxSize);
  state->discardCount[player] = newDiscardCount;
  FillWithRandomCards(state->discard[player], newDiscardCount);

  // Randomize hand count and contents.
  const int newHandCount = GetRandomBetween(1, maxSize);
  state->handCount[player] = newHandCount;
  FillWithRandomCards(state->hand[player], newHandCount);
}

void RandomizePlayedCards(struct gameState* state) {
  const int playedCardCount = GetRandomBetween(0, MAX_DECK / 2);
  state->playedCardCount = playedCardCount;
  FillWithRandomCards(state->playedCards, playedCardCount);
}

// --- Test setup ---
struct gameState GetRandomInitialState() {
  struct gameState state;

  // Randomize kingdom cards, ensuring no duplicates.
  int kingdomCards[10];
  int isDuplicate[10] = {};
  int card;
  for (int i = 0; i < 10; i++) {
    do {
      card = GetRandomBetween(adventurer, treasure_map);
    } while (isDuplicate[card]);

    kingdomCards[i] = card;
  }

  // Randomize number of players.
  int numPlayers = GetRandomBetween(2, MAX_PLAYERS);

  // Call initializeGame() to get default starting state.
  int seed = rand(); // random number in range [0, RAND_MAX]
  initializeGame(numPlayers, kingdomCards, seed, &state);

  // Randomize hands, decks, discard piles, played pile, and current player.
  for (int i = 0; i < numPlayers; i++) {
    RandomizeHandDeckAndDiscard(&state, i);
  }
  RandomizePlayedCards(&state);
  const int player = GetRandomBetween(0, numPlayers - 1);
  state.whoseTurn = player;
  updateCoins(player, &state, 0);

  return state;
}

struct gameInputs InitializeGameInputs(struct gameState* state, int card) {
  struct gameInputs inputs;
  inputs.choice1 = 0;
  inputs.choice2 = 0;
  inputs.choice3 = 0;
  inputs.bonus = 0;

  const int currentPlayer = whoseTurn(state);
  inputs.handPos = 0;
  state->hand[currentPlayer][inputs.handPos] = card; // ensure |card| present
  updateCoins(currentPlayer, state, 0); // in case a coin was just overwritten

  return inputs;
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

void ExpectEquals(char* valueDescription,
                  int value,
                  int expectedValue,
                  int* didTestPass) {
  if (value != expectedValue) {
    printf("ヽ(°ロ°)ﾉ ASSERT FAILED: %s expected %d, actual %d\n",
           valueDescription, expectedValue, value);
    *didTestPass = FALSE;
  }
}

void ExpectGreaterThanOrEqualTo(char* valueDescription,
                                int value,
                                int expectedLowerValue,
                                int* didTestPass) {
  if (value < expectedLowerValue) {
    printf("ヽ(°ロ°)ﾉ ASSERT FAILED: %s expected >= %d, actual %d\n",
           valueDescription, expectedLowerValue, value);
    *didTestPass = FALSE;
  }
}

void ExpectLessThanOrEqualTo(char* valueDescription,
                             int value,
                             int expectedHigherValue,
                             int* didTestPass) {
  if (value > expectedHigherValue) {
    printf("ヽ(°ロ°)ﾉ ASSERT FAILED: %s expected <= %d, actual %d\n",
           valueDescription, expectedHigherValue, value);
    *didTestPass = FALSE;
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
void PrintRandomTestHeader(int numTestRuns, char* testCard) {
  printf("Running %d tests of card: %s\n", numTestRuns, testCard);
}

void PrintSuccess() {
  printf("ヽ(・∀・)ﾉ ALL TESTS PASSED\n");
}

void PrintTestInformation(struct gameState state) {
  printf("   numPlayers: %d\n", state.numPlayers);
  for (int i = 0; i < state.numPlayers; i++) {
    printf("      player %d: handCount %d, deckCount %d, discardCount %d\n",
           i, state.handCount[i], state.deckCount[i], state.discardCount[i]);
  }
  printf("   playedCardCount %d\n", state.playedCardCount);
  printf("   current player: %d\n", state.whoseTurn);
  printf("   coins: %d\n", state.coins);
}