#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mvm.h"

/* BRACKSPACE adds space for the brackets and spaces required for the print function */
#define BRACKSPACE 5
/* Space for the 'substring' when building a string in the print function */
#define SUBSIZE 30


/* Helper function to free individual cell. */
void free_cell(mvmcell** cell);
/* Helper function to calculate length of string to be printed. */
int str_len(mvm* m);

mvm* mvm_init(void)
{
  mvm* map;
  map = (mvm*) calloc(1,sizeof(mvm));
  if(map == NULL){
     ON_ERROR("Creation of MultiValue Map Failed\n");
  }
  map->numkeys = 0;
  map->head = NULL;
  return map;
}


/* Number of key/value pairs stored */
int mvm_size(mvm* m)
{
  if(m == NULL){
    return 0;
  }
  return m->numkeys;
}


/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data)
{
  mvmcell* cell;
  if(m == NULL || key == NULL || data == NULL){
    return;
  }
  cell = (mvmcell*) calloc(1,sizeof(mvmcell));
  if(cell == NULL){
    ON_ERROR("Failed to create multivalue map cell.");
  }
  /* Add 1 to strlen to account for NULL character */
  cell->key = (char*) calloc(1, strlen(key)+1);
  cell->data = (char*) calloc(1, strlen(data)+1);
  if(cell->key == NULL || cell->data == NULL){
    ON_ERROR("Failed to create space for key/value pair.");
  }
  strcpy(cell->key, key);
  strcpy(cell->data, data);
  /* Insert at start of linked list */
  cell->next = m->head;
  m->head = cell;
  m->numkeys++;
}


/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
  char* string;
  char* substring;
  mvmcell* current = m->head;
  int length = str_len(m);
  if(m == NULL){
    return NULL;
  }
  string = (char*) calloc(length, sizeof(char));
  substring = (char*) calloc(SUBSIZE, sizeof(char));
  if(string == NULL || substring == NULL){
    ON_ERROR("Failed to print string.");
  }
  while(current != NULL){
    sprintf(substring, "[%s](%s) ", current->key, current->data);
    strcat(string, substring);
    current = current->next;
  }
  free(substring);
  return string;
}

/* calculates length of string to be printed */
int str_len(mvm* m)
{
  int len = 0;
  mvmcell* current = m->head;
  if(m == NULL){
    return 0;
  }
  while(current != NULL){
    len += strlen(current->key) + strlen(current->data) + BRACKSPACE;
    current = current->next;
  }
  /* Add 1 onto len to account for NULL character. */
  len++;
  return len;
}

/* Remove one key/value */
void mvm_delete(mvm* m, char* key)
{
  mvmcell* prev;
  mvmcell* current;
  if(m == NULL || key == NULL){
    return;
  }
  prev = current = m->head;
  /* Check first cell in linked list. */
  if(strcmp(current->key, key) == 0){
    m->head = m->head->next;
    free_cell(&current);
    m->numkeys--;
  /* else loop through other cells */
  }else{
    current = current->next;
    while(current != NULL){
      if(strcmp(current->key, key) == 0){
        /* Unlink 'current' from the chain and free it. */
        prev->next = current->next;
        free_cell(&current);
        m->numkeys--;
        return;
      }
      prev = current;
      current = current->next;
    }
  }
}


/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key)
{
  mvmcell* current;
  if(m == NULL || key == NULL){
    return NULL;
  }
  current = m->head;
  while(current != NULL){
    if(strcmp(current->key, key) == 0){
      return current->data;
    }
    current = current->next;
  }
  /* Reached end of list without finding key, therefore NULL is returned */
  return NULL;
}



/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n)
{
  char** pointers;
  mvmcell* current;
  int count = 0;
  if(m == NULL || key == NULL || n == NULL){
    return NULL;
  }
  /* Calloc enough space for 'pointers' in worse case scenario of all cells having the same key */
  pointers = (char**) calloc(m->numkeys, sizeof(char*));
  if(pointers == NULL){
    ON_ERROR("Creation of multisearch pointer array failed\n");
  }
  current = m->head;
  while(current != NULL){
    if(strcmp(current->key, key) == 0){
      pointers[count] = current->data;
      count++;
    }
    current = current->next;
  }
  *n = count;
  return pointers;
}


void free_cell(mvmcell** cell)
{
  mvmcell* c = *cell;
  if(cell == NULL){
    return;
  }
  free(c->data);
  free(c->key);
  free(c);
  *cell = NULL;
}


/* Free & set p to NULL */
void mvm_free(mvm** p)
{
  mvmcell* current;
  mvmcell* prev;
  mvm* m = *p;
  if(p == NULL){
    return;
  }
  prev = current = (*p)->head;
  if(current == NULL){
    free(m);
    *p = NULL;
    return;
  }else{
    current = current->next;
    while(current != NULL){
      free_cell(&prev);
      prev = current;
      current = current->next;
    }
  }
  /* Free final cell and the map itself. */
  free_cell(&prev);
  free(m);
  *p = NULL;
}
