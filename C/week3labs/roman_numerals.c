/* A program that reads a roman numeral in the range of 1 - 3999
and returns the corresponding arabic integer */

#include <stdio.h>
#include <ctype.h>

int romanToArabic(char *roman);
int letterToNumber(char c);

int main(int argc, char **argv)
{
  if (argc == 2){
    printf("The roman numeral %s is equal to %d\n",\
    argv[1], romanToArabic(argv[1]));
  }
  else{
    printf("ERROR: Incorrect usage, try e.g. %s XXI\n", argv[0]);
  }
  return 0;
}

int romanToArabic(char *roman)
{
  int i = 0, sum = 0, current, next;
  while(roman[i] != '\0'){
    current = letterToNumber(roman[i]);
    if (current == 0){
      printf("The letter %c is not a Roman numeral!\n", roman[i]);
    }
    /* Checks if we can compare the current letter with the
     next letter to see if subtraction is required. */
    if(roman[i+1] != '\0'){
      next = letterToNumber(roman[i + 1]);
      if(current < next){       /* subtraction is required. */
        sum += next - current;
        i = i + 2;     /* Skips the next letter as it has already been used. */
      }
      else{
        sum += current;
        i++;
      }
    }
    else{
      /* The last letter in the string, not been used for subtraction. */
      sum += current;
      i++;
    }
  }
  return sum;
}

int letterToNumber(char c)
{
  int number;
  switch(toupper(c)){  /* To account for the user typing lower case letters */
    case 'I':
    number = 1;
    break;
    case 'V':
    number = 5;
    break;
    case 'X':
    number = 10;
    break;
    case 'L':
    number = 50;
    break;
    case 'C':
    number = 100;
    break;
    case 'D':
    number = 500;
    break;
    case 'M':
    number = 1000;
    break;
    /* If a non-roman letter is used then it will not be included in the sum */
    default:
    number = 0;
  }
  return number;
}
