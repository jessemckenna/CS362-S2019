// Name: testme.c
// Description: Random-input generators for function testme(), created for 
//              Random Testing Quiz
// Author: Jesse McKenna
// Date: 5/12/2019

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHAR_CHARACTERS 9
#define NUM_STRING_CHARACTERS 4
#define STRING_LENGTH 5

char CHAR_CHARACTERS[NUM_CHAR_CHARACTERS] = {'[', '(', '{', ' ', 'a', 'x',
                                                   ']', ')', '}'};
char STRING_CHARACTERS[NUM_STRING_CHARACTERS] = {'r', 'e', 's', 't'};

char getRandomChar(char* charArray, int charArrayLength) {
  return charArray[rand() % charArrayLength];
}

char inputChar()
{
  return getRandomChar(CHAR_CHARACTERS, NUM_CHAR_CHARACTERS);
}

char* inputString()
{
  char* randomString = malloc(STRING_LENGTH + 1);
  randomString[STRING_LENGTH] = '\0';
  for (int i = 0; i < STRING_LENGTH; i++) {
    randomString[i] = getRandomChar(STRING_CHARACTERS, NUM_STRING_CHARACTERS);
  }
  return randomString;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }

    free(s); // required to prevent inputString() from leaking memory
  }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
