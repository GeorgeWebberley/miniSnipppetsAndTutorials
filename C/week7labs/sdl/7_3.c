/* A program that simulates an 8-tile puzzle. A user can input an 8-tile board
as a string (e.g. "123 54678") and the program will print out the correct
sequence of moves needed to solve the problem or return with failure if it is not
possible. */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "neillsdl2.h"

#define SIZE 3
#define QLEN 300000
#define RECTSIZE 60
#define MILLISECONDDELAY 200


struct sequence{
  char grid[SIZE][SIZE];
  int x;
  int y;
  int parent;
  int path_flag;
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
  /*test(); */
  if(argc != 2){
    fprintf(stderr, "Incorrect usage. Please type %s followed by a string of numbers inside double inverted commas.\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  convertArray(argv[1], queue, end);
  end++;
  findSpace(queue, current);
  do{
    findNext(queue, current, &end);
    current++;
  }while(current != end);
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

/* Finds the x and y coordinates of the 'space' tile in the 3/3 used_grid
(top left corner is 0-0, bottom right corner is 2-2). */
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
  int i;
  /*Check right-left movement (change in 'x').
  Loops from 'x-1' to 'x+1' excluding 'x'. */
  for(i = x - 1; i <= x + 1; i = i+2){
    if(i >= 0 && i <= 2){
      queue[*end] = queue[current];
      queue[*end].grid[y][i] = queue[current].grid[y][x];
      queue[*end].grid[y][x] = queue[current].grid[y][i];
      queue[*end].parent = current;
      queue[*end].x = i;
      /* If not already used it is added to the queue. If it is already used the queue
      is not advanced and the current struct will be overwritten by the next struct. */
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
  /*Check up-down movement (change in 'y').
  Loops from 'y-1' to 'y+1' excluding 'y'. */
  for(i = y - 1; i <= y + 1; i = i+2){
    if(i >= 0 && i <= 2){
      queue[*end] = queue[current];
      queue[*end].grid[i][x] = queue[current].grid[y][x];
      queue[*end].grid[y][x] = queue[current].grid[i][x];
      queue[*end].parent = current;
      queue[*end].y = i;
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
  /* Loop through the queue */
  for(i = 0; i < end; i++){
    used_flag = 1;
    /* Loop through the grid */
    for(j = 0; j < SIZE; j++){
      for(k = 0; k < SIZE; k++){
        if(queue[i].grid[j][k] != current[j][k]){
          used_flag = 0;
        }
      }
    }
    /* If it reaches end of grid comparison and the flag is still raised then this
    grid has already been used. */
    if(used_flag == 1){
      return 1;
    }
  }
  /* If it reaches the end of the queue and it hasn't returned 1 then this grid
  has not yet been used. */
  return 0;
}

void recordPath(sequence *queue, int end)
{
  int current, i;
  /* first, ensure all path-flags are initialised to 0 */
  for(i = 0; i < end; i++){
    queue[i].path_flag = 0;
  }
  /*Then need to loop back through the 'parent' elements setting path-flags to 1 */
  current = end;
  while(current != 0){
    queue[current].path_flag = 1;
    current = queue[current].parent;
  }
  queue[0].path_flag = 1;
}

void printPath(sequence *queue, int end)
{
/*  int i, j, k;*/

  SDL_Simplewin sw;
  SDL_Rect rectangle_one;
  SDL_Rect rectangle_two;
  SDL_Rect rectangle_three;
  SDL_Rect rectangle_four;
  SDL_Rect rectangle_five;
  SDL_Rect rectangle_six;
  SDL_Rect rectangle_seven;
  SDL_Rect rectangle_eight;
  SDL_Rect rectangle_nine;
  Neill_SDL_Init(&sw);

  rectangle_one.w = RECTSIZE;
  rectangle_one.h = RECTSIZE;
  rectangle_two.w = RECTSIZE;
  rectangle_two.h = RECTSIZE;
  rectangle_three.w = RECTSIZE;
  rectangle_three.h = RECTSIZE;
  rectangle_four.w = RECTSIZE;
  rectangle_four.h = RECTSIZE;
  rectangle_five.w = RECTSIZE;
  rectangle_five.h = RECTSIZE;
  rectangle_six.w = RECTSIZE;
  rectangle_six.h = RECTSIZE;
  rectangle_seven.w = RECTSIZE;
  rectangle_seven.h = RECTSIZE;
  rectangle_eight.w = RECTSIZE;
  rectangle_eight.h = RECTSIZE;
  rectangle_nine.w = RECTSIZE;
  rectangle_nine.h = RECTSIZE;
  Neill_SDL_SetDrawColour(&sw, 0, 255, 0);
  rectangle_one.x = 0;
  rectangle_one.y = 0;
  SDL_RenderFillRect(sw.renderer, &rectangle_one);
  Neill_SDL_SetDrawColour(&sw, 40, 255, 40);
  rectangle_two.x = RECTSIZE;
  rectangle_two.y = 0;
  SDL_RenderFillRect(sw.renderer, &rectangle_two);
  Neill_SDL_SetDrawColour(&sw, 60, 255, 60);
  rectangle_three.x = 2*RECTSIZE;
  rectangle_three.y = 0;
  SDL_RenderFillRect(sw.renderer, &rectangle_three);
  Neill_SDL_SetDrawColour(&sw, 90, 255, 90);
  rectangle_four.x = 0;
  rectangle_four.y = RECTSIZE;
  SDL_RenderFillRect(sw.renderer, &rectangle_four);
  Neill_SDL_SetDrawColour(&sw, 120, 255, 120);
  rectangle_five.x = RECTSIZE;
  rectangle_five.y = RECTSIZE;
  SDL_RenderFillRect(sw.renderer, &rectangle_five);
  Neill_SDL_SetDrawColour(&sw, 150, 255, 150);
  rectangle_six.x = 2*RECTSIZE;
  rectangle_six.y = RECTSIZE;
  SDL_RenderFillRect(sw.renderer, &rectangle_six);
  Neill_SDL_SetDrawColour(&sw, 180, 255, 180);
  rectangle_seven.x = 0;
  rectangle_seven.y = 2*RECTSIZE;
  SDL_RenderFillRect(sw.renderer, &rectangle_seven);
  Neill_SDL_SetDrawColour(&sw, 210, 255, 210);
  rectangle_eight.x = RECTSIZE;
  rectangle_eight.y = 2*RECTSIZE;
  SDL_RenderFillRect(sw.renderer, &rectangle_eight);
  Neill_SDL_SetDrawColour(&sw, 240, 255, 240);
  rectangle_nine.x = 2*RECTSIZE;
  rectangle_nine.y = 2*RECTSIZE;
  SDL_RenderFillRect(sw.renderer, &rectangle_nine);
  Neill_SDL_UpdateScreen(&sw);

  SDL_Delay(1000);


/*
  for(i = 0; i <= end; i++){
    if(queue[i].path_flag == 1){
      for(j = 0; j < SIZE; j++){
        for(k = 0; k < SIZE; k++){
          printf("%c ", grid[j][k]);
        }
      }
    }
  }
*/

  exit(EXIT_SUCCESS);
}

void test(void)
{
  static sequence queue[QLEN];
/*  char grid[SIZE][SIZE];
  char next[SIZE][SIZE]; */
  char test_string[9] = "23614 758";
  char test_string2[9] = "1234567 8";
  char test_correct[9] = "12345678 ";

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
  convertArray(test_string2, queue, end);
  end++;
  convertArray(test_correct, queue, end);
  end++;

  assert(queue[current].grid[1][1] == '4');
  assert(queue[current].grid[2][2] == '8');
  assert(queue[current].grid[2][0] == '7');
  /*printArray(queue[current].grid);*/

  findSpace(queue, current);
  assert(queue[current].x == 2);
  assert(queue[current].y == 1);

  findSpace(queue, current+1);
  assert(queue[current+1].x == 1);
  assert(queue[current+1].y == 2);


  assert(checkSolution(queue[current].grid) == 0);
  assert(checkSolution(queue[current+1].grid) == 0);
  assert(checkSolution(queue[current+2].grid) == 1);


  assert(checkUsed(queue, used, end) == 1);
  assert(checkUsed(queue, unused, end) == 0);


  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);
  current++;
  findNext(queue, current, &end);

  for(i = 0; i < end; i++){
    printArray(queue[i].grid);
    printf("-------------\n");
  }




}
