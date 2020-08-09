/* A program that asks the use to input an integer for the number
of songs on a playlist. The program then returns a randomised
list of numbers to represent the order of the songs to be played */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int random_number(int songs);
void print_list(int *p, int songs);



enum bool{true, false};
typedef enum bool bool;

int main(void)
{
  int songs, count = 0, i, *p;
  bool used;
  int random;
  srand(time(0));
  printf("How many songs required?\n");
  scanf("%d", &songs);
  p = (int *)calloc(songs, sizeof(int));
  while(count < songs){
    random = random_number(songs);
    used = false;
    for (i = 0; i < songs; i++){
      if(p[i] == random){
        used = true;
      }
    }
    if(used == false){
      printf("%d ", random);
      p[count] = random;
      count++;
    }
  }
  return 0;
}

int random_number(int songs)
{
  int random;
  random = rand() % songs; /* Generates a number from 0-9 */
  random++;             /* Makes the number between 1-10 */
  return random;
}
