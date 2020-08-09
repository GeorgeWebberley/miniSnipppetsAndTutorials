/* A program that simulates the word ladders game. Given a starting word and
end word from the command line it gives the edit distance and the words in
the sequence */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define WORDLENGTH 4

int edit_distance(char*s, char*t);
void test(void);
void recursive(char *start, char *end, FILE *file);
void fileManager(char *start, char *end);

int main(int argc, char **argv)
{
  unsigned length1, length2;
  test();
  if(argc != 3){
    fprintf(stderr, "Please enter %s followed by two valid 4-letter words\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  length1 = strlen(argv[1]);
  length2 = strlen(argv[2]);
  if(length1 != WORDLENGTH || length2 != WORDLENGTH){
    fprintf(stderr, "Please ensure both words are 4 letters long.\n");
    exit(EXIT_FAILURE);
  }
  fileManager(argv[1], argv[2]);
  return 0;
}


void fileManager(char *start, char *end)
{
  FILE *word_file;

  if((word_file = fopen("34words.txt", "r")) == NULL){
    fprintf(stderr, "Something went wrong.\n");
    exit(EXIT_FAILURE);
  }
  printf("%s\n", start);
  recursive(start, end, word_file);
  fclose(word_file);
}


int edit_distance(char*s, char*t)
{
  int count = 0, i;
  for(i = 0; i < WORDLENGTH; i++){
    if(s[i] != t[i]){
      count++;
    }
  }
  return count;
}





void recursive(char *start, char *end, FILE *file)
{
  char candidate[WORDLENGTH], parent[WORDLENGTH];
  char children[WORDLENGTH][WORDLENGTH];
  int distance, i=0;
  parent = start;
  if(distance == 0){
    return;
  }
  rewind(file);
  while(fscanf(file, "%s", candidate) != EOF){
    if(edit_distance(candidate, start) == 1){
      children[i] = candidate;
      i++;
    }
  }
}




void test(void)
{
  assert(edit_distance("wild", "tame") == 4);
  assert(edit_distance("wild", "wall") == 2);
  assert(edit_distance("wild", "wild") == 0);
  assert(edit_distance("poke", "pole") == 1);
}
