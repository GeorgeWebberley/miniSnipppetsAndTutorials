/* Checking that the nth triangle number is the sum
of the numbers from 1 to n.
Then comparing this to the nth triangle number based
on the equation Tn = n*(n+1)/2 */

#include <stdio.h>
#define NUMBER 20

int triangles_recursive(int a, int sum, int i);
void triangles_iterative(int a, int sum, int i);
void equation(int a);
/*
int equation(int a);
*/

int main(void)
{
  int sum = 0, i = 1;
  printf("\nUsing a Recursive function\n");
  triangles_recursive(NUMBER, sum, i);
  printf("\nUsing an iterative function\n");
  triangles_iterative(NUMBER, sum, i);
  printf("\nUsing the equation Tn = n * (n + 1) / 2\n");
  equation(NUMBER);
  return 0;
}


/* Recursive function */
int triangles_recursive(int a, int sum, int i)
{
  while (i <= a){
    sum += i;
    printf("%d. %d\n", i, sum);
    i++;
    return triangles_recursive(a, sum, i);
  }
  return 1;
}

/* Iterative function */
void triangles_iterative(int a, int sum, int i)
{
  for (i = 1; i <= a; i++){
    sum += i;
    printf("%d. %d\n", i, sum);
  }
}

void equation(int a)
{
  printf("The triangle number for %d is: ", a);
  a = a*(a+1)/2;
  printf("%d\n", a);
}
