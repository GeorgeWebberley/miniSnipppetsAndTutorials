/* A program that resembles a cash machine that can only
dispense money in increments of £20. If the user requests
an amount other than this the machine suggests values on
either side that it can dispense */

#include <stdio.h>
#include <assert.h>


int division_20(int a);
int find_larger(int a);
int find_smaller(int a);
void cash_machine(void);



int main(void)
{
  assert(find_larger(17) == 20);
  assert(find_larger(21) == 40);
  assert(find_larger(39) == 40);
  assert(find_larger(40) == 40);
  assert(find_smaller(39) == 20);
  assert(find_smaller(21) == 20);
  cash_machine();
  return 0;
}



void cash_machine(void)
{
  int input, smaller, larger;
  printf("How much money would you like?\n");
  scanf("%d", &input);
  if(division_20(input) == 1){
    printf("Ok, dispensing...\n");
  }
  else{
    smaller = find_smaller(input);
    larger = find_larger(input);
    printf("I can give you %d or %d, try again.\n", smaller, larger);
    cash_machine();
  }
  return;
}



int division_20(int a)
{
  if((a % 20) == 0){
    return 1;
  }
  return 0;
}



int find_larger(int a)
{
  int i;
  for(i = 0; i < 20; i++){
    if(division_20(a) != 1){
      a++;
    }
  }
  return a;
}



int find_smaller(int a)
{
  int i;
  for(i = 0; i < 20; i++){
    if(division_20(a) != 1){
      a--;
    }
  }
  return a;
}
