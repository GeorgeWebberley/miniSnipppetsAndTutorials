#define strsame(A, B) (strcmp(A, B) == 0)
/* Buffer for tokensize. This spaces is eventuall alloced */
#define TOKENBUFFER 10000
/* Maximum size for scanf buffer in in2str and innum functions. */
#define BUFFER 30

/* Flags to be used for the equalGreater function. */
#define EQUAL 1
#define GREATER 2

#define NEXTWORD p->cw++
#define CURRENTWORD p->wds[p->cw]
#define OUTOFBOUNDS p->cw >= p->size
/* Copies one letter from the character stream to the word to be saved and increments the counters */
#define COPYLETTER {word[j] = stream[i]; i++; j++;}

/* If testing I do not want the program to quit when it raches an error. Instead I want it to raise a flag. */
#ifdef TEST
#define ERROR(PHRASE) p->test_flag = 1
#else
#define ERROR(PHRASE) {fprintf(stderr, "Fatal error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); exit(2);}
#endif


/* #defines specific to testing. */
#define TESTPASS prog.test_flag == 0
#define TESTFAIL prog.test_flag == 1


struct program{
  /* Array of pointers to store all words from the file */
  char** wds;
  /* Current word */
  int cw;
  /* Total number of words */
  int size;
  /* Variables to be stored here. */
  mvm* vars;
  /* This flag is set to 1 if a request to abort is made. */
  int abort_flag;
  /* This flag is purely for testing. It is set to 1 if the program meets an error */
  int test_flag;
};
typedef struct program Program;

/* function will return nil if it does not meet criteria of being a varcon */
enum varcon{nil, strvar, numvar, strcon, numcon};
typedef enum varcon varcon;

/* ------------File reading/tokenising functions-------------*/

/* Takes the file name, reads the file and saves contents in a single character array. */
Program* fileManager(char *file, mvm* variables);
/* Takes the file stream and seperates it into tokens which it then saves in the program structure. */
void string2Prog(char* stream, Program* prog);
/* Counts number of words in the file so the appropriate space can be created with alloc */
int countWords(char* stream);
/* Inserts 1 word into the program */
void insertWord(Program* p, char* word, int length);


/* --------------General program functions -------------*/

/* These functions start the program and calls instructions. */
void Prog(Program *p);
void Instrs(Program *p);
void Instruct(Program *p);

/* ----------------Helper functions-----------------*/

/* Takes a string and either removes the quotation marks if present or sends string to rot18 if required.
The new string is created through an alloc and therefore requires freeing. */
char* processString(char* string);
/* Applies rot13 to characters and rot 5 to numbers.  */
void rot18(char* string, char* new);
/* Returns if a word is a number-variable, string-variable, number-constant or string-constant (or nil none of them). */
varcon whatVarcon(char* string);
/* Checks the word meets criteria for the relevant variable/consant. Returns 'nil' otherwise. */
varcon checkStrvar(char* string);
varcon checkNumvar(char* string);
varcon checkStrcon(char* string);
varcon checkNumcon(char* string);
/* Checks variable is not already save in mvm. If it is, the function deletes it and inserts new variable. */
void saveVariable(Program* p, char* key, char* value);

/* --------------- Instructs -----------------*/

/* This function parses the 'FILE' command, processes the string constant and runs a new program with the new file */
void file(Program* p);
/* Print function that also takes an argument 'linefeed' flag */
void Print(Program *p);
/* Prints a string and accounts for new line characters and tab characters. */
void printString(char* string);
/* Checks statement meets requirements of the formal grammar. It then sets a number or string constant to a number
or string variable and saves it in the mvm 'variables'. */
void set(Program* p);
/* Generates a random number between 0-99 and assigns it to a variable before storing it in the mvm (as a string). */
void rnd(Program* p);
/* Obtains a number from mvm based on variable specified in the language. It converts this number-string
to a float, increments it by 1 and then saves it as a string back in the mvm under the same variable name. */
void inc(Program *p);
/* Moves the program counter to location in the file specified in the formal grammar. */
void jump(Program* p);
/* This function parses the syntax for ifEqual and ifGreater aswell as save relevant variables to be used in
the ifEqual and ifGreater functions. The 'flag' specifies if the ifEqual or ifGreater function should be used. */
#ifdef INTERP
void equalGreater(Program *p, int flag);
#else
void equalGreater(Program *p);
#endif
/* Checks two variables or constants to see if they are equal or if the second is greater than the first. If the
condition is not met in skips over the content within the following curly braces using skipBraces(). */
void ifEqual(Program *p, char* first, char* second);
void ifGreater(Program *p, char* first, char* second, varcon word1, varcon word2);
/* Obtains the word to be used in ifEqual and ifGreater based on what type of variable/constant it is */
char* extractWord(Program* p, varcon var_const, char* word);
/* Skips everything inside the curly braces if the ifEqual or ifGreater statements are not met. */
void skipBraces(Program *p);
/* Assigns a number (typed by the user) to a variable. The number is converted to string before the key/value
pair is saved in the mvm. */
void innum(Program *p);
/* Parses the formal grammar for in2str and then sends both variables to in2StrInterp(). */
void in2str(Program *p);
/* Takes two words as inputs from the user and saves them with the variable names in the mvm */
void in2StrInterp(Program* p, char* var1, char* var2);
/* Frees words stored in program */
void clearProg(Program* p);


/* --------------------Functions relating to the extension--------------- */

/* Asks user to input a name. Creates a text file with this name and saves the file name and all variables to this text file */
void saveGame(Program* p);
/* Prints file names and asks user to select file to open. Passes this file name to the loadManager */
void loadGame(Program* p);
/* Prints previous save files (if any) */
int printSaves(void);
/* Converts contents of file into one continuous string ready to be parsed */
char* loadManager(char* file);
/* Parses the string created by loadManager(), saving variables in a mvm */
void load2Prog(char* stream, mvm* variables, char* file);
