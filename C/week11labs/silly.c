#include <stdio.h>

int main(void)
{
  int j = 0;
  for(int i=0; i<200000; i++)
  {
    j += i;
  }
  return 0;
}
