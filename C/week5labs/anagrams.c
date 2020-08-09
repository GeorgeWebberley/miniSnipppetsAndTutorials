/* A program that takes user input from the command line and returns a list
of valid anagrams of that word */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLENGTH 30

void runAnagrams(char *word);
int checkAnagram(char *word, char *candidate, unsigned length);
void sortLetters(char *word, unsigned length);


int main(int argc, char **argv)
{
  if(argc != 2){
    fprintf(stderr, "Please type %s followed by a word you would like to see anagrams of.",
            argv[0]);
    exit(EXIT_FAILURE);
  }else{
    runAnagrams(argv[1]);
  }
  return 0;
}


void runAnagrams(char *word)
{
  FILE *valid_words;
  char candidate[MAXLENGTH];
  unsigned c_length, w_length;
  w_length = strlen(word);
  valid_words = fopen("eng_370k_shuffle.txt", "r");
  if(valid_words == NULL){
    fprintf(stderr, "Something went wrong.");
    exit(EXIT_FAILURE);
  }
  sortLetters(word, w_length);
  while(fscanf(valid_words, "%s", candidate) != EOF){
    c_length = strlen(candidate);
    if(c_length == w_length){
      if(checkAnagram(word, candidate, w_length)){
        fprintf(stdout, "%s\n", candidate);
      }
    }
  }
}

/* Makes a copy of the candidate which is then sorted and compared with the
already sorted input word */
int checkAnagram(char *word, char *candidate, unsigned length)
{
  unsigned i;
  char copy[MAXLENGTH];
  strcpy(copy, candidate);
  sortLetters(copy, length);
  for(i = 0; i < length; i++){
    if(word[i] != copy[i]){
      return 0;
    }
  }
  return 1;
}

/* Sorts the letters of the word into alphabetical order */
void sortLetters(char *word, unsigned length)
{
  unsigned i, j;
  char temp;
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
