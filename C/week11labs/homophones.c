#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "mvm.h"

/* Max length of word */
#define MAXLEN 40
/* Max length of line */
#define MAXLINE 150

void test(void);
char* filter_phone(char* line, int n);
char* filter_word(char* line);
void make_maps(FILE* fp, mvm* map1, mvm* map2, int n);
void print_phonemes(mvm* map1, mvm* map2, char** argv, int argc, int arg1);


int main(int argc, char** argv)
{
  int n;
  FILE* fp;
  mvm* map1;
  mvm* map2;

  test();
  if(argc < 2){
    ON_ERROR("Incorrect usage. Please type './homophones' followed by '-n <number of phonemes> WORD(s)'\n");
  }
  map1 = mvm_init();
  map2 = mvm_init();
  fp = fopen("cmudict.txt", "r");
  if(strcmp(argv[1], "-n") == 0){
    if(sscanf(argv[2], "%d", &n) != 1){
      fclose(fp);
      ON_ERROR("Not able to scan number from command line. Please type one number after '-n' and a space.\n");
    }
    make_maps(fp, map1, map2, n);
    /* If '-n' is included then the first word to read is the 4th command line argument */
    print_phonemes(map1, map2, argv, argc, 3);
  }else{
    n = 3;
    make_maps(fp, map1, map2, n);
    /* If '-n' is not included then the first word to read is the 2nd command line argument */
    print_phonemes(map1, map2, argv, argc, 1);
  }
  fclose(fp);
  mvm_free(&map1);
  mvm_free(&map2);
  return 0;
}

/* Insert all data into 2 multi value maps (by key and by data) */
void make_maps(FILE* fp, mvm* map1, mvm* map2, int n)
{
  char* word;
  char* phoneme;
  char line[MAXLINE];

  while(fgets(line, MAXLINE, fp) != NULL){
    word = filter_word(line);
    phoneme = filter_phone(line, n);
    if(phoneme != NULL){
      /* Line taken from https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input */
      phoneme[strcspn(phoneme, "\r\n")] = 0;
      mvm_insert(map1, word, phoneme);
      mvm_insert(map2, phoneme, word);
      free(phoneme);
    }
    free(word);
  }
}

/* Search multivalue maps and print 'rhyming' words */
void print_phonemes(mvm* map1, mvm* map2, char** argv, int argc, int arg1)
{
  char * p;
  char ** av;
  int i, j, num;
  for(i = arg1; i < argc; i++){
    p = mvm_search(map1, argv[i]);
    av = mvm_multisearch(map2, p, &num);
    if(av == NULL){
      printf("\nNot able to find any rhymes for %s with this many phonemes. Please use capital letters and check the word has enough phonemes.\n", argv[i]);
    }
    if(p != NULL && av != NULL){
      printf("\n%s (%s): ", argv[i], p);
      for(j = 0; j < num; j++){
        printf("%s ", (av[j]));
      }
      printf("\n");
    }
    free(av);
  }
}


/* Returns the last 'n' phonemes of a word as a string pointer. If the word has too few phonemes then it returns NULL */
char* filter_phone(char* line, int n)
{
  int count = 0;
  char *c;
  unsigned len;
  if(line == NULL){
    return NULL;
  }
  len = strlen(line);
  /* Subtract 1 from len to get the index of the last letter in the word */
  len--;
  /* Loop back through the line, adding 1 to the count for each space or the # */
  while(count != n){
    if(line[len] == ' '){
      count++;
    }
    if(line[len] == '#'){
      count++;
      if(count < n){
        return NULL;
      }
    }
    len--;
  }
  /* Need to add two back onto 'len' to move the pointer back beyond the '#' or ' ' to the start of the substring we want. */
  len = len+2;
  /* Malloc size length of line to ensure there is enough space. */
  c = (char*) malloc(strlen(line));
  strcpy(c, &(line[len]));
  return c;
}


/* Filters out the word from the line */
char* filter_word(char* line)
{
  char* word;
  int i = 0;
  if(line == NULL){
    return NULL;
  }
  word = (char*) calloc(MAXLEN, sizeof(char));
  if(word == NULL){
    ON_ERROR("Unable to filter word.");
  }
  while(line[i] != '#'){
    word[i] = line[i];
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
  free(c);

  c = filter_word(phoneme2);
  k = strcmp(c, "FURGERSON");
  assert(k == 0);
  free(c);

  c = filter_word(phoneme3);
  k = strcmp(c, "STEELED");
  assert(k == 0);
  free(c);

  c = filter_word(phoneme4);
  k = strcmp(c, "CRENELATE");
  assert(k == 0);
  free(c);

}
