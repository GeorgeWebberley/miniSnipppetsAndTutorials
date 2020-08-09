/* A program that returns the soundex code of a name or word.
User inputs the name as a command line argument */

#include <stdio.h>
#include <ctype.h>
#define CODELENGTH 4
#define HW -1
#define VOWEL -2


void nameToSoundex(char *name, char *code);
void printCode(char *code);
int soundexCode(char c);

int main(int argc, char **argv)
{
  char code[CODELENGTH] = {0, 0, 0, 0};
  if(argc == 2){
    nameToSoundex(argv[1], code);
    printf("The soundex code for %s is equal to ",\
    argv[1]);
    printCode(code);
  }
  else{
    printf("ERROR: Please enter %s followed by a name (letters only).\n",\
    argv[0]);
  }
  return 0;
}

void nameToSoundex(char *name, char *code)
{
 /* A vowel flag is used to check if the previous letter was a vowel */
 int n_i = 0, c_i = 0, current, vowel_flag = 0, temp;
 /* Store the first letter in a temp so that it can be re accessed later */
 temp = name[n_i];
 /* Convert the first letter to a number. This number is not needed
 for the code but it is still needed to compare with the next number. */
 code[c_i] = soundexCode(name[n_i]);
 n_i++;
 c_i++;
 while (name[n_i] != '\0'){
   current = soundexCode(name[n_i]);
   if (current == VOWEL){
     vowel_flag = 1;
   }
   if(current != VOWEL && current != HW){
     if(vowel_flag == 0){
       if(current != code[c_i-1]){
         code[c_i] = current;
         c_i++;
       }
     }
     else{
/* if vowel flag is raised then it doesn't matter if the numbers are the same */
       code[c_i] = current;
       c_i++;
     }
     vowel_flag = 0;
   }
   if(current == VOWEL){
     vowel_flag = 1;
   }
   if(current == HW){
     vowel_flag = 0;
   }
   n_i++;
 }
 /* Re set the first letter in the code to the first letter of the name. */
 code[0] = temp;
}




int soundexCode(char c)
{
  int score;
  switch (toupper(c)){
    case 'B':
    case 'F':
    case 'P':
    case 'V':
      score = 1;
      break;
    case 'C':
    case 'G':
    case 'J':
    case 'K':
    case 'Q':
    case 'S':
    case 'X':
    case 'Z':
      score = 2;
      break;
    case 'D':
    case 'T':
      score = 3;
      break;
    case 'L':
      score = 4;
      break;
    case 'M':
    case 'N':
      score = 5;
      break;
    case 'R':
      score = 6;
      break;
    case 'H':
    case 'W':
      score = HW;
      break;
    default:
      score = VOWEL;
  }
  return score;
}


void printCode(char *code)
{
  int i = 0;
  printf("%c", toupper(code[i]));
  for(i = 1; i < CODELENGTH; i++){
    printf("%d", code[i]);
  }
  printf("\n");
}
