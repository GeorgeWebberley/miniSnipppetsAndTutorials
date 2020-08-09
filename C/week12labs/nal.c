#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/* These hash defines i have copied from Neil in the handout. Will need to change these for the hand in */
#define strsame(A, B) (strcmp(A, B) == 0)
#define PROGNAME "test1.nal"
#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 150
#define ERROR(PHRASE) {fprintf(stderr, "Fatal error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); exit(2);}

struct program{
  char wds[MAXNUMTOKENS][MAXTOKENSIZE];   /*Change these hash defines!? Making assumptions regarding length. */
  int cw;
};
typedef struct program Program;

/* function will return nil if it does not meet criteria of being a varcon */
enum varcon{nil, strvar, numvar, strcon, numcon};
typedef enum varcon varcon;


void test(void);
void print(Program p);
void Prog(Program *p);
void Instruct(Program *p);
void Instrs(Program *p);
void Print(Program *p);
char* rot18(char* string);

void printArray(Program *p);

varcon whatVarcon(char* string);

void file(char *file);

/*
bool isVarcon();
bool isVar();
bool isCon();
bool isStrvar();
bool isNumvar();
bool isStrcon();
bool isNumcon();

*/

/* while(fscanf(fp, "%s", prog.wds[i++]) == 1 && i < MAXNUMTOKENS); */

int main(void)
{
  int i, j;
  int len;
  FILE *fp;
  Program prog;
  char *stream;
  char word[MAXTOKENSIZE];

  prog.cw = 0;
  for(i = 0; i < MAXNUMTOKENS; i++){
    prog.wds[i][0] = '\0';
  }
  if(!(fp = fopen(PROGNAME, "r"))){
    fprintf(stderr, "Cannot open %s\n", PROGNAME);
    exit(2);
  }

  /* Modified version of https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c */
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  /* +1 to account for adding null character */
  stream = malloc(len+1);
  if(stream == NULL){
    fclose(fp);
    ERROR("Failed to create space for file stream.");
  }
  fread(stream, 1, len, fp);
  fclose(fp);
  stream[len] = '\0';


  i = 0;
  while(stream[i] != '\0'){
    j = 0;
    if(stream[i] == '\"'){
      word[j] = stream[i];
      i++;
      j++;
      while(stream[i] != '\"'){
        word[j] = stream[i];
        i++;
        j++;
      }
      word[j] = stream[i];
      i++;
      j++;
      word[j] = '\0';
      strcpy(prog.wds[prog.cw], word);
      prog.cw++;
    }else{
      if(isalnum(stream[i]) || ispunct(stream[i])){
        while(isalnum(stream[i]) || ispunct(stream[i])){
          word[j] = stream[i];
          i++;
          j++;
        }
        word[j] = '\0';
        strcpy(prog.wds[prog.cw], word);
        prog.cw++;
      }
    }
    i++;
  }

  printArray(&prog);


  prog.cw = 0;
  Prog(&prog);
  printf("Parsed Ok\n");

  test();

  return 0;
}


void printArray(Program *p)
{
  int i;
  for(i = 0; i < p->cw; i++){
    printf("%s\n", p->wds[i]);
  }
}


void Prog(Program *p)
{
  if(!(strsame(p->wds[p->cw], "{"))){
    ERROR("No '{' at start of file?");
  }
  p->cw++;
  Instrs(p);
}


void Instrs(Program *p)
{
  if(strsame(p->wds[p->cw], "}")){
    p->cw++;
    return;
  }
  Instruct(p);
  Instrs(p);
}


void Instruct(Program *p)
{
  if(strsame(p->wds[p->cw], "PRINT")){
    p->cw++;
    Print(p);
    return;
  }
  if(strsame(p->wds[p->cw], "ABORT")){
    p->cw++;
    exit(EXIT_SUCCESS);
  }
  if(strsame(p->wds[p->cw], "FILE")){
    p->cw++;
    file(p->wds[p->cw]);
    return;
  }
}


void file(char *file)
{
  int i, j;
  int len;
  FILE *fp;
  Program *prog;
  char *stream;
  char word[MAXTOKENSIZE];

  prog = (Program *)malloc(sizeof(Program));
  if(prog == NULL){
    fprintf(stderr, "Cannot malloc space for program with file %s.", file);
    exit(2);
  }

  for(i = 0; i < MAXNUMTOKENS; i++){
    prog->wds[i][0] = '\0';
  }
  if(!(fp = fopen(file, "r"))){
    fprintf(stderr, "Cannot open %s\n", file);
    exit(2);
  }

  /* Modified version of https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c */
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  /* +1 to account for adding null character */
  stream = malloc(len+1);
  if(stream == NULL){
    fclose(fp);
    ERROR("Failed to create space for file stream.");
  }
  fread(stream, 1, len, fp);
  fclose(fp);
  stream[len] = '\0';


  i = 0;
  while(stream[i] != '\0'){
    j = 0;
    if(stream[i] == '\"'){
      word[j] = stream[i];
      i++;
      j++;
      while(stream[i] != '\"'){
        word[j] = stream[i];
        i++;
        j++;
      }
      word[j] = stream[i];
      i++;
      j++;
      word[j] = '\0';
      strcpy(prog->wds[prog->cw], word);
      prog->cw++;
    }else{
      if(isalnum(stream[i]) || ispunct(stream[i])){
        while(isalnum(stream[i]) || ispunct(stream[i])){
          word[j] = stream[i];
          i++;
          j++;
        }
        word[j] = '\0';
        strcpy(prog->wds[prog->cw], word);
        prog->cw++;
      }
    }
    i++;
  }

  printArray(prog);


  prog->cw = 0;





}


char* rot18(char* string)
{
  char* new;
  unsigned long i;
  char temp;
  if(string[0] != '#'){
    return NULL;
  }
  new = (char *)malloc(strlen(string)-2);
  if(new == NULL){
    ERROR("Failed to compute ROT18.");
  }
  for(i = 0; i < strlen(string) - 2; i++){
    if(isalpha(string[i+1])){
      if(isupper(string[i+1])){
        temp = string[i+1] - 'A';
        temp = (temp + 13) % 26;
        new[i] = temp + 'A';
      }else{
        temp = string[i+1] - 'a';
        temp = (temp + 13) % 26;
        new[i] = temp + 'a';
      }
    }
    if(isdigit(string[i+1])){
      temp = string[i+1] - '0';
      temp = (temp + 5) % 10;
      new[i] = temp + '0';
    }
    if(!isdigit(string[i+1]) && ! isalpha(string[i+1])){
      new[i] = string[i+1];
    }
  }
  return new;
}

void Print(Program *p)
{
  if(whatVarcon(p->wds[p->cw]) == nil){
    ERROR("Expected a variable or constant following 'PRINT'.");
  }
  p->cw++;
}



varcon whatVarcon(char* string)
{
  int i = 0;
  int point;
  if(string[0] == '$'){
    i++;
    while(string[i] != '\0'){
      if(!isupper(string[i])){
          return nil;
      }
      i++;
    }
    return strvar;
  }
  if(string[0] == '%'){
    i++;
    while(string[i] != '\0'){
      if(!isupper(string[i])){
          return nil;
      }
      i++;
    }
    return numvar;
  }
  if(string[0] == '\"'){
    if(string[strlen(string)-1] == '\"'){
      return strcon;
    }else{
      return nil;
    }
  }
  if(string[0] == '#'){
    if(string[strlen(string)-1] == '#'){
      return strcon;
    }else{
      return nil;
    }
  }
  if(isdigit(string[0])){
    point = 0;
    while(string[i] != '\0'){
      /* Checks for a number or a decimal point. */
      if((!isdigit(string[i])) && (string[i] != '.')){
          return nil;

      }
      if(string[i] == '.'){
        point++;
        /* If too many decimal points in the number. */
        if(point > 1){
          return nil;
        }
      }
      i++;
    }
    return numcon;
  }
  /* function will return nil if it does not meet any of the above criteria */
  return nil;
}



void test(void)
{
  char string1[] = "$TEST";
  char string2[] = "%TEST";
  char string3[] = "\"This is test number 3.\"";
  char string4[] = "#This is test number 4.#";
  char string5[] = "58.392";
  char string6[] = "$5a";
  char string7[] = "%'h'";
  char string8[] = "3.58.7";

  char rot181[] = "#Arvyy#";
  char rot182[] = "#Uryyb Jbeyq!#";
  char* new;
  int k;

  assert(whatVarcon(string1) == strvar);
  assert(whatVarcon(string2) == numvar);
  assert(whatVarcon(string3) == strcon);
  assert(whatVarcon(string4) == strcon);
  assert(whatVarcon(string5) == numcon);
  assert(whatVarcon(string6) == nil);
  assert(whatVarcon(string7) == nil);
  assert(whatVarcon(string8) == nil);

  new = rot18(rot181);
  k = strcmp(new, "Neill");
  printf("%s\n", new);
  printf("%d\n", k);
  assert(k == 0);
  new = rot18(rot182);
  k = strcmp(new, "Hello World!");
  printf("%s\n", new);
  assert(k == 0);






}








/*
bool isVarcon(){




}

bool isVar(){



}


bool isCon(){



}


bool isStrvar(){



}


bool isNumvar(){



}



bool isStrcon(){



}



bool isNumcon(){



}

*/
