/* A program that analyses a microwave with 3 times (10 minutes, 1 minute and 10 seconds).
It computes the least amount of buttons required to push for a given user input of time in seconds */

#include <stdio.h>

/* The microwave has 3 buttons */
#define BUTTONS 3
#define MINUTE 60

int number_buttons(int *microwave, int input);

int main(void)
{
  int microwave[BUTTONS] = {10 * MINUTE, 1 * MINUTE, 10};
  int input, answer;
  printf("Please enter the time required in seconds\n");
  scanf("%d", &input);
  answer = number_buttons(&microwave[0], input);
  printf("Number of button presses = %d\n", answer);
  return 0;
}

int number_buttons(int *microwave, int input)
{
  int count = 0;
  int total = 0;
  int i = 0;
  while(total < input){
    while (i < BUTTONS){
      if (input > microwave[i]){
        total += microwave[i];
        count++;
      }
    i++;
    }
  }
  return count;
}
