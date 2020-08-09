#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>

#include "mvm.h"
#include "nal.h"

void test(void);


int main(int argc, char** argv)
{
  Program* p;
  mvm* variables;

  #ifdef TEST
  test();
  return 0;
  #endif

  if(argc != 2){
    fprintf(stderr, "Incorrect usage. Please type %s followed by file name.\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  variables = mvm_init();
  p = fileManager(argv[1], variables);
  srand(time(0));

  /* Start the program. */
  Prog(p);
  #ifndef INTERP
  printf("Parsed Ok\n");
  #endif

  /* Final frees */
  mvm_free(&variables);
  clearProg(p);
  free(p);

  return 0;
}



/* Takes the file name, reads the file and saves contents in a single character array. */
Program* fileManager(char* file, mvm* variables)
{
  int len;
  FILE *fp;
  Program *p;
  char *stream;

  p = (Program *)malloc(sizeof(Program));
  if(p == NULL){
    fprintf(stderr, "Cannot malloc space for program with file %s.", file);
    exit(EXIT_FAILURE);
  }
  if(!(fp = fopen(file, "r"))){
    fprintf(stderr, "Cannot open %s\n", file);
    exit(EXIT_FAILURE);
  }
  /* Modified version of https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c */
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  /* +1 to account for adding null character */
  stream = malloc(len+1);
  if(stream == NULL){
    fclose(fp);
    fprintf(stderr, "Failed to create space for file stream.");
    exit(EXIT_FAILURE);
  }
  /* Read the file into the 'stream' array and set a null terminator at the end. */
  fread(stream, 1, len, fp);
  stream[len] = '\0';
  fclose(fp);
  string2Prog(stream, p);
  free(stream);
  p->cw = 0;
  p->vars = variables;
  p->abort_flag = 0;
  return p;
}

/* Takes the file stream and seperates it into words which it then saves in the program structure. */
void string2Prog(char* stream, Program* p)
{
  int i = 0, j = 0;
  char word[TOKENBUFFER];
  char hashQuote;
  int word_count;

  word_count = countWords(stream);
  p->wds = (char **)calloc(word_count, sizeof(char*));
  p->cw = 0;
  while(stream[i] != '\0'){
    j = 0;
    /* Strings within quotation marks or # signs are to be saved as one 'word'. */
    if(stream[i] == '\"' || stream[i] == '#'){
      hashQuote = stream[i];
      COPYLETTER;
      while(stream[i] != hashQuote){
        COPYLETTER;
      }
      COPYLETTER;
      word[j] = '\0';
      insertWord(p, word, j+1);
      /* Else each word is seperated by space. */
    }else{
      if(isalnum(stream[i]) || ispunct(stream[i])){
        while(isalnum(stream[i]) || ispunct(stream[i])){
          COPYLETTER;
        }
        word[j] = '\0';
        insertWord(p, word, j+1);
      }
    }
    i++;
  }
  p->size = p->cw;
}

/* Insert word into program */
void insertWord(Program* p, char* word, int length)
{
  char* temp;

  temp = (char *)calloc(length, sizeof(char));
  strcpy(temp, word);
  p->wds[p->cw] = temp;
  NEXTWORD;
}


/* Counts number of words in the file so the appropriate space can be created with alloc */
int countWords(char* stream)
{
  int i = 0;
  char symbol;
  /* Count initialised to 1 to make up for the final word not having a space character following it. */
  int count = 1;
  while(stream[i] != '\0'){
    if(stream[i] == '\"' || stream[i] == '#'){
      symbol = stream[i];
      i++;
      while(stream[i] != symbol){
        i++;
      }
      i++;
    }
    if(stream[i] == ' '){
      count++;
    }
    i++;
  }
  return count;
}


void Prog(Program *p)
{
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, "{"))){
    ERROR("No '{' at start of file?");
  }
  NEXTWORD;

  #ifndef TEST
  Instrs(p);
  #endif
}


void Instrs(Program *p)
{
  #ifdef INTERP
  if(p->abort_flag == 1){
    return;
  }
  #endif
  if(strsame(CURRENTWORD, "}")){
    NEXTWORD;
    return;
  }
  Instruct(p);
  Instrs(p);
}


void Instruct(Program *p)
{
  if(OUTOFBOUNDS){
    ERROR("Expected instruction word.");
    return;
  }
  if(strsame(CURRENTWORD, "FILE")){
    NEXTWORD;
    #ifndef TEST
    file(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "PRINT")){
    NEXTWORD;
    #ifndef TEST
    Print(p);
    #ifdef INTERP
    printf("\n");
    #endif
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "PRINTN")){
    NEXTWORD;
    #ifndef TEST
    Print(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "ABORT")){
    NEXTWORD;
    #ifdef INTERP
    p->abort_flag = 1;
    #endif
    return;
  }
  if(CURRENTWORD[0] == '$' || CURRENTWORD[0] == '%'){
    #ifndef TEST
    set(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "RND")){
    NEXTWORD;
    #ifndef TEST
    rnd(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "INC")){
    NEXTWORD;
    #ifndef TEST
    inc(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "JUMP")){
    NEXTWORD;
    #ifndef TEST
    jump(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "IFEQUAL")){
    NEXTWORD;
    #ifndef TEST
    #ifdef INTERP
    equalGreater(p, EQUAL);
    #else
    equalGreater(p);
    #endif
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "IFGREATER")){
    NEXTWORD;
    #ifndef TEST
    #ifdef INTERP
    equalGreater(p, GREATER);
    #else
    equalGreater(p);
    #endif
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "INNUM")){
    NEXTWORD;
    #ifndef TEST
    innum(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "IN2STR")){
    NEXTWORD;
    #ifndef TEST
    in2str(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "SAVE")){
    NEXTWORD;
    #ifndef TEST
    saveGame(p);
    #endif
    return;
  }
  if(strsame(CURRENTWORD, "LOAD")){
    NEXTWORD;
    #ifndef TEST
    loadGame(p);
    #endif
    return;
  }
  /* If none of the above 'if' statements are met then a default 'catch-all' is used with an error message. */
  ERROR("Expected a valid instruction.");
}


/* Takes a string and either removes the quotation marks if present or sends string to rot18 if required. */
char* processString(char* string)
{
  char* new;
  unsigned i;
  new = (char *)calloc(strlen(string), sizeof(char));
  if(new == NULL){
    fprintf(stderr, "Failed to compute process string consant.\n");
    exit(EXIT_FAILURE);
  }
  /* If string is within quotation marks then it only needs to be stripped of the quotation marks */
  if(string[0] == '\"'){
    for(i = 0; i < (strlen(string)-2); i++){
      new[i] = string[i+1];
    }
  }else{  /* Else the string constant must be inside # sign and require rot18 */
    rot18(string, new);
  }
  return new;
}


void rot18(char* string, char* new)
{
  unsigned i;
  char temp;
  for(i = 0; i < (strlen(string)-2); i++){
    /* rotate by 13 */
    if(isalpha(string[i+1])){
      if(isupper(string[i+1])){
        temp = string[i+1] - 'A';
        /* 26 letters in alphabet. Modulus to wrap round to the start. */
        temp = (temp + 13) % 26;
        new[i] = temp + 'A';
      }else{
        temp = string[i+1] - 'a';
        temp = (temp + 13) % 26;
        new[i] = temp + 'a';
      }
    }
     /* rotate by 5 */
    if(isdigit(string[i+1])){
      temp = string[i+1] - '0';
      /* 10 integers. Modulus to wrap round to the start. */
      temp = (temp + 5) % 10;
      new[i] = temp + '0';
    }
    if(!isdigit(string[i+1]) && !isalpha(string[i+1])){
      new[i] = string[i+1];
    }
  }
}


varcon whatVarcon(char* string)
{
  varcon word = nil;
  if(string == NULL){
    return word;
  }
  if(string[0] == '$'){
    word = checkStrvar(string);
  }
  if(string[0] == '%'){
    word = checkNumvar(string);
  }
  if(string[0] == '\"' || string[0] == '#'){
    word = checkStrcon(string);
  }
  if(isdigit(string[0])){
    word = checkNumcon(string);
  }
  /* function will return nil if it does not meet any of the above criteria */
  return word;
}

varcon checkStrvar(char* string)
{
  int i = 1;
  while(string[i] != '\0'){
    if(!isupper(string[i])){
        return nil;
    }
    i++;
  }
  return strvar;
}

varcon checkNumvar(char* string)
{
  int i = 1;
  while(string[i] != '\0'){
    if(!isupper(string[i])){
        return nil;
    }
    i++;
  }
  return numvar;
}

varcon checkStrcon(char* string)
{
  if(string[0] == '\"'){
    if(string[strlen(string)-1] == '\"'){
      return strcon;
    }
  }
  if(string[0] == '#'){
    if(string[strlen(string)-1] == '#'){
      return strcon;
    }
  }
  /* If neither of the above if statements are met then the 'word' does not meet criteria for a varcon */
  return nil;
}

varcon checkNumcon(char* string)
{
  int i = 0;
  int point = 0;
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


void saveVariable(Program* p, char* key, char* value)
{
  /* My extension saves all variables, therefore I need to delete old variables before saving new ones. */
  if(mvm_search(p->vars, key) != NULL){
    mvm_delete(p->vars, key);
  }
  mvm_insert(p->vars, key, value);
}


/* This function parses the 'FILE' command, processes the string constant and runs a new program with the new file */
void file(Program* p)
{
  #ifdef INTERP
  Program *newprog;
  char* file;
  #endif

  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) != strcon){
    ERROR("Expected string constant following 'FILE' command.");
  }

  #ifdef INTERP
  file = processString(CURRENTWORD);
  newprog = fileManager(file, p->vars);
  free(file);
  Prog(newprog);
  p->abort_flag = newprog->abort_flag;
  clearProg(newprog);
  free(newprog);
  #endif

  NEXTWORD;
}

void Print(Program *p)
{
  #ifdef INTERP
  varcon word;
  char* string_con;
  #endif
  /* Parse */
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) == nil){
    ERROR("Expected a variable or constant following 'PRINT'.");
  }
  /* Interp */
  #ifdef INTERP
  word = whatVarcon(CURRENTWORD);
  if(word == strvar){
    string_con = processString(mvm_search(p->vars, CURRENTWORD));
    printString(string_con);
    free(string_con);
  }
  if(word == numvar){
    printf("%s", mvm_search(p->vars, CURRENTWORD));
  }
  if(word == strcon){
    string_con = processString(CURRENTWORD);
    printString(string_con);
    free(string_con);
  }
  if(word == numcon){
    printf("%s", CURRENTWORD);
  }
  #endif
  NEXTWORD;
}

/* Prints a string whilst accounting for new line characters or tab charcters */
void printString(char* string)
{
  int i = 0;
  while(string[i+1] != '\0' && string[i] != '\0'){
    if((string[i] == '\\') && (string[i+1] == 'n')){
      printf("\n");
      i++;
    }else if ((string[i] == '\\') && (string[i+1] == 't')){
      printf("\t");
      i++;
    }else{
      printf("%c", string[i]);
    }
    i++;
  }
  printf("%c", string[i]);
}


void set(Program *p)
{
  char* key;
  if(!(OUTOFBOUNDS) && ((whatVarcon(CURRENTWORD) == strvar) || (whatVarcon(CURRENTWORD) == numvar))){
    key = CURRENTWORD;
    NEXTWORD;
    if(OUTOFBOUNDS || !strsame(CURRENTWORD, "=")){
      ERROR("Expected '=' for variable declaration.");
    }
    NEXTWORD;
    if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) == nil){
      ERROR("Expected constant after '=' in SET declaration.");
    }
    #if defined INTERP || defined TEST
    if((whatVarcon(key) == strvar) && (whatVarcon(CURRENTWORD) != strcon)){
      ERROR("Expected string constant following '='.");
    }
    if((whatVarcon(key) == numvar) && (whatVarcon(CURRENTWORD) != numcon)){
      ERROR("Expected number constant following '='.");
    }
    saveVariable(p, key, CURRENTWORD);
    #endif
    NEXTWORD;
    return;
  }
  ERROR("Expected variable before '=' in SET declaration.");
}


void rnd(Program *p)
{
  #if defined INTERP || defined TEST
  int number;
  /* Maximum size of 3 as the number is between 0-99 (plus null terminator). */
  char number_string[3];
  #endif

  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, "("))){
    ERROR("Missing '(' after RND statement.");
  }
  NEXTWORD;
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) != numvar){
    ERROR("Expected number variable as argument for RND");
  }

  #if defined INTERP || defined TEST
  number = rand() % 100;
  /* Copy integer into a string */
  sprintf(number_string, "%d", number);
  saveVariable(p, CURRENTWORD, number_string);
  #endif

  NEXTWORD;
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, ")"))){
    ERROR("Missing ')' after RND statement?");
  }
  NEXTWORD;
}




void inc(Program *p)
{
  #if defined INTERP || defined TEST
  double number;
  char* ptr;
  char number_string[BUFFER];
  char* value;
  #endif

  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, "("))){
    ERROR("Missing '(' after INC statement?");
  }
  NEXTWORD;
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) != numvar){
    ERROR("Expected number variable as argument for INC");
  }

  #if defined INTERP || defined TEST
  value = mvm_search(p->vars, CURRENTWORD);
  if(value == NULL){
    ERROR("Please assign number variable before attempting to increment with INC.");
  }
  number = strtod(value, &ptr);
  number++;
  /* %g used to remove unnecessary decimal places. */
  sprintf(number_string, "%g", number);
  saveVariable(p, CURRENTWORD, number_string);
  #endif

  NEXTWORD;
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, ")"))){
    ERROR("Missing ')' after RND statement?");
  }
  NEXTWORD;
}




void jump(Program* p)
{
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) != numcon){
    ERROR("Expected a number constant after 'JUMP' command.");
  }

  #if defined INTERP || defined TEST
  /* I have subtracted 1 to account for the fact that the parser needs to increment the word count by 1 where as the interpreter does not. */
  p->cw = atoi(CURRENTWORD) - 1;
  #endif
  NEXTWORD;
}



#ifdef INTERP
void equalGreater(Program *p, int flag)
#else
void equalGreater(Program *p)
#endif
{
  #ifdef INTERP
  char *word1, *word2;
  #endif
  varcon var_const1, var_const2;
  /* Temp pointers used to keep track of the token when the program moves on. */
  char *temp1, *temp2;
  /* parse */
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, "("))){
    ERROR("Missing '(' after IFEQUAL statement.");
  }
  NEXTWORD;
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) == nil){
    ERROR("Expected variable or constant within IFEQUAL/IFGREATER statement.");
  }
  var_const1 = whatVarcon(CURRENTWORD);
  temp1 = CURRENTWORD;
  NEXTWORD;
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, ","))){
    ERROR("Expected ',' between variables/constants inside statement.");
  }
  NEXTWORD;
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) == nil){
    ERROR("Expected variable or constant within IFEQUAL/IFGREATER statement?");
  }
  var_const2 = whatVarcon(CURRENTWORD);
  temp2 = CURRENTWORD;
  NEXTWORD;
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, ")"))){
    ERROR("Expected ')' at end of IFEQUAL/IFGREATER statement.");
  }
  NEXTWORD;
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, "{"))){
    ERROR("Expected '{' following IFEQUAL/IFGREATER statement.");
  }
  NEXTWORD;
  /* interp */
  #ifdef INTERP
  word1 = extractWord(p, var_const1, temp1);
  word2 = extractWord(p, var_const2, temp2);
  if(flag == EQUAL){
    ifEqual(p, word1, word2);
  }
  if(flag == GREATER){
    ifGreater(p, word1, word2, var_const1, var_const2);
  }
  /* If either of the variables/constants were strings then they need freeing as have been 'alloced' */
  if(var_const1 == strvar || var_const1 == strcon){
    free(word1);
  }
  if(var_const2 == strvar || var_const2 == strcon){
    free(word2);
  }
  #endif
}


void ifEqual(Program *p, char* word1, char* word2)
{
  if(strsame(word1, word2)){
    Instrs(p);
  }else{
    skipBraces(p);
  }
}


void ifGreater(Program *p, char* word1, char* word2, varcon var_const1, varcon var_const2)
{
  if(var_const1 == numvar || var_const1 == numcon){
    if(var_const2 != numvar && var_const2 != numcon){
      ERROR("Not able to compare strings and numbers in IFGREATER.");
    }
    if((atof(word1) - atof(word2)) > 0){
      Instrs(p);
    }else{
      skipBraces(p);
    }
  }
  if(var_const1 == strvar || var_const1 == strcon){
    if(var_const2 != strvar && var_const2 != strcon){
      ERROR("Not able to compare strings and numbers in IFGREATER.");
    }
    if(strcmp(word1, word2) > 0){
      Instrs(p);
    }else{
      skipBraces(p);
    }
  }
}


char* extractWord(Program* p, varcon var_const, char* word)
{
  char* result;
  if(var_const == numvar){
    result = mvm_search(p->vars, word);
  }
  if(var_const == strvar){
    result = processString(mvm_search(p->vars, word));
  }
  if(var_const == numcon){
    result = word;
  }
  if(var_const == strcon){
    result = processString(word);
  }
  return result;
}


void skipBraces(Program *p)
{
  /* The brace count starts at 1 as we have already had one opening brace.
  The count is done to ensure the correct braces are skipped. */
  int count = 1;

  while(count != 0){
    if(strsame(CURRENTWORD, "}")){
      count--;
    }
    if(strsame(CURRENTWORD, "{")){
      count++;
    }
    NEXTWORD;
  }
}

void innum(Program* p)
{
  #ifdef INTERP
  char number[BUFFER];
  #endif

  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, "("))){
    ERROR("Missing '(' after INNUM statement?");
  }
  NEXTWORD;
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) != numvar){
    ERROR("Expected number variable as argument for INNUM");
  }

  #ifdef INTERP
  printf("\n> ");
  if(scanf("%s", number) != 1){
    fprintf(stderr, "Failed to scan input.\n");
    exit(EXIT_FAILURE);
  }
  saveVariable(p, CURRENTWORD, number);
  #endif
  #ifdef TEST
  saveVariable(p, "%A", "42");
  #endif

  NEXTWORD;
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, ")"))){
    ERROR("Missing ')' after INNUM statement?");
  }
  NEXTWORD;
}


void in2str(Program *p)
{
  /* Temp pointer used to keep track of first variable/constant when the program moves on. */
  char* temp;

  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, "("))){
    ERROR("Missing '(' after IN2STR statement.");
  }
  NEXTWORD;
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) != strvar){
    ERROR("Expected string variable as first argument for IN2STR.");
  }
  temp = CURRENTWORD;
  NEXTWORD;
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, ","))){
    ERROR("Missing ',' between variables in IN2STR statement.");
  }

  NEXTWORD;
  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) != strvar){
    ERROR("Expected string variable as second argument for IN2STR");
  }

  #ifdef INTERP
  in2StrInterp(p, temp, CURRENTWORD);
  #endif

  NEXTWORD;
  if(OUTOFBOUNDS || !(strsame(CURRENTWORD, ")"))){
    ERROR("Missing ')' after IN2STR statement?");
  }
  NEXTWORD;
}


void in2StrInterp(Program* p, char* var1, char* var2)
{
  char scan1[BUFFER], scan2[BUFFER];
  char string1[BUFFER], string2[BUFFER];
  #ifdef INTERP
  printf("\n> ");
  if(scanf("%s %s", scan1, scan2) != 2){
    fprintf(stderr, "Failed to scan input.\n");
    exit(EXIT_FAILURE);
  }
  #endif
  #ifdef TEST
  strcpy(scan1, "TESTING");
  strcpy(scan2, "IN2STR");
  #endif
  /* Add quotation marks around the strings before inserting into the mvm. */
  sprintf(string1, "\"%s\"", scan1);
  sprintf(string2, "\"%s\"", scan2);
  saveVariable(p, var1, string1);
  saveVariable(p, var2, string2);
}


void clearProg(Program* p)
{
  int i = 0;
  while(i < p->size){
    free(p->wds[i]);
    p->wds[i] = NULL;
    i++;
  }
  free(p->wds);
}

/* -------------EXTENSION BELOW HERE-------------- */

void saveGame(Program* p)
{
  FILE* fp;
  char name[BUFFER];
  char file[BUFFER];
  char* variables;

  if(OUTOFBOUNDS || whatVarcon(CURRENTWORD) != strcon){
    ERROR("Expected string constant following save command.");
  }
  printf("\n> ");
  if(scanf("%s", name) != 1){
    fprintf(stderr, "Not able to scan input for save game.\n");
    exit(EXIT_FAILURE);
  };
  sprintf(file, "%s.save", name);
  if(!(fp = fopen(file, "w+"))){
    fprintf(stderr, "Cannot save file.\n");
    exit(EXIT_FAILURE);
  }
  /* First need to save file name so know where to start when loading. */
  fprintf(fp, "%s\n", CURRENTWORD);
  /* Next need to save variables, incase they are needed again in the game. */
  variables = mvm_print(p->vars);
  fprintf(fp, "%s\n", variables);

  fclose(fp);
  free(variables);

  NEXTWORD;
  printf("------------GAME SAVED SUCCESSFULLY------------\n");
}


void loadGame(Program* p)
{
  char name[BUFFER];
  char* file_name;
  char temp_file_name[BUFFER];
  mvm* newMvm;
  char* stream;
  Program* newprog;
  if(printSaves() == 0){
    printf("\n---------There are no previous save files---------\n");
    return;
  }
  printf("\n> ");
  if(scanf("%s", name) != 1){
    fprintf(stderr, "Not able to scan input to load game.\n");
    exit(EXIT_FAILURE);
  };
  strcat(name, ".save");

  newMvm = mvm_init();
  stream = loadManager(name);
  load2Prog(stream, newMvm, temp_file_name);
  file_name = processString(temp_file_name);
  newprog = fileManager(file_name, newMvm);

  free(stream);
  free(file_name);


  printf("------------GAME LOADED SUCCESSFULLY-----------\n");
  Prog(newprog);
  mvm_free(&(newprog->vars));
  clearProg(newprog);
  free(newprog);
  /* set abort flag to 1 so that old program does not continue running. This also
  ensures everything is freed correctly in the old program. */
  p->abort_flag = 1;
}


int printSaves(void)
{
  int count = 0;
  DIR *dir;
  char* ext;
  int i;
  struct dirent *ent;
  if((dir = opendir(".")) != NULL){
    printf("Previous save files: \n");
    while((ent = readdir(dir)) != NULL){
      i = 0;
      ext = strrchr(ent->d_name, '.');
      if(ext){
        if(strsame(ext, ".save")){
          printf("- ");
          count++;
          while(ent->d_name[i] != '.'){
            printf("%c", ent->d_name[i]);
            i++;
          }
          printf("\n");
        }
      }
    }
    closedir(dir);
  }else{
    fprintf(stderr, "Not able to check previous saves.\n");
    exit(EXIT_FAILURE);
  }
  return count;
}


char* loadManager(char* file)
{
  int len;
  FILE *fp;
  char *stream;

  if(!(fp = fopen(file, "r"))){
    fprintf(stderr, "Cannot open %s\n", file);
    exit(EXIT_FAILURE);
  }
  /* Modified version of https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c */
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  /* +1 to account for adding null character */
  stream = malloc(len+1);
  if(stream == NULL){
    fclose(fp);
    fprintf(stderr, "Failed to create space for file stream.\n");
  }
  /* Read the file into the 'stream' array and set a null terminator at the end. */
  fread(stream, 1, len, fp);
  stream[len] = '\0';
  fclose(fp);

  return stream;
}


/* Takes the file stream and seperates it into words which it then saves in the program structure. */
void load2Prog(char* stream, mvm* variables, char* file)
{
  int i, j;
  char key[TOKENBUFFER];
  char value[TOKENBUFFER];

  /* First save the file name */
  sscanf(stream, "%s", file);
  /* Now save the variables to the mvm */
  i = 0;
  while(stream[i] != '\0'){
    if(stream[i] == '['){
      i++;
      j = 0;
      while(stream[i] != ']'){
        key[j] = stream[i];
        i++;
        j++;
      }
      key[j] = '\0';
    }
    if(stream[i] == '('){
      i++;
      j = 0;
      while(stream[i] != ')'){
        value[j] = stream[i];
        i++;
        j++;
      }
      value[j] = '\0';
      mvm_insert(variables, key, value);
    }
    i++;
  }
}


#ifdef TEST

void test(void)
{
  char* testString;

  int k;

  Program prog;
  char stream[TOKENBUFFER] = "Hello World! This is a test. Curly braces { should be their own word. As should = and ,\
 \"Anything within quotations\" #or within hashes# however should be treated as a single word.";
  char string[TOKENBUFFER] = "$TEST";
  char rnd_stream[20] = "( %RNDNUM )";
  char rnd_var[20] = "%RNDNUM";
  char* num_string;
  int rnd_num;
  int inc_num;

  char set_var[20] = "$A";
  char set_var2[20] = "$B";
  char* set_string;

  mvm* test_mvm;


  test_mvm = mvm_init();
  prog.vars = test_mvm;

/* ------- Testing 'functionality'/interpreter of functions below (see further down for parsing of functions) ---------- */
  /* Testing word count */
  assert(countWords(stream) == 29);
  assert(countWords(rnd_stream) == 3);
  assert(countWords(rnd_var) == 1);

  /* Testing insertWord() */
  prog.cw = 0;
  prog.wds = (char **)calloc(3, sizeof(char*));
  prog.size = 3;
  insertWord(&prog, "Hello", 6);
  insertWord(&prog, "World", 6);
  insertWord(&prog, "", 1);
  k = strcmp(prog.wds[0], "Hello");
  assert(k == 0);
  k = strcmp(prog.wds[1], "World");
  assert(k == 0);
  k = strcmp(prog.wds[2], "");
  assert(k == 0);
  clearProg(&prog);


  /* Testing string2Prog() function */

  prog.cw = 0;
  string2Prog(stream, &prog);
  assert(prog.cw == 29);

  k = strcmp(prog.wds[0], "Hello");
  assert(k == 0);
  k = strcmp(prog.wds[1], "World!");
  assert(k == 0);
  k = strcmp(prog.wds[5], "test.");
  assert(k == 0);
  k = strcmp(prog.wds[8], "{");
  assert(k == 0);
  k = strcmp(prog.wds[14], "As");
  assert(k == 0);
  k = strcmp(prog.wds[16], "=");
  assert(k == 0);
  k = strcmp(prog.wds[18], ",");
  assert(k == 0);
  k = strcmp(prog.wds[19], "\"Anything within quotations\"");
  assert(k == 0);
  k = strcmp(prog.wds[20], "#or within hashes#");
  assert(k == 0);
  k = strcmp(prog.wds[27], "single");
  assert(k == 0);
  k = strcmp(prog.wds[28], "word.");
  assert(k == 0);
  clearProg(&prog);



  /* Testing the various variable and constant functions */
  strcpy(string, "$TEST");
  assert(checkStrvar(string) == strvar);
  assert(whatVarcon(string) == strvar);
  strcpy(string, "%TEST");
  assert(checkNumvar(string) == numvar);
  assert(whatVarcon(string) == numvar);
  strcpy(string, "\"This is test number 3.\"");
  assert(checkStrcon(string) == strcon);
  assert(whatVarcon(string) == strcon);
  strcpy(string, "#This is test number 4.#");
  assert(whatVarcon(string) == strcon);
  strcpy(string, "58.392");
  assert(checkStrcon(string) == nil);
  assert(whatVarcon(string) == numcon);
  strcpy(string, "$5a");
  assert(whatVarcon(string) == nil);
  strcpy(string, "%'h'");
  assert(whatVarcon(string) == nil);
  strcpy(string, "3.58.7");
  assert(whatVarcon(string) == nil);


  /* Testing processString() function. */
  strcpy(string, "#Arvyy#");
  testString = processString(string);
  k = strcmp(testString, "Neill");
  assert(k == 0);
  free(testString);
  strcpy(string, "#Uryyb Jbeyq!#");
  testString = processString(string);
  k = strcmp(testString, "Hello World!");
  assert(k == 0);
  free(testString);
  strcpy(string, "\"HELLO WORLD\"");
  testString = processString(string);
  k = strcmp(testString, "HELLO WORLD");
  assert(k == 0);
  free(testString);


  /* Testing jump() function. */
  memset(string, '\0',sizeof(string));
  strcpy(string, "JUMP 5");
  string2Prog(string, &prog);
  prog.cw = 1;
  jump(&prog);
  assert(prog.cw == 5);
  clearProg(&prog);


  /* Testing rnd() function. */
  string2Prog(rnd_stream, &prog);
  prog.cw = 0;
  rnd(&prog);
  num_string = mvm_search(prog.vars, rnd_var);
  rnd_num = atoi(num_string);
  assert(rnd_num >= 0);
  assert(rnd_num <= 99);

  /* Testing inc() function. */
  prog.cw = 0;
  inc(&prog);
  num_string = mvm_search(prog.vars, rnd_var);
  inc_num = atoi(num_string);
  assert(inc_num - rnd_num == 1);
  clearProg(&prog);

  /* Testing set() function. */
  strcpy(string, "$A = \"HELLO\"");
  string2Prog(string, &prog);
  prog.cw = 0;
  set(&prog);
  set_string = mvm_search(prog.vars, set_var);
  k = strcmp(set_string, "\"HELLO\"");
  assert(k == 0);
  clearProg(&prog);
  memset(string, '\0',sizeof(string));

  /* Testing skipBraces function. */

  strcpy(string, "}");
  string2Prog(string, &prog);
  prog.cw = 0;
  skipBraces(&prog);
  assert(prog.cw == 1);
  clearProg(&prog);
  memset(string, '\0',sizeof(string));

  strcpy(string, "Hello World! } }");
  string2Prog(string, &prog);
  prog.cw = 0;
  skipBraces(&prog);
  assert(prog.cw == 3);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "{ Hello World! } Next is the correct closing brace. } These words should not be counted. }");
  string2Prog(string, &prog);
  prog.cw = 0;
  skipBraces(&prog);
  assert(prog.cw == 11);
  prog.cw = 18;
  clearProg(&prog);

/* Testing in2Str and innum using predefined inputs to replace user input. */
  in2StrInterp(&prog, set_var, set_var2);
  testString = mvm_search(prog.vars, set_var);
  k = strcmp(testString, "\"TESTING\"");
  assert(k == 0);
  set_string = mvm_search(prog.vars, set_var2);
  k = strcmp(mvm_search(prog.vars, set_var2), "\"IN2STR\"");
  assert(k == 0);

  innum(&prog);
  testString = mvm_search(prog.vars, "%A");
  k = strcmp(testString, "42");
  assert(k == 0);

/*-------------------- Parsing tests below ------------------*/

  /* Parsing Prog() function  */
  memset(string, '\0',sizeof(string));
  strcpy(string, "{ HELLO }");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Prog(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "{ HELLO");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Prog(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "HELLO }");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Prog(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  /* Parsing Instruct() function. Should pass with any valid instruct as the first word. */
  memset(string, '\0',sizeof(string));
  strcpy(string, "HELLO");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Instruct(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "PRINT HELLO");
  prog.cw = 0;
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Instruct(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "ABORT");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Instruct(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "{ IFEQUAL { varcon, varcon }");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Instruct(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "IFGREATER { varcon, varcon }");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Instruct(&prog);
  assert(TESTPASS);
  clearProg(&prog);


  memset(string, '\0',sizeof(string));
  strcpy(string, "");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Instruct(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  /* Testing file() function. Needs a string constant. */
  memset(string, '\0',sizeof(string));
  strcpy(string, "\"FILE.TXT\"");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  file(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "HELLO.TXT");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  file(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "854");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  file(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "HELLO");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  file(&prog);
  assert(TESTFAIL);
  clearProg(&prog);


  /* Testing innum() function */
  memset(string, '\0',sizeof(string));
  strcpy(string, "( %NUM )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  innum(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( $NUM )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  innum(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "%NUM )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  innum(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( %NUM ");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  innum(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  innum(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  /* Testing in2Str() function */
  memset(string, '\0',sizeof(string));
  strcpy(string, "( $STR , $STR )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  in2str(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( %STR , $STR )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  in2str(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( $STR )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  in2str(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( $STR $STR )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  in2str(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  in2str(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  /* Testing rnd() function. This has the same parse structure as inc(). The different functionalities were tested above. */
  memset(string, '\0',sizeof(string));
  strcpy(string, "( %NUM )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  rnd(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( $NUM )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  rnd(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "{ $NUM )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  rnd(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( ");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  rnd(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  /* Testing jump() function */
  memset(string, '\0',sizeof(string));
  strcpy(string, "5");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  jump(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "JUMP 5");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  jump(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "5 HELLO WORLD");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  jump(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  /* Testing set() function. */

  memset(string, '\0',sizeof(string));
  strcpy(string, "$A = \"HELLO\"");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  set(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "$A = 42");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  set(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "%A = \"HELLO\"");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  set(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "Hello World!");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  set(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  /* Testing parser on print() function */
  memset(string, '\0',sizeof(string));
  strcpy(string, "\"Hello world!\"");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Print(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "#Hello world!#");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Print(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "Hello World");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  Print(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  /* Testing parser on equalGreater() function */
  memset(string, '\0',sizeof(string));
  strcpy(string, "( $HELLO , %WORLD ) {");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  equalGreater(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( , )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  equalGreater(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( \"HELLO\" , %WORLD ) {");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  equalGreater(&prog);
  assert(TESTPASS);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  equalGreater(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "( %WORLD )");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  equalGreater(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  memset(string, '\0',sizeof(string));
  strcpy(string, "{ $HELLO , %WORLD }");
  string2Prog(string, &prog);
  prog.cw = 0;
  prog.test_flag = 0;
  equalGreater(&prog);
  assert(TESTFAIL);
  clearProg(&prog);

  mvm_free(&test_mvm);
  printf("Tests passed!\n");

}
#endif
