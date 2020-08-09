/*Feedback from ge19548:
The code is easily readable, functions defined well,
approach works correctly. Maybe not enough testing for outlying cases, and also
perhaps validate the user input a little more thoroughly. For example abcdi
returns a sum of 601 even though it is not a roman numeral, and abc returns a
sum of 0 even though it is a roman numeral. No house style rules broken, well
indented and very readable.
60

Feedback from ap16989:
The code is consistent, easy to understand and respects the Coding House Style.
Nice way of dealing with an invalid input from the user, too.
The testing cases were sufficient and took into account the different
possibilities.
Overall: 65 - 70%.


A program that reads a roman numeral in the range of 1 - 3999
and returns the corresponding arabic integer. */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define NDEBUG
#include <assert.h>


int romanToArabic(char *roman);
int letterToNumber(char c);
void test(void);

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
      printf("The character '%c' is not a Roman numeral!\n", roman[i]);
    }
    /* Check if we can compare the current letter with the
     next letter in the string to see if subtraction is required. */
    if(roman[i+1] != '\0'){
      next = letterToNumber(roman[i + 1]);
      if(current < next){       /* subtraction required. */
        sum += next - current;
        i = i + 2;     /* Skips the next letter as it has already been used. */
      }else{           /* subtraction not required */
        sum += current;
        i++;
      }
    }else{
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
      printf("That is not a roman number! Please try again.\n");
      exit(1);
  }
  return number;
}

void tests(void)
{
  /* Testing letterToNumber function */
  assert(letterToNumber('C') == 100);
  assert(letterToNumber('i') == 1);
  assert(letterToNumber('K') == 0);
  /* Testing romanToArabic function */
  assert(romanToArabic("MDCCXXXII") == 1732);
  assert(romanToArabic("IV") == 4);
  assert(romanToArabic("MCMXCIX") == 1999);
}
