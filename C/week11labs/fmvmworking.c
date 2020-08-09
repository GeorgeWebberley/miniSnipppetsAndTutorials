#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmvm.h"


#define BRACKSPACE 5
#define SUBSIZE 30
#define STARTSIZE 53

/* Helper function to calculate length of string to be printed. */
int str_len(h_table* m);
/* Various helper functions */
int isPrime(int n);
int findNextPrime(int n);
unsigned long hash1( char *str);
void insert_hash(h_table *t, mvmcell *cell);
void free_cell(mvmcell** cell);
void resize(h_table* table, int s);
long find_hash(h_table* m, char* key);
long find_empty(h_table* t, char* key);



mvm* mvm_init(void)
{
  mvm* m;

  m = (mvm *) calloc(1, sizeof(mvm));

  if(m == NULL){
     printf("Creation of Table Failed\n");
     exit(EXIT_FAILURE);
  }
  m->table = (h_table*) calloc(1, sizeof(h_table));
  m->table->data = (mvmcell**) calloc(STARTSIZE, sizeof(mvmcell*));
  if((m->table == NULL) || (m->table->data == NULL)){
     printf("Creation of Table Failed\n");
     exit(EXIT_FAILURE);
  }
  m->table->size = STARTSIZE;
  m->table->numkeys = 0;
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
  return m->table->numkeys;
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


void free_cell(mvmcell** cell)
{
  mvmcell* c = *cell;
  if(cell == NULL){
    return;
  }
  free(c->value);
  free(c->key);
  free(c);
  *cell = NULL;
}


void mvm_free(mvm** t)
{
  mvm* a = *t;
  mvmcell* prev;
  mvmcell* current;

  int i;
  printf("size: %d\n", (*t)->table->size);
  for(i = 0; i< (*t)->table->size; i++){
    if(((*t)->table->data)[i] != NULL){
      prev = current = (*t)->table->data[i];
      while(current != NULL){
        current = current->next;
        free_cell(&prev);
        prev = current;

      }
    }
  }
  free(a->table->data);
  free(a->table);
  free(a);
  /* Helps to assert that the job has been done.*/
  *t = NULL;
}




/*
mvmcell* prev;
mvmcell* current;


if(((*t)->table->data)[i] != NULL){
  prev = current = (*t)->table->data[i];
  while(current != NULL){
    current = current->next;
    free_cell(&prev);
    prev = current;
  }

*/









/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data)
{
  mvmcell* cell;
  mvm* new_m;
  int i;

  if(m == NULL || key == NULL || data == NULL){
    return;
  }
  cell = (mvmcell*) calloc(1,sizeof(mvmcell));
  if(cell == NULL){
    ON_ERROR("Failed to create multivalue map cell.\n");
  }
  cell->next = NULL;
  /* Add 1 to strlen to account for NULL character */
  cell->key = (char*) calloc(1, strlen(key)+1);
  cell->value = (char*) calloc(1, strlen(data)+1);
  if(cell->key == NULL || cell->value == NULL){
    ON_ERROR("Failed to creat space for key/value pair.\n");
  }
  strcpy(cell->key, key);
  strcpy(cell->value, data);
  insert_hash(m->table, cell);

  if((m->table->numkeys*100)/m->table->size > 70){
    new_m = mvm_init();
    resize(new_m->table, m->table->size);
    for(i = 0; i < m->table->size; i++){
      if(m->table->data[i] != NULL){
        insert_hash(new_m->table, m->table->data[i]);
      }
    }
    new_m->table->numkeys++;
    free(m->table->data);
    free(m->table);
    m->table = new_m->table;
    free(new_m);
  }
}

void insert_hash(h_table* t, mvmcell* cell)
{
  long hash;
  hash = find_empty(t, cell->key);
  cell->next = t->data[hash];
  t->data[hash] = cell;
  t->numkeys++;
}




long find_empty(h_table* t, char* key)
{
  long hash;
  hash = hash1(key) % t->size;
  while(t->data[hash] != NULL){
    if((strcmp(key, t->data[hash]->key) == 0)){
      return hash;
    }
    hash++;
    /*Wrap around end of table */
    hash = hash % t->size;
  }
  /* m->data[hash] is at an empty cell and the hash can be returned. */
  return hash;
}



/* Can combine this with function above?? */
long find_hash(h_table* m, char* key)
{
  long hash;
  hash = hash1(key) % m->size;
  while(m->data[hash] != NULL){

    if((strcmp((key), m->data[hash]->key) == 0)){
      return hash;
    }
    hash++;
    /*Wrap around end of table */
    hash = hash % m->size;
  }
  /* m->data[hash] is at an empty cell and therefore not able to find this key in table. */
  return -1;
}



void resize(h_table* t, int s)
{
  int new_s;

  s = s*2;
  new_s = findNextPrime(s);
  free(t->data);
  t->data = (mvmcell**) calloc(new_s, sizeof(mvmcell*));
  t->size = new_s;
  t->numkeys = 0;
}






/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
  char* string;
  char* substring;
  mvmcell* current;
  int i;
  int length = str_len(m->table);
  if(m == NULL){
    return NULL;
  }
  string = (char*) calloc(length, sizeof(char));
  substring = (char*) calloc(SUBSIZE, sizeof(char));
  if(string == NULL || substring == NULL){
    ON_ERROR("Failed to print string.\n");
  }
  /* Loop through the array */
  for(i = 0; i < m->table->size; i++){
    if(m->table->data[i] != NULL){
      current = m->table->data[i];
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
int str_len(h_table* t)
{
  int i;
  mvmcell * current;
  int len = 0;
  if(t == NULL){
    return 0;
  }
  /* Loop through the array */
  for(i = 0; i < t->size; i++){
    if(t->data[i] != NULL){
      /* Loop through any linked cells */
      current = t->data[i];
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
  hash = find_hash(m->table, key);
  if(hash == -1){
    return;
  }else{
    temp = m->table->data[hash];
    /* Redirect pointer to next value in the key linked list. If there is only
    one value then the pointer is set to NULL. */
    m->table->data[hash] = m->table->data[hash]->next;
    free_cell(&temp);
    m->table->numkeys--;
  }
}



/* Return single value for a key */
char* mvm_search(mvm* m, char* key)
{
  long hash;
  if(m == NULL || key == NULL){
    return NULL;
  }
  hash = find_hash(m->table, key);
  if(hash == -1){
    return NULL;
  }else{
    return m->table->data[hash]->value;
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
  hash = find_hash(m->table, key);
  if(hash == -1){
    return NULL;
  }
  current = m->table->data[hash];
  while(current != NULL){
    current = current->next;
    count++;
    }
  pointers = (char**) calloc(count, sizeof(char*));   /* IS NUMKEYS TOO BIG?? */
  if(pointers == NULL){
    ON_ERROR("Creation of multisearch Map Failed\n");
  }
  current = m->table->data[hash];
  count = 0;
  while(current != NULL){
    pointers[count] = current->value;
    count++;
    current = current->next;
  }
  *n = count;
  return pointers;
}
