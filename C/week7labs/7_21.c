/* A program that simulates an 8-tile puzzle. A user can input an 8-tile board
as a string (e.g. "123 54678") and the program will print out the correct
sequence of moves needed to solve the problem or return with failure if it is not
possible. */

#include <stdio.h>
#include <assert.h>

#define SIZE 3
#define QLEN 30000


struct sequence{
  char grid[SIZE][SIZE];
  int parent;
  int space;
/*  int path = 0; */
};
typedef struct sequence sequence;


void test(void);
void convertArray(char* string, sequence *queue, int position);
void printArray(char grid[SIZE][SIZE]);
void findNext(sequence *queue, int current, int *end, int space);
int checkSolution(char grid[SIZE][SIZE]);
int checkUsed(sequence *queue, char current[SIZE][SIZE], int end);


int main(void)
{
  test();


  return 0;
}

/* Converts the inital string into a 2d array to begin the queue. */
void convertArray(char* string, sequence *queue, int position)
{
  int i, j, s = 0;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++, s++){
      queue[position].grid[i][j] = string[s];
    }
  }
}

void printArray(char grid[SIZE][SIZE])
{
  int i, j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      printf("%c ", grid[i][j]);
    }
    printf("\n");
  }
}

/* Finds where the space character is in a 3x3 grid (counting from '1' to '3'
on the top row, '4' to '6' along middle row and '7' to '9' along bottom row) */
int findSpace(char grid[SIZE][SIZE])
{
  int i, j, count = 1;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      if(grid[i][j] != ' '){
        count++;
      }else{
        return count;
      }
    }
  }
  return 0;
}

/* Checks if current grid is the same as the 'final' grid */
int checkSolution(char grid[SIZE][SIZE])
{
  char result[SIZE][SIZE] = {{'1', '2', '3'},
                             {'4', '5', '6'},
                             {'7', '8', ' '}};
  int i, j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      if(grid[i][j] != result[i][j]){
        return 0;
      }
    }
  }
  return 1;
}

/* Checks if the current grid has been used at all previously in the queue. */
int checkUsed(sequence *queue, char current[SIZE][SIZE], int end)
{
  int i, j, k, used_flag;
  for(i = 0; i < end; i++){
    used_flag = 1;
    for(j = 0; j < SIZE; j++){
      for(k = 0; k < SIZE; k++){
        if(queue[i].grid[j][k] != current[j][k]){
          used_flag = 0;
        }
      }
    }
    /* If reaches end of grid comparison and the flag is still raised then this
    grid has already been used. */
    if(used_flag == 1){
      return 1;
    }
  }
  /* If it reaches the end of the queue and it hasn't return 1 then this grid
  has not yet been used. */
  return 0;
}





void findNext(sequence *queue, int current, int* end, int space)
{
  switch(space){
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[0][0] = queue[current].grid[0][1];
      queue[*end].grid[0][1] = queue[current].grid[0][0];
      queue[*end].parent = current;
      queue[*end].space = 2;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[0][0] = queue[current].grid[1][0];
      queue[*end].grid[1][0] = queue[current].grid[0][0];
      queue[*end].parent = current;
      queue[*end].space = 4;
      *end = *end + 1;
      break;
    case 2:
    /* Top middle, 3 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[0][0] = queue[current].grid[0][1];
      queue[*end].grid[0][1] = queue[current].grid[0][0];
      queue[*end].parent = current;
      queue[*end].space = 1;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[0][1] = queue[current].grid[1][1];
      queue[*end].grid[1][1] = queue[current].grid[0][1];
      queue[*end].parent = current;
      queue[*end].space = 5;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[0][1] = queue[current].grid[0][2];
      queue[*end].grid[0][2] = queue[current].grid[0][1];
      queue[*end].parent = current;
      queue[*end].space = 3;
      *end = *end + 1;
      break;
    case 3:
    /* Top right corner, 2 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[0][2] = queue[current].grid[0][1];
      queue[*end].grid[0][1] = queue[current].grid[0][2];
      queue[*end].parent = current;
      queue[*end].space = 2;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[0][2] = queue[current].grid[1][2];
      queue[*end].grid[1][2] = queue[current].grid[0][2];
      queue[*end].parent = current;
      queue[*end].space = 6;
      *end = *end + 1;
      break;
    case 4:
    /* Middle left, 3 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[1][0] = queue[current].grid[0][0];
      queue[*end].grid[0][0] = queue[current].grid[1][0];
      queue[*end].parent = current;
      queue[*end].space = 1;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[1][0] = queue[current].grid[1][1];
      queue[*end].grid[1][1] = queue[current].grid[1][0];
      queue[*end].parent = current;
      queue[*end].space = 5;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[1][0] = queue[current].grid[2][0];
      queue[*end].grid[2][0] = queue[current].grid[1][0];
      queue[*end].parent = current;
      queue[*end].space = 7;
      *end = *end + 1;
      break;
    case 5:
      /* Middle, 4 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[1][1] = queue[current].grid[0][1];
      queue[*end].grid[0][1] = queue[current].grid[1][1];
      queue[*end].parent = current;
      queue[*end].space = 2;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[1][1] = queue[current].grid[1][2];
      queue[*end].grid[1][2] = queue[current].grid[1][1];
      queue[*end].parent = current;
      queue[*end].space = 6;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[1][1] = queue[current].grid[2][1];
      queue[*end].grid[2][1] = queue[current].grid[1][1];
      queue[*end].parent = current;
      queue[*end].space = 8;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[1][1] = queue[current].grid[1][0];
      queue[*end].grid[1][0] = queue[current].grid[1][1];
      queue[*end].parent = current;
      queue[*end].space = 4;
      *end = *end + 1;
      break;
    case 6:
    /* Middle right, 3 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[1][2] = queue[current].grid[0][2];
      queue[*end].grid[0][2] = queue[current].grid[1][2];
      queue[*end].parent = current;
      queue[*end].space = 3;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[1][2] = queue[current].grid[1][1];
      queue[*end].grid[1][1] = queue[current].grid[1][2];
      queue[*end].parent = current;
      queue[*end].space = 5;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[1][2] = queue[current].grid[2][2];
      queue[*end].grid[2][2] = queue[current].grid[1][2];
      queue[*end].parent = current;
      queue[*end].space = 9;
      *end = *end + 1;
      break;
    case 7:
    /* Bottom left corner, 2 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[2][0] = queue[current].grid[1][0];
      queue[*end].grid[1][0] = queue[current].grid[2][0];
      queue[*end].parent = current;
      queue[*end].space = 4;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[2][0] = queue[current].grid[2][1];
      queue[*end].grid[2][1] = queue[current].grid[2][0];
      queue[*end].parent = current;
      queue[*end].space = 8;
      *end = *end + 1;
      break;
    case 8:
    /* Bottom middle, 3 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[2][1] = queue[current].grid[2][0];
      queue[*end].grid[2][0] = queue[current].grid[2][1];
      queue[*end].parent = current;
      queue[*end].space = 7;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[2][1] = queue[current].grid[1][1];
      queue[*end].grid[1][1] = queue[current].grid[2][1];
      queue[*end].parent = current;
      queue[*end].space = 5;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[2][1] = queue[current].grid[2][2];
      queue[*end].grid[2][2] = queue[current].grid[2][1];
      queue[*end].parent = current;
      queue[*end].space = 9;
      *end = *end + 1;
      break;
    case 9:
    /* Bottom right corner, 2 different movements possible added onto end of queue */
      queue[*end] = queue[current];
      queue[*end].grid[2][2] = queue[current].grid[1][2];
      queue[*end].grid[1][2] = queue[current].grid[2][2];
      queue[*end].parent = current;
      queue[*end].space = 6;
      *end = *end + 1;
      queue[*end] = queue[current];
      queue[*end].grid[2][2] = queue[current].grid[2][1];
      queue[*end].grid[2][1] = queue[current].grid[2][2];
      queue[*end].parent = current;
      queue[*end].space = 8;
      *end = *end + 1;
  }
}






void test(void)
{
  static sequence queue[QLEN];
/*  char grid[SIZE][SIZE];
  char next[SIZE][SIZE]; */
  char test_string[9] = "12345678 ";
  /*char test_string2[9] = "1234567 8";*/
  char used[3][3] = {{'1', '2', '3'},
                     {'4', '5', '6'},
                     {'7', '8', ' '}};

  char unused[3][3] = {{'5', '2', '3'},
                       {'4', '1', '6'},
                       {'7', '8', ' '}};

  int end = 0, current = 0;


  int i;

  convertArray(test_string, queue, end);
  end++;
  /*convertArray(test_string2, queue, end);
  end++;*/

  assert(queue[current].grid[1][1] == '5');
  assert(queue[current].grid[2][2] == ' ');
  assert(queue[current].grid[2][0] == '7');
  /*printArray(queue[current].grid);*/

  queue[current].space = findSpace(queue[current].grid);
  assert(queue[current].space == 9);
  /*queue[current+1].space = findSpace(queue[current+1].grid);
  assert(queue[current+1].space == 8);*/

  assert(checkSolution(queue[current].grid) == 1);
  /*assert(checkSolution(queue[current+1].grid) == 0);*/

  assert(checkUsed(queue, used, end) == 1);
  assert(checkUsed(queue, unused, end) == 0);


  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);
  current++;
  findNext(queue, current, &end, queue[current].space);

  for(i = 0; i < end; i++){
    printArray(queue[i].grid);
    printf("-------------\n");
  }
}
