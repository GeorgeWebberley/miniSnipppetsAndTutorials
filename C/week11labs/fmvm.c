/* I have created a dynamically sized hash table. After a single hash, the table index
increments one at a time until it finds either an empty space or a cell with the same key.
If the space is empty it is simply inserted. If the cell has the same key then it is
linked to this cell by a 'next' pointer.
Despite likely taking longer to 'build' the data in the table, this should improve on the time complexity
of searching to be closer to O(n) compared to complexity O(n) of looping through a linked list.
This is because the search only has to calculate the hash number which should take it to the correct
index in the hash table (or close to the correct index). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmvm.h"

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


int is_prime(int n)
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


int find_next_prime(int n)
{
  n++;
  while(!is_prime(n)){
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

/* Loops through hash table and any linked lists from the hashtable, freeing all cells. Finally frees the hash table and the MVM struct. */
void mvm_free(mvm** t)
{
  mvm* a = *t;
  mvmcell* prev;
  mvmcell* current;
  int i;
  /* Loop through table */
  for(i = 0; i < a->table->size; i++){
    if((a->table->data)[i] != NULL){
      prev = current = a->table->data[i];
      /* Loop through any linked lists from hash table */
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
  *t = NULL;
}


/* Create one key/value pair and send to 'insert_hash' to insert it. */
void mvm_insert(mvm* m, char* key, char* data)
{
  mvmcell* cell;
  if(m == NULL || key == NULL || data == NULL){
    return;
  }
  cell = (mvmcell*) malloc(sizeof(mvmcell));
  if(cell == NULL){
    ON_ERROR("Failed to create multivalue map cell.\n");
  }
  cell->next = NULL;
  /* Add 1 to strlen to account for NULL character */
  cell->key = (char*) calloc(1, strlen(key)+1);
  cell->value = (char*) calloc(1, strlen(data)+1);
  if(cell->key == NULL || cell->value == NULL){
    ON_ERROR("Failed to create space for key/value pair.\n");
  }
  strcpy(cell->key, key);
  strcpy(cell->value, data);
  insert_hash(m->table, cell);
  /* Resize table when 70% full */
  if((m->table->numkeys*100)/m->table->size > 70){
    resize_table(m);
  }
}


/* Generates hash number and inserts cell at hash location. */
void insert_hash(h_table* t, mvmcell* cell)
{
  long hash;
  hash = find_hash(t, cell->key, INSERT);
  cell->next = t->data[hash];
  t->data[hash] = cell;
  t->numkeys++;
}


/* Resizes table, loops through old table reinserting old cells */
void resize_table(mvm* m)
{
  mvm* new_m;
  int i;
  mvmcell* prev;
  mvmcell* current;

  new_m = mvm_init();
  resize(new_m->table, m->table->size);
  /* Loop through hash table */
  for(i = 0; i < m->table->size; i++){
    if(m->table->data[i] != NULL){
      prev = current = m->table->data[i];
      /* Loop through any linked lists from the hash table with the same key.
      This is needed because of the way collisions are inserted as a linked list.  */
      while(current != NULL){
        current = current->next;
        insert_hash(new_m->table, prev);
        prev = current;
      }
    }
  }
  /* Free old table and data before redirecting pointer. */
  free(m->table->data);
  free(m->table);
  m->table = new_m->table;
  free(new_m);
}


/* Doubles size and finds next prime */
void resize(h_table* t, int s)
{
  int new_s;

  s = s*2;
  new_s = find_next_prime(s);
  free(t->data);
  t->data = (mvmcell**) calloc(new_s, sizeof(mvmcell*));
  t->size = new_s;
  t->numkeys = 0;
}


/* Searches hash table for key or empty cell, depending on the job specified. */
long find_hash(h_table* t, char* key, int job)
{
  long hash;
  hash = hash1(key) % t->size;
  while(t->data[hash] != NULL){
    if((strcmp((key), t->data[hash]->key) == 0)){
      return hash;
    }
    hash++;
    /*Wrap around end of table */
    hash = hash % t->size;
  }
  /* m->data[hash] is at an empty cell and the value returned depends on the 'job' of find_hash. */
  if(job == INSERT){
    return hash;
  }else{  /* job must be equal to SEARCH */
    return -1;
  }
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
  hash = find_hash(m->table, key, SEARCH);
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
  hash = find_hash(m->table, key, SEARCH);
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
  hash = find_hash(m->table, key, SEARCH);
  if(hash == -1){
    return NULL;
  }
  current = m->table->data[hash];
  while(current != NULL){
    current = current->next;
    count++;
    }
  pointers = (char**) calloc(count, sizeof(char*));
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
