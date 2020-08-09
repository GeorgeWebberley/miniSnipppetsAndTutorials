/* Multi-Value Map ADT : via Linked List
   Both key & data are strings (char*)
   Multiple Keys may be stored
   New data is inserted at the front of the list:
   O(1) insertion
   O(n) search
   O(n) deletion
*/

/* Error that can't be ignored */
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

/* I have placed the #defines and the helper function top lines in this header file so that they can be used with testfmvm.c */

/* BRACKSPACE adds space for the brackets and spaces required for the print function */
#define BRACKSPACE 5
/* Space for the 'substring' when building a string in the print function */
#define SUBSIZE 30
/* Prime number chosen for starting size of hash table */
#define STARTSIZE 53

/* Hash defined two different 'jobs' for the 'find_hash' function. This specifies if inserting into table or searching the table. */
#define INSERT 1
#define SEARCH 2


struct mvmcell {
   char* key;
   char* value;
   struct mvmcell* next;
};
typedef struct mvmcell mvmcell;

struct h_table {
   mvmcell** data;
   int size;
   int numkeys;
};
typedef struct h_table h_table;

struct mvm{
  h_table* table;
};
typedef struct mvm mvm;

mvm* mvm_init(void);
/* Number of key/value pairs stored */
int mvm_size(mvm* m);
/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data);
/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m);
/* Remove one key/value */
void mvm_delete(mvm* m, char* key);
/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key);
/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n);
/* Free & set p to NULL */
void mvm_free(mvm** p);


/* Various helper functions */

/* Modified djb2 by Dan Bernstein, taken from https://stackoverflow.com/questions/7666509/hash-function-for-string */
unsigned long hash1(char *str);
/* Generates hash number and inserts cell at hash location. */
void insert_hash(h_table *t, mvmcell *cell);
/* Calls 'resize' and loops through old table reinserting old cells */
void resize_table(mvm* m);
/* Finds new size for table and changes size */
void resize(h_table* table, int s);
/* Frees individual cell */
void free_cell(mvmcell** cell);
/* Searches hash table for key or empty cell, depending on the job specified. */
long find_hash(h_table* m, char* key, int job);
/* calculates length of string to be printed */
int str_len(h_table* m);
/* Checks if number is prime */
int is_prime(int n);
/* Finds the next prime number */
int find_next_prime(int n);
