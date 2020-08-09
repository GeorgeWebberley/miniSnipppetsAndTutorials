/* A program that outputs all combinations of consonant, vowel, consonant
which represent all possible names on the Planet Trium */

#include <stdio.h>
#define NUM_VOWELS 5

enum bool{false = 0, true = 1};
typedef enum bool bool;

bool is_vowel(int letter);

int main(void)
{


  int first_letter = 'a';
  int last_letter = 'z';
  int i, j, k;
  for(i = first_letter; i <= last_letter; i++){
    if(is_vowel(i) == false){
      for(j = first_letter; j <= last_letter; j++){
        if(is_vowel(j) == true){
          for(k = first_letter; k <= last_letter; k++){
            if(is_vowel(k) == false){
              printf("%c%c%c\n", i, j, k);
            }
          }
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
