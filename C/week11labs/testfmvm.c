#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fmvm.h"

int main(void)
{

   int i, j;
   mvm *m;
   char* str;
   char** av;
   char animals[5][10] = {"cat",  "dog",  "bird",  "horse", "frog"};
   char  noises[5][10] = {"meow", "bark", "tweet", "neigh", "croak"};
   long hash;

   printf("Basic MVM Tests ... Start\n");
   /* Set up empty array */
   m = mvm_init();

   assert(m != NULL);
   assert(mvm_size(m)==0);

   /* Building and Searching */
   for(j=0; j<5; j++){
      mvm_insert(m, animals[j], noises[j]);
      assert(mvm_size(m)==j+1);
      i = strcmp(mvm_search(m, animals[j]), noises[j]);
      assert(i==0);
   }


   /* Test building & printing */
   str = mvm_print(m);
   i = strcmp(str, "[bird](tweet) [cat](meow) [dog](bark) [horse](neigh) [frog](croak) ");
   assert(i==0);
   free(str);

   /* Search for non-existent key */
   /* No-one knows what the fox says ? */
   assert(mvm_search(m, "fox") == NULL);

   /* Deletions - middle, then front */
   mvm_delete(m, "dog");
   assert(mvm_size(m)==4);
   str = mvm_print(m);
   i = strcmp(str, "[bird](tweet) [cat](meow) [horse](neigh) [frog](croak) ");
   assert(i==0);
   free(str);
   mvm_delete(m, "frog");
   assert(mvm_size(m)==3);
   str = mvm_print(m);
   i = strcmp(str, "[bird](tweet) [cat](meow) [horse](neigh) ");
   assert(i==0);
   free(str);

   /* Insert Multiple Keys */
   mvm_insert(m, "frog", "croak");
   mvm_insert(m, "frog", "ribbit");
   assert(mvm_size(m)==5);
   str = mvm_print(m);
   i = strcmp(str, "[bird](tweet) [cat](meow) [horse](neigh) [frog](ribbit) [frog](croak) ");
   free(str);
   assert(i==0);

   /* Search Multiple Keys */
   str = mvm_search(m, "frog");
   i = strcmp(str, "ribbit");
   assert(i==0);
   /* Multisearching */

   av = mvm_multisearch(m, "cat", &i);
   assert(i==1);
   i = strcmp(av[0], "meow");
   assert(i==0);
   free(av);
   av = mvm_multisearch(m, "horse", &i);
   assert(i==1);
   i = strcmp(av[0], "neigh");
   assert(i==0);
   free(av);
   av = mvm_multisearch(m, "frog", &i);
   assert(i==2);
   i = strcmp(av[0], "ribbit");
   j = strcmp(av[1], "croak");
   assert((i==0)&&(j==0));
   free(av);

   /* Delete Multiple Keys */
   mvm_delete(m, "frog");
   assert(mvm_size(m)==4);
   mvm_delete(m, "frog");
   assert(mvm_size(m)==3);
   str = mvm_print(m);
   i = strcmp(str, "[bird](tweet) [cat](meow) [horse](neigh) ");
   assert(i==0);
   free(str);

   /* Weird NULL insert() edge cases */
   mvm_insert(m, NULL, "quack");
   assert(mvm_size(m)==3);
   mvm_insert(NULL, "duck", "quack");
   assert(mvm_size(m)==3);
   mvm_insert(m, "duck", NULL);
   assert(mvm_size(m)==3);

   /* Weird NULL delete() edge cases */
   mvm_delete(m, "");
   assert(mvm_size(m)==3);
   mvm_delete(m, NULL);
   assert(mvm_size(m)==3);
   mvm_delete(NULL, "frog");
   assert(mvm_size(m)==3);
   mvm_delete(m, "bird");
   assert(mvm_size(m)==2);
   str = mvm_print(m);
   i = strcmp(str, "[cat](meow) [horse](neigh) ");
   assert(i==0);
   free(str);



   /* Testing isPrime */
   assert(is_prime(1) == 0);
   assert(is_prime(2) == 1);
   assert(is_prime(0) == 0);
   assert(is_prime(7) == 1);
   assert(is_prime(197) == 1);
   assert(is_prime(198) == 0);
   assert(is_prime(199) == 1);
   assert(is_prime(-5) == 0);

   /* Testing findNextPrime */
   assert(find_next_prime(1) == 2);
   assert(find_next_prime(2) == 3);
   assert(find_next_prime(3) == 5);
   assert(find_next_prime(5) == 7);
   assert(find_next_prime(7) == 11);
   assert(find_next_prime(11) == 13);
   assert(find_next_prime(113) == 127);
   assert(find_next_prime(151) == 157);

   /* Freeing */
   mvm_free(&m);
   assert(m==NULL);
   assert(mvm_size(m)==0);


   /* Test table resize */
   m = mvm_init();
   assert(m->table->size == 53);
   resize(m->table, m->table->size);
   assert(m->table->size == 107);

   /* Testing find_hash function */
   hash = find_hash(m->table, animals[0], INSERT);
   assert(hash != -1);
   mvm_insert(m, animals[0], noises[0]);
   i = strcmp(animals[0], m->table->data[hash]->key);
   assert(i == 0);
   i = strcmp(noises[0], m->table->data[hash]->value);
   assert(i == 0);

   /* Tests resize table works with the data also being re-hashed to new position */
   resize_table(m);
   hash = find_hash(m->table, animals[0], INSERT);
   assert(hash != -1);
   i = strcmp(animals[0], m->table->data[hash]->key);
   assert(i == 0);
   i = strcmp(noises[0], m->table->data[hash]->value);
   assert(i == 0);

   /* Testing find_hash with SEARCH parameter */
   hash = find_hash(m->table, animals[0], SEARCH);
   assert(hash != -1);
   hash = find_hash(m->table, animals[1], SEARCH);
   assert(hash == -1);

   /* Freeing */
   mvm_free(&m);
   assert(m==NULL);
   assert(mvm_size(m)==0);


   printf("Basic MVM Tests ... Stop\n");
   return 0;
}
