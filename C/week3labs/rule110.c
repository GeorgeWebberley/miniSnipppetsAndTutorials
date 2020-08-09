/* A program that simulates rule110 pattern generating.
The user is asked whether they would like to use a randomly generated
line of 0s and 1s or a line with only a single 1 in the central
location. For this program the array size is length 31. */

#include <stdio.h>
#include <stdlib.h>

/* Length of line to print */
#define SIZE 91
/* Number of lines to print */
#define LINES 50

void print_array(int *new);
int * change_array(int *new);
int check_rule(int first, int second, int third);

int main(void)
{
  int *new, middle, i;
  new = (int *)calloc(SIZE, sizeof(int));
  middle = (SIZE - 1)/2;
  new[middle] = 1;
  print_array(new);
  for(i = 0; i < LINES; i++){
    new = change_array(new);
    print_array(new);
  }
  return 0;
}


void print_array(int *new)
{
  int i;
  printf("\n");
  for(i = 0; i < SIZE; i++){
    printf("%d", new[i]);
  }
}

int * change_array(int *new)
{
  int *old = new;
  int i;
  /* To represent the 3 integers directly above in the preceding list.
  When used with modulus it will start with the last digit in the array,
  the first and the second and then move on from there. */
  int mod1 = SIZE - 1, mod2 = SIZE, mod3 = SIZE + 1;
  int o_1, o_2, o_3;
  int new_num;
  new = (int *)calloc(SIZE, sizeof(int));
  for (i = 0; i < SIZE; i++, mod1++, mod2++, mod3++){
    o_1 = mod1 % SIZE;
    o_2 = mod2 % SIZE;
    o_3 = mod3 % SIZE;
    new_num = check_rule(old[o_1], old[o_2], old[o_3]);
    new[i] = new_num;
  }
  free(old);
  return new;
}

int check_rule(int first, int second, int third)
{
  if(first == 1 && second == 1 && third == 1){
    return 0;
  }
  if(first == 1 && second == 0 && third == 0){
    return 0;
  }
  if(first == 0 && second == 0 && third == 0){
    return 0;
  }
  return 1;
}
