/* A program that displays the initial number that creates the longest
hailstone sequence and also displays the initial number that leads to the largest
number appearing in the sequence. The initial number must be less than 50,000 */


#include <stdio.h>
#define MAXNUM 50000

int sequence_length(int a);
int largest_number(int a);


int main(void)
{
  int longest_initial = 1, largest_initial = 1, longest = 1, current_length, largest = 1, current_largest, i;
  for(i = 1; i < MAXNUM; i++){
    current_length = sequence_length(i);
    current_largest = largest_number(i);
    if(current_length > longest){
      longest_initial = i;
      longest = current_length;
    }
    if(current_largest > largest){
      largest_initial = i;
      largest = current_largest;
    }
  }
  printf("The number that creates the longest sequence is: %d\n", longest_initial);
  printf("The number that creates the larges number is: %d\n", largest_initial);
  return 0;
}


/* A function that returns the length of a hailstone sequence from an initial number */
int sequence_length(int a)
{
  int count = 1;
  while(a != 1){
    if(a % 2 == 0){  /* if the number is even then divide by 2 */
      a = a/2;
    }
    else{            /* if the number is odd then 3*n +1 */
      a = (3 * a) + 1;
    }
    count++;
  }
  return count;
}


/* A function that returns the largest number in the hailstone sequence */
int largest_number(int a)
{
  int largest = a;
  while(a != 1){
    if(a % 2 == 0){  /* if the number is even then divide by 2 */
      a = a/2;
    }
    else{            /* if the number is odd then 3*n +1 */
      a = (3 * a) + 1;
    }
    if(a > largest){
      largest = a;
    }
  }
  return largest;
}
