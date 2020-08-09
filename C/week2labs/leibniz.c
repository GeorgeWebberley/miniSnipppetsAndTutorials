/* A program that calculates Pi using the equation:
π = 4 - 4/3 + 4/5 - 4/7 + 4/9 - 4/11 + ... */

#include <stdio.h>
#define ITERATIONS 100
#define PLUS 1
#define MINUS 0


int main(void)
{
  int i, prev_sign = MINUS;
  float sum = 0;
  for(i = 1; i < ITERATIONS * 2; i += 2){   /* ITERATIONS is multiplied by 2 as we are incrementing in 2s, not 1s. */
    if (prev_sign == MINUS){
      sum += 4.0/i;
      prev_sign = PLUS;
    }
    else{
      sum -= 4.0/i;
      prev_sign = MINUS;
    }

    printf("Pi: %f\n", sum);
  }
  return 0;
}
