/* A game where the user guesses a number between 1 and 1000.
The game informs the user if the guess was too high or too low.
The user has 10 attempts to guess before they lose */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define WIN 1
#define LOSE 0


int random_number(void);
void higher_lower(int guess, int random);


int main(void)
{
  int random, guess = 0, count = 0, result = LOSE;
  random = random_number();
  while (result == LOSE && count < 10){
    printf("Please enter an integer between 1 and 1000.\n");
    printf("You have %d guesses remaining\n", 10 - count);
    scanf("%d", &guess);
    if(guess == random){
      result = WIN;
    }
    else{
      higher_lower(guess, random);
    }
    count++;
  }
  if(result == LOSE){
    printf("\nYou have ran out of guesses. The correct number was %d\n", random);
  }
  else{
    printf("\nCorrect! You guessed the number %d in %d guesses\n", random, count);
  }
  return 0;
}



int random_number(void)
{
  int random;
  srand(time(0));
  random = (rand() % 1000); /* generates a number from 0 to 999 */
  random++; /* Changes the number to be between 1 and 1000 */
  return random;
}

void higher_lower(int guess, int random)
{
  if(guess > random){
    printf("\nYour guess is too high!\n");
  }
  else{
    printf("\nYour guess is too low!\n");
  }
}
