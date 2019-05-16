// Name: randomtest_helpers.h
// Description: Helper functions to be used in randomtest*.c.
// Author: Jesse McKenna
// Date: 5/19/2019

#ifndef _RANDOMTEST_HELPERS_H
#define _RANDOMTEST_HELPERS_H

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

// --- Randomization ---
int GetRandomBetween(int min, int max);
void RandomizeHandDeckAndDiscard(struct gameState* state, int player);
void RandomizePlayedCards(struct gameState* state);

// --- Test setup ---
struct gameInputs {
  int choice1;
  int choice2;
  int choice3;
  int handPos;
  int bonus;
};

struct gameState GetRandomInitialState();
struct gameInputs InitializeGameInputs(struct gameState* state, int card);
int GetValueFromTreasure(int treasureCard);
int IsTreasure(int card);

// --- Test evaluation ---
void ExpectTrue(
    char* statementDescription, int assertedStatement, int* didTestPass);
void ExpectEquals(
    char* valueDescription, int value, int expectedValue, int* didTestPass);
void ExpectGreaterThanOrEqualTo(char* valueDescription,
                                int value,
                                int expectedLowerValue,
                                int* didTestPass);
void ExpectLessThanOrEqualTo(char* valueDescription,
                             int value,
                             int expectedHigherValue,
                             int* didTestPass);
void ExpectCoinsUnchanged(struct gameState state,
                          struct gameState initialState,
                          int* didTestPass);
void ExpectOtherPlayersUnchanged(struct gameState state,
                                 struct gameState initialState,
                                 int currentPlayer,
                                 int* didTestPass);
void ExpectVictoryPileUnchanged(
    struct gameState state, struct gameState initialState, int* didTestPass);
void ExpectTreasurePileUnchanged(
    struct gameState state, struct gameState initialState, int* didTestPass);
void ExpectKingdomPileUnchanged(
    struct gameState state, struct gameState initialState, int* didTestPass);
void ExpectPlayedPileUnchanged(
    struct gameState state, struct gameState initialState, int* didTestPass);

// --- Output messages ---
void PrintRandomTestHeader(int numTestRuns, char* testCard);
void PrintSuccess();
void PrintTestInformation(struct gameState state);

#endif