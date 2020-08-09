/* A program that reorders an array of integers in place */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void re_order(int list[]);
void print_list(int list[]);
int random_number(void);

int main(void)
{
  int list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  srand(time(0));
  printf("Original list:\n");
  print_list(list);
  re_order(list);
  printf("\n\nAfter reorder:\n");
  print_list(list);
  return 0;
}

void re_order(int *list)
{
  int i, temp, random;
  for(i = 0; i < 10; i++){
    random = random_number();
    temp = list[i];
    list[i] = list[random];
    list[random] = temp;
  }
}


int random_number(void)
{
  int random;
  random = rand() % 10; /* Generates a number from 0-9 */
  return random;
}

void print_list(int *list)
{
  int i;
  for(i = 0; i < 10; i++){
    printf("%d ", list[i]);
  }
}
