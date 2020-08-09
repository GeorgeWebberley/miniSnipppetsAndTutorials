#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmvm.h"


#define BRACKSPACE 5
#define SUBSIZE 30
#define STARTSIZE 53

/* Helper function to calculate length of string to be printed. */
int str_len(mvm* m);
/* Various helper functions */
int isPrime(int n);
int findNextPrime(int n);
unsigned long hash1( char *str);
unsigned long hash2( char *str);
void insert_hash(mvm **m, mvmcell *cell);
void free_cell(mvmcell** cell);
void resize(mvm* table, int s);
long find_hash(mvm* m, char* key);
long find_empty(mvm* m, char* key);



mvm* mvm_init(void)
{
  mvm* m;

  m = (mvm *) calloc(1, sizeof(mvm));

  if(m == NULL){
     printf("Creation of Table Failed\n");
     exit(EXIT_FAILURE);
  }

  m->data = (mvmcell**) calloc(STARTSIZE, sizeof(mvmcell*));
  if(m->data == NULL){
     printf("Creation of Table Failed\n");
     exit(EXIT_FAILURE);
  }
  m->size = STARTSIZE;
  m->numkeys = 0;
  return m;
}



int isPrime(int n)
{
  int i;
  if(n <= 1){
    return 0;
  }
  if(n == 2){
    return 1;
  }
  for(i = 2; i < n ; i++){
    if(n%i == 0){
      return 0;
    }
  }
  return 1;
}


int findNextPrime(int n)
{
  n++;
  while(!isPrime(n)){
    n++;
  }
  return n;
}




/* Number of key/value pairs stored */
int mvm_size(mvm* m)
{
  if(m == NULL){
    return 0;
  }
  return m->numkeys;
}





/* Modified djb2 by Dan Bernstein, taken from https://stackoverflow.com/questions/7666509/hash-function-for-string */
unsigned long hash1( char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)){
      hash = ((hash << 5) + hash) + c;
    } /* hash * 33 + c */

    return hash;
}

/* Taken from http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29 */
unsigned long hash2( char *str)
{
  unsigned long h;
  char *us;

  us = str;

  h = 0;
  while(*us != '\0') {
      h = h * 33 + *us;
      us++;
  }
  return h;
}


void free_cell(mvmcell** cell)
{
  mvmcell* c = *cell;
  if(cell == NULL){
    return;
  }
  printf("cell\n");
  free(c->value);
  free(c->key);
  free(c);
  *cell = NULL;
}


void mvm_free(mvm** t)
{
  mvm* a = *t;
  int i;
  for(i = 0; i< (*t)->size; i++){
    if(((*t)->data)[i] != NULL){
      free_cell(&(((*t)->data)[i]));
    }
  }
  printf("mvm\n");
  free(a->data);
  free(a);
  /* Helps to assert that the job has been done.*/
  *t = NULL;
}




/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data)
{
  mvmcell* cell;
  if(m == NULL || key == NULL || data == NULL){
    printf("Please pass valid pointers to mvm_insert\n");
    return;
  }
  cell = (mvmcell*) calloc(1,sizeof(mvmcell));
  if(cell == NULL){
    ON_ERROR("Failed to create multivalue map cell.\n");
  }
  /* Add 1 to strlen to account for NULL character */
  cell->key = (char*) calloc(1, strlen(key)+1);
  cell->value = (char*) calloc(1, strlen(data)+1);
  if(cell->key == NULL || cell->value == NULL){
    ON_ERROR("Failed to creat space for key/value pair.\n");
  }
  strcpy(cell->key, key);
  strcpy(cell->value, data);
  printf("before: %d\n", m->size);
  insert_hash(&m, cell);
  printf("after: %d\n", m->size);
  m->numkeys++;
}

void insert_hash(mvm** m, mvmcell* cell)
{
  long hash;
  mvm* new_m;
  mvm* temp;
  int i;

  hash = find_empty(*m, cell->key);
  if(hash != -1){
    cell->next = (*m)->data[hash];
    (*m)->data[hash] = cell;
  }else{
    new_m = mvm_init();
    resize(new_m, (*m)->size);
    for(i = 0; i < (*m)->size; i++){
      if((*m)->data[i] != NULL){
        insert_hash(&new_m, (*m)->data[i]);
      }
    }
    insert_hash(&new_m, cell);
    temp = *m;
    *m = new_m;
    mvm_free(&temp);
  }
}




long find_empty(mvm* m, char* key)
{
  long hash;
  hash = hash1(key) % m->size;
  if(m->data[hash] == NULL){
    return hash;
  }
  if((strcmp((key), m->data[hash]->key) == 0)){
    return hash;
  }else{
    hash = (hash1(key) + hash2(key)) % m->size;
    if(m->data[hash] == NULL){
      return hash;
    }
    if((strcmp((key), m->data[hash]->key) == 0)){
      return hash;
    }
  }
  /* Not able to find empty hash for this cell. Resizing required. */
  return -1;
}



/* Can combine this with function above?? */
long find_hash(mvm* m, char* key)
{
  long hash;
  hash = hash1(key) % m->size;
  if(m->data[hash] == NULL){
    return -1;
  }
  if(strcmp((key), m->data[hash]->key) == 0){
    return hash;
  }else{
    hash = (hash1(key) + hash2(key)) % m->size;
    if(m->data[hash] == NULL){
      return -1;
    }
    if(strcmp((key), m->data[hash]->key) == 0){
      return hash;
    }
  }
  /* Not able to find hash for this cell. */
  return -1;
}



void resize(mvm* m, int s)
{
  int new_s;
  s = s*2;
  new_s = findNextPrime(s);

  m->data = (mvmcell**) calloc(new_s, sizeof(mvmcell*));
  m->size = new_s;
  m->numkeys = 0;
}






/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
  char* string;
  char* substring;
  mvmcell* current;
  int i;
  int length = str_len(m);
  if(m == NULL){
    return NULL;
  }
  string = (char*) calloc(length, sizeof(char));
  substring = (char*) calloc(SUBSIZE, sizeof(char));
  if(string == NULL || substring == NULL){
    ON_ERROR("Failed to print string.\n");
  }
  /* Loop through the array */
  for(i = 0; i < m->size; i++){
    if(m->data[i] != NULL){
      current = m->data[i];
      /* Loop through any linked cells */
      while(current != NULL){
        sprintf(substring, "[%s](%s) ", current->key, current->value);
        strcat(string, substring);
        current = current->next;
      }

    }
  }
  free(substring);
  return string;
}



/* calculates length of string to be printed */
int str_len(mvm* m)
{
  int i;
  mvmcell * current;
  int len = 0;
  if(m == NULL){
    return 0;
  }
  /* Loop through the array */
  for(i = 0; i < m->size; i++){
    if(m->data[i] != NULL){
      /* Loop through any linked cells */
      current = m->data[i];
      while(current != NULL){
        len += strlen(current->key) + strlen(current->value) + BRACKSPACE;
        current = current->next;
      }
    }
  }
  /* Add 1 onto len to account for NULL character. */
  len++;
  return len;
}



/* Remove one key/value */
void mvm_delete(mvm* m, char* key)
{
  long hash;
  mvmcell* temp;
  if(m == NULL || key == NULL){
    return;
  }
  hash = find_hash(m, key);
  if(hash == -1){
    printf("Not able to find and delete %s from MultiValue map.\n", key);
  }else{
    temp = m->data[hash];
    /* Redirect pointer to next value in the key linked list. If there is only
    one value then the pointer is set to NULL. */
    m->data[hash] = m->data[hash]->next;
    free_cell(&temp);
    m->numkeys--;
  }
}



/* Return single value for a key */
char* mvm_search(mvm* m, char* key)
{
  long hash;
  if(m == NULL || key == NULL){
    return NULL;
  }
  hash = find_hash(m, key);
  if(hash == -1){
    printf("Not able to find %s in MultiValue map.\n", key);
    return NULL;
  }else{
    return m->data[hash]->value;
  }
}



/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n)
{
  char** pointers;
  mvmcell* current;
  int count = 0;
  long hash;
  if(m == NULL || key == NULL || n == NULL){
    return NULL;
  }
  hash = find_hash(m, key);
  if(hash == -1){
    printf("Not able to find %s in MultiValue map.\n", key);
    return NULL;
  }
  current = m->data[hash];
  while(current != NULL){
    current = current->next;
    count++;
    }
  pointers = (char**) calloc(count, sizeof(char*));   /* IS NUMKEYS TOO BIG?? */
  if(pointers == NULL){
    ON_ERROR("Creation of multisearch Map Failed\n");
  }
  current = m->data[hash];
  count = 0;
  while(current != NULL){
    pointers[count] = current->value;
    count++;
    current = current->next;
  }
  *n = count;
  return pointers;
}
