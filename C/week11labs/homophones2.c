#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mvm.h"

#define MAXLEN 30
#define STRINGJUNK "%*[^0-9[]"

void test(void);
char* filter_phone(char* phoneme, int n);
char* filter_word(char* phoneme);

int main(void)
{
  test();






}


/* Returns the last 'n' phonemes of a word as a string pointer.
If the word has too few phonemes then it returns NULL */
char* filter_phone(char* phoneme, int n)
{
  int count = 0;
  char *c;
  unsigned len;
  if(phoneme == NULL){
    return NULL;
  }
  len = strlen(phoneme);
  /* Subtract 1 from len to get the index of the last letter in the word */
  len--;
  while(count != n){
    if(phoneme[len] == ' '){
      count++;
    }
    if(phoneme[len] == '#'){
      count++;
      if(count < n){
        return NULL;
      }
    }
    len--;
  }
  /* Need to add two back onto 'len' to move the pointer back beyond the '#'
     or ' ' to the start of the substring we want. */
  len = len+2;
  c = (char*) calloc(1, strlen(&(phoneme[(int)len])) * sizeof(char));
  strcpy(c, &(phoneme[len]));
  return c;
}





char* filter_word(char* phoneme)
{
  char* word;
  int i = 0;
  if(phoneme == NULL){
    return NULL;
  }
  word = (char*) calloc(MAXLEN, sizeof(char));
  if(word == NULL){
    ON_ERROR("Unable to filter word.");
  }
  while(phoneme[i] != '#'){
    word[i] = phoneme[i];
    i++;
  }
  /* insert null character on the end */
  word[i] = '\0';

  return word;
}


void test(void)
{
  char phoneme1[] = "STANO#S T AA1 N OW0";
  char phoneme2[] = "FURGERSON#F ER1 G ER0 S AH0 N";
  char phoneme3[] = "STEELED#S T IY1 L D";
  char phoneme4[] = "CRENELATE#K R EH1 N AH0 L EY2 T";
  int k;
  char* c;

  /* Testing filter_phone */
  c = filter_phone(phoneme1, 3);
  k = strcmp(c, "AA1 N OW0");
  assert(k == 0);
  free(c);

  c = filter_phone(phoneme2, 2);
  k = strcmp(c, "AH0 N");
  assert(k == 0);
  free(c);

  c = filter_phone(phoneme3, 6);
  assert(c == NULL);

  c = filter_phone(phoneme4, 8);
  k = strcmp(c, "K R EH1 N AH0 L EY2 T");
  assert(k == 0);
  free(c);

  /* Testing filter_word */
  c = filter_word(phoneme1);
  k = strcmp(c, "STANO");
  assert(k == 0);

  c = filter_word(phoneme2);
  k = strcmp(c, "FURGERSON");
  assert(k == 0);

  c = filter_word(phoneme3);
  k = strcmp(c, "STEELED");
  assert(k == 0);

  c = filter_word(phoneme4);
  k = strcmp(c, "CRENELATE");
  assert(k == 0);

}
