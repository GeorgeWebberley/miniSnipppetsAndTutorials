/* A program that outputs all combinations of consonant-vowel-consonant
or vowel-consonant-vowel where the first and third letters are the same
and the sum of the letters (with a=1, b=2 etc.) must be prime.
This represents all possible names on planet bob. */

#include <stdio.h>
#define NUM_VOWELS 5

enum bool{false = 0, true = 1};
typedef enum bool bool;

bool is_vowel(int letter);
bool is_prime(int a);
int letter_converter(int a);


int main(void)
{
  int first_letter = 'a';
  int last_letter = 'z';
  int i, j;
  int first_last, middle;
  for(i = first_letter; i <= last_letter; i++){
    for(j = first_letter; j <= last_letter; j++){
      if(is_vowel(i) != is_vowel(j)){
        first_last = letter_converter(i);
        middle = letter_converter(j);
        if (is_prime((2 * first_last) + middle) == 1){
          printf("%c%c%c\n", i, j, i);
        }
      }
    }
  }
  return 0;
}

bool is_vowel(int letter)
{
  int vowel[NUM_VOWELS] = {'a', 'e', 'i', 'o', 'u'};
  int i;
  for (i = 0; i < NUM_VOWELS; i++){
    if(letter == vowel[i]){
      return true;
    }
  }
  return false;
}

/* A function that converts the letters to integers
with a = 1, b = 2, ...z = 26. */
int letter_converter(int a)
{
  a = (a - 'a'); /* This converts to a = 0, b = 1...etc */
  a++;           /* Converts to a = 1, b = 2... etc */
  return a;
}



bool is_prime(int a)
{
  int i;
  if(a == 1 || a == 2){
    return true;
  }
  for (i = 2; i < a; i++){
    if(a % i == 0){
      return false;
    }
  }
  return true;
}
