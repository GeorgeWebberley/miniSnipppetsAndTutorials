/* A program that returns all words that are anagrams of eachother */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORDLENGTH 30
#define WORDS 370120

int checkAnagram(char *word, char *candidate);
void sortLetters(char *word);
void printArray(char **dictionary);



int main(void)
{
  FILE *valid_words;
  char dictionary[WORDS][WORDLENGTH];
  unsigned length;
  int i = 0;
  char word;
  valid_words = fopen("eng_370k_shuffle.txt", "r");
  if(valid_words == NULL){
    fprintf(stderr, "Something went wrong.");
    exit(EXIT_FAILURE);
  }
  while(fscanf(valid_words, "%s", word) != EOF){
    dictionary[i][0] = word;
    i++;
  }
  printArray(dictionary);
  return 0;
}


int checkAnagram(char *word, char *candidate)
{
  unsigned i;
  char copy_word[WORDLENGTH], copy_can[WORDLENGTH];
  strcpy(copy_can, candidate);
  strcpy(copy_word, word);
  sortLetters(copy_can);
  sortLetters(copy_word);
  for(i = 0; i < length; i++){
    if(word[i] != copy[i]){
      return 0;
    }
  }
  return 1;
}


/* Sorts the letters of the word into alphabetical order */
void sortLetters(char *word)
{
  unsigned i, j, length;
  char temp;
  length = strlen(word);
  for(i = 0; i < length; i++){
    for(j = i+1; j < length; j++){
      if(word[i] > word[j]){
        temp = word[i];
        word[i] = word[j];
        word[j] = temp;
      }
    }
  }
}

void printArray(char **dictionary)
{
  int i;
  for(i = 0; i<WORDS; i++){
    fprintf(stdout, "%s\n", dictionary[i]);
  }
}
