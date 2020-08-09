/* A program that asks the user to input a phrase.
The program returns whether the phrase is a palindrome
or not. */

#include <stdio.h>
#include <ctype.h>

#define MAXLINE 100

enum bool{false, true};
typedef enum bool bool;

int type_phrase(char string[]);
bool check_pal(int last, char string[]);

int main(void)
{
  char pal[MAXLINE];
  int last;
  bool result;
  printf("Please type in sentence to be checked for palindrome\n");
  last = type_phrase(pal);
  result = check_pal(last, pal);
  if (result == true){
    printf("That is a palindrome!\n");
  }
  else{
    printf("That is not a palindrome!\n");
  }
  return 0;
}



int type_phrase(char string[])
{
  int c, i = 0;

  while((c = getchar()) != EOF && c != '\n'){
    if(isalpha(c)){
       string[i] = c;
       i++;
    }
  }
  string[i] = '\0';
  return i - 1;  /* This returns the position of the last letter in the string (not including the null char)*/
}


bool check_pal(int last, char string[])
{
  int i;
  for(i = 0; i <= last; i++, last--){
    printf("Comparing %c and %c\n", string[i], string[last]);
    if(tolower(string[i]) != tolower(string[last])){
      return false;
    }
  }
  return true;
}
