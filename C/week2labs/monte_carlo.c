/* This program provides an approaching estimate for π
by representing darts thrown at a square of length r which contains
a quarter circle of radius r. The amount of darts that hit both the
square and the circle can be represented in the equation:
4hc/ha where ha is the entire square and hc is the circle */


#include <stdio.h>
#define RADIUS 1000

float pi_calc(int a, int b);

int main(void)
{
  int x, y; /* Representing the coordinates */
  int count = 0, count_circle = 0; /* Representing the count for number of darts and count for how many hit the circle */
  int square;
  float pi;
  square = RADIUS * RADIUS;
  for(x = 1; x < RADIUS; x++){
    for(y = 1; y < RADIUS; y++){
      count++;
      if((x*x) + (y*y) <= square){
        count_circle++;
      }
      pi = pi_calc(count_circle, count);
      printf("Pi: %f\n", pi);
    }
  }
  return 0;

}



float pi_calc(int a, int b)
{
  float pi;
  pi = 4 * (float)a/(float)b;
  return pi;
}
