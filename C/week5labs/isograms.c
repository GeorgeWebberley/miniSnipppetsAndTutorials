#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXLENGTH 20

int isogram(char *s);
void test(void);
void checkLongest(FILE *fp);


int main(int argc, char **argv)
{
  FILE* fp;
  test();
  if(argc != 2){
    fprintf(stderr, "Incorrect use. Type %s followed by <file>.txt.\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  fp = fopen(argv[1], "r");
  if(fp == NULL){
    fprintf(stderr, "Something went wrong with opening the file %s.\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  checkLongest(fp);
  fclose(fp);
  return 0;
}


void checkLongest(FILE *fp)
{
  char word[MAXLENGTH], longest[MAXLENGTH];
  unsigned l = 0, ui;
  while(fscanf(fp, "%s", word) != EOF){
    if(isogram(word)){
      if(strlen(word) > l){
        l = strlen(word);
        for(ui = 0; ui < l; ui++){
          longest[ui] = word[ui];
        }
      }
    }
  }
  printf("%s\n", longest);
}



int isogram(char *s)
{
  int alphabet[26];
  int i;
  char c;
  unsigned length, ui;

  /* Populate alphabet array with '0' */

  for(i = 0; i < 26; i++){
    alphabet[i] = 0;
  }

  /* Add to alphabet index for each letter */
  length = strlen(s);
  for(ui = 0; ui < length; ui++){
    c = s[ui];
    alphabet[c - 'a']++;
    if(alphabet[c - 'a'] == 2){
      return 0;
    }
  }
  return 1;
}


void test(void)
{

  assert(isogram("programming") == 0);
  assert(isogram("housewarmings") == 0);
  assert(isogram("abductions") == 1);
  assert(isogram("housewarming") == 1);
  assert(isogram("hydromagnetics") == 1);
  assert(isogram("uncopyrightable") == 1);


}
