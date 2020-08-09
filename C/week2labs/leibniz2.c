/* A program that calculates Pi using the equation:
π = 4 - 4/3 + 4/5 - 4/7 + 4/9 - 4/11 + ... */

#include <stdio.h>
#define PLUS 1
#define MINUS 0
#define PI_9 3.141592653
#define ACCURACY 0.000001

int main(void)
{
  int i = 1, prev_sign = MINUS, count = 0;
  double sum = 0;
  while((sum - PI_9) > ACCURACY || (sum - PI_9) < -ACCURACY){
    if (prev_sign == MINUS){
      sum += 4.0/i;
      prev_sign = PLUS;
    }
    else{
      sum -= 4.0/i;
      prev_sign = MINUS;
    }
    i += 2;
    count += 1;
    printf("%d. Pi: %.9f\n", count, sum);
  }
  return 0;
}
