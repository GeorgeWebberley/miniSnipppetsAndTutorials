/* A program that simulates an 8-tile puzzle. A user can input an 8-tile board
as a string (e.g. "123 54678") and the program will print out the correct
sequence of moves needed to solve the problem or return with failure if it is not
possible. */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define SIZE 3
#define QLEN 300000
#define MAX 300000

struct sequence{
  char grid[SIZE][SIZE];
  int x;
  int y;
  int parent;
  int path;
};
typedef struct sequence sequence;


void test(void);
void convertArray(char* string, sequence *queue, int position);
void findSpace(sequence *queue, int current);
void printArray(char grid[SIZE][SIZE]);
void findNext(sequence *queue, int current, int* end);
int checkSolution(char grid[SIZE][SIZE]);
int checkUsed(sequence *queue, char current[SIZE][SIZE], int end);
void recordPath(sequence *queue, int end);
void printPath(sequence *queue, int end);



int main(int argc, char** argv)
{
  static sequence queue[QLEN];
  int end = 0, current = 0;
  test();
  if(argc != 2){
    fprintf(stderr, "Incorrect usage. Please type %s followed by a string of numbers inside double inverted commas.\n", argv[0]);
  }
  convertArray(argv[1], queue, end);
  end++;
  findSpace(queue, current);
  while(end < MAX){
    findNext(queue, current, &end);
    current++;
  }
  printf("Unable to find a solution for the string '%s'.\n", argv[1]);
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
  printf("------------\n");
}

/* Finds where the space character is in a 3x3 grid (counting from '1' to '3'
on the top row, '4' to '6' along middle row and '7' to '9' along bottom row) */
void findSpace(sequence *queue, int current)
{
  int i, j;
  queue[current].x = -1;
  queue[current].y = -1;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      if(queue[current].grid[i][j] == ' '){
        queue[current].y = i;
        queue[current].x = j;
      }
    }
  }
}

void findNext(sequence *queue, int current, int* end)
{
  int x = queue[current].x;
  int y = queue[current].y;
  /*Check if space can 'move' upwards. */
  if(queue[current].y - 1 >= 0){
    queue[*end] = queue[current];
    queue[*end].grid[y][x] = queue[current].grid[y-1][x];
    queue[*end].grid[y-1][x] = queue[current].grid[y][x];
    queue[*end].parent = current;
    queue[*end].y--;
    if(!checkUsed(queue, queue[*end].grid, *end)){
      if(checkSolution(queue[*end].grid)){
        recordPath(queue, *end);
        printPath(queue, *end);
      }else{
        *end = *end + 1;
      }
    }
  }
  /*Check if space can 'move' right. */
  if(queue[current].x + 1 <= 2){
    queue[*end] = queue[current];
    queue[*end].grid[y][x] = queue[current].grid[y][x+1];
    queue[*end].grid[y][x+1] = queue[current].grid[y][x];
    queue[*end].parent = current;
    queue[*end].x++;
    if(!checkUsed(queue, queue[*end].grid, *end)){
      if(checkSolution(queue[*end].grid)){
        recordPath(queue, *end);
        printPath(queue, *end);
      }else{
        *end = *end + 1;
      }
    }
  }
  /*Check if space can 'move' downards. */
  if(queue[current].y + 1 <= 2){
    queue[*end] = queue[current];
    queue[*end].grid[y][x] = queue[current].grid[y+1][x];
    queue[*end].grid[y+1][x] = queue[current].grid[y][x];
    queue[*end].parent = current;
    queue[*end].y++;
    if(!checkUsed(queue, queue[*end].grid, *end)){
      if(checkSolution(queue[*end].grid)){
        recordPath(queue, *end);
        printPath(queue, *end);
      }else{
        *end = *end + 1;
      }
    }
  }
  /*Check if space can 'move' left. */
  if(queue[current].x - 1 >= 0){
    queue[*end] = queue[current];
    queue[*end].grid[y][x] = queue[current].grid[y][x-1];
    queue[*end].grid[y][x-1] = queue[current].grid[y][x];
    queue[*end].parent = current;
    queue[*end].x--;
    if(!checkUsed(queue, queue[*end].grid, *end)){
      if(checkSolution(queue[*end].grid)){
        recordPath(queue, *end);
        printPath(queue, *end);
      }else{
        *end = *end + 1;
      }
    }
  }
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







void recordPath(sequence *queue, int end)
{
  int current, i;
  /* first need to initialise all path-flags to 0 */
  for(i = 0; i < end; i++){
    queue[i].path = 0;
  }
  /*Then need to loop back through the 'parent' elements setting path-flags to 1 */
  current = end;
  while(current != 0){
    queue[current].path = 1;
    current = queue[current].parent;
  }
  queue[0].path = 1;
}

void printPath(sequence *queue, int end)
{
  int i;
  for(i = 0; i <= end; i++){
    if(queue[i].path == 1){
      printArray(queue[i].grid);
    }
  }
  exit(EXIT_SUCCESS);
}


void test(void)
{
  static sequence queue[QLEN];
/*  char grid[SIZE][SIZE];
  char next[SIZE][SIZE]; */
  char test_string[9] = "23614 758";
  char test_string2[9] = "1234567 8";
  /*
  char used[3][3] = {{'1', '2', '3'},
                     {'4', '5', '6'},
                     {'7', '8', ' '}};

  char unused[3][3] = {{'5', '2', '3'},
                       {'4', '1', '6'},
                       {'7', '8', ' '}};
  */
  int end = 0, current = 0;


  /*int i;*/

  convertArray(test_string, queue, end);
  end++;
  convertArray(test_string2, queue, end);
  end++;
  /*
  assert(queue[current].grid[1][1] == '5');
  assert(queue[current].grid[2][2] == ' ');
  assert(queue[current].grid[2][0] == '7');
  printArray(queue[current].grid);*/

  findSpace(queue, current);
  assert(queue[current].x == 2);
  assert(queue[current].y == 1);

  findSpace(queue, current+1);
  assert(queue[current+1].x == 1);
  assert(queue[current+1].y == 2);

  /*assert(queue[current].space == 9);
  queue[current+1].space = findSpace(queue[current+1].grid);
  assert(queue[current+1].space == 8);

  assert(checkSolution(queue[current].grid) == 1);
  assert(checkSolution(queue[current+1].grid) == 0);

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


  while(1){
    findNext(queue, current, &end, queue[current].space);
    current++;
  }
  */
}
