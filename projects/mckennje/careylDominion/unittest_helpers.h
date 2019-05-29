// Name: unitttest_helpers.h
// Description: Helper functions to be used in unittest1-4.c and cardtest1-4.c.
// Author: Jesse McKenna
// Date: 5/5/2019

#ifndef _UNITTEST_HELPERS_H
#define _UNITTEST_HELPERS_H

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

// --- Test setup ---
struct gameInputs {
  int choice1;
  int choice2;
  int choice3;
  int handPos;
  int bonus;
};

struct gameState GetInitialState();
struct gameInputs InitializeGameInputs(struct gameState state, int card);
void EmptyHandToDiscard(struct gameState* state, int player);
void EmptyDeckToDiscard(struct gameState* state, int player);
int GetValueFromTreasure(int treasureCard);
int IsTreasure(int card);

// --- Test evaluation ---
void ExpectTrue(
    char* statementDescription, int assertedStatement, int* didTestPass);
void AssertTrue(char* statementDescription, int assertedStatement);
void ExpectEquals(
    char* valueDescription, int value, int expectedValue, int* didTestPass);
void AssertEquals(char* valueDescription, int value, int expectedValue);
void ExpectHandCountUnchanged(struct gameState state,
                              struct gameState initialState,
                              int currentPlayer,
                              int* didTestPass);
void ExpectDeckCountUnchanged(struct gameState state,
                              struct gameState initialState,
                              int currentPlayer,
                              int* didTestPass);
void ExpectDeckUnchanged(struct gameState state,
                         struct gameState initialState,
                         int currentPlayer,
                         int* didTestPass);
void ExpectDiscardUnchanged(struct gameState state,
                            struct gameState initialState,
                            int currentPlayer,
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
void PrintCardTestHeader(char* testCard);
void PrintFunctionTestHeader(char* testFunction);
void PrintSuccess();

#endif