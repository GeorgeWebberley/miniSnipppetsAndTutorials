/* A program that converts a stream of text typed by a user into a secret code.
The letter 'a' is converted into 'z', 'b' into 'y' etc. */

#include <stdio.h>
#include <ctype.h>

int scode(int a);

int main(void)
{
  int c, letter;
  printf("Please enter in text to be converted:\n");
  while ((c = getchar()) != EOF){
    letter = scode(c);
    printf("%c", letter);
  }
  return 0;
}

int scode(int a)
{
  int letters[26];
  int i;
  /* Creates an array with numbers ranging from 0-25 to represent the alphabet */
  for (i = 0; i < 26; i ++){
    letters[i] = i;
  }
  if ('a' <= a && 'z' >= a){ /* Checks if the character is lower case letter. */
    a = a - 'a';             /* converts lower case alphabet to numbers from 0 to 25. */
    a = letters[25 - a];     /* swaps the letter with it's opposite-end counterpart. */
    a = a + 'a';             /* Returns the number to it's original ASCII code. */
    return a;
  }
  if ('A' <= a && 'Z' >= a){ /* Checks if the character is upper case letter. */
    a = a - 'A';             /* converts upper case alphabet to numbers from 0 to 25. */
    a = letters[25 - a];     /* swaps the letter with it's opposite-end counterpart. */
    a = a + 'A';             /* Returns the number to it's original ASCII code. */
    return a;
  }

  return a;
}
