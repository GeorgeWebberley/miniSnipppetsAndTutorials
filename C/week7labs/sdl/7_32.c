

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
#define LEN 9
#define RECTSIZE 100
#define MILLISECOND 750
#define BORDER 5


struct sequence{
  char grid[SIZE][SIZE];
  int x;
  int y;
  int parent;
  int path_flag;
};
typedef struct sequence sequence;


enum bool{false, true};
typedef enum bool bool;

void test(void);
bool checkValid(char *string);
void convertArray(char* string, sequence *queue, int position);
void findSpace(sequence *queue, int current);
void printArray(char grid[SIZE][SIZE]);
void findNext(sequence *queue, int current, int* end);
void swapPosition(sequence *queue, int current, int* end, int i, int j);
bool checkSolution(char grid[SIZE][SIZE]);
bool checkUsed(sequence *queue, char current[SIZE][SIZE], int end);
void recordPath(sequence *queue, int end);
void printPath(sequence *queue, int end);


int main(int argc, char** argv)
{
  static sequence queue[QLEN];
  int end = 0, current = 0;
  test();
  if(argc != 2){
    fprintf(stderr, "Incorrect usage. Please type %s followed by a string of numbers inside double inverted commas.\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  if(!checkValid(argv[1])){
    fprintf(stderr, "Please enter valid input. Make sure no integers are entered twice and that there is a space character.\n");
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

bool checkValid(char *string)
{
  /* Histogram is LEN - 1 as it will not include the ' ' character */
  int histogram[LEN-1];
  int i, space = 0;
  /* Initialise histogram to 0 ready for integers 1-8 */
  for(i = 0; i < LEN-1; i++){
    histogram[i] = 0;
  }
  /* Run through adding 1 to corresponding position in histogram. Also check for space character. */
  for(i = 0; i < LEN; i++){
    if(string[i] == ' '){
      space++;
    }else{
      if(string[i] < '1' || string[i] > '8'){
        return false;
      }
      histogram[string[i]-'1']++;
    }
  }
  /* If there is not exactly 1 ' ' character and one of each integer characters then return false */
  if(space != 1){
    return false;
  }
  for(i = 0; i < LEN-1; i++){
    if(histogram[i] != 1){
      return false;
    }
  }
  return true;
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
      swapPosition(queue, current, end, y, i);
    }
  }
  /*Check up-down movement (change in 'y').
  Loops from 'y-1' to 'y+1' excluding 'y'. */
  for(i = y - 1; i <= y + 1; i = i+2){
    if(i >= 0 && i <= 2){
      swapPosition(queue, current, end, i, x);
    }
  }
}

void swapPosition(sequence *queue, int current, int* end, int i, int j)
{
  int x = queue[current].x;
  int y = queue[current].y;
  queue[*end] = queue[current];
  queue[*end].grid[i][j] = queue[current].grid[y][x];
  queue[*end].grid[y][x] = queue[current].grid[i][j];
  queue[*end].parent = current;
  queue[*end].y = i;
  queue[*end].x = j;
  if(!checkUsed(queue, queue[*end].grid, *end)){
    if(checkSolution(queue[*end].grid)){
      recordPath(queue, *end);
      printPath(queue, *end);
    }else{
      *end = *end + 1;
    }
  }
}

/* Checks if current grid is the same as the 'final' grid */
bool checkSolution(char grid[SIZE][SIZE])
{
  char result[SIZE][SIZE] = {{'1', '2', '3'},
                             {'4', '5', '6'},
                             {'7', '8', ' '}};
  int i, j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      if(grid[i][j] != result[i][j]){
        return false;
      }
    }
  }
  return true;
}

/* Checks if the current grid has been used at all previously in the queue. */
bool checkUsed(sequence *queue, char current[SIZE][SIZE], int end)
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
      return true;
    }
  }
  /* If it reaches the end of the queue and it hasn't returned 1 then this grid
  has not yet been used. */
  return false;
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
  int i, j, k;
  SDL_Simplewin sw;
  SDL_Rect rectangle;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  int count = 0;
  char count_string[30];

  rectangle.w = RECTSIZE;
  rectangle.h = RECTSIZE;

  Neill_SDL_ReadFont(fontdata, "mode7.fnt");
  Neill_SDL_Init(&sw);


  for(k = 0; k <= end && !sw.finished; k++){
    if(queue[k].path_flag == 1){
      sprintf(count_string, "Count: %d", count);
      /* Set all squares to grey */
      for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
          /* Set colours based on i and j (40 given as a modifier for gradual blend) */
          Neill_SDL_SetDrawColour(&sw, i * 40, 200, i * 40);
          rectangle.x = j * (RECTSIZE + BORDER);
          rectangle.y = i * (RECTSIZE + BORDER);
          SDL_RenderFillRect(sw.renderer, &rectangle);
          Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
          Neill_SDL_DrawChar(&sw, fontdata, queue[k].grid[i][j], (j * RECTSIZE) + RECTSIZE/2, (i * RECTSIZE) + RECTSIZE/2);
        }
      }
      /*Set the space square to white */
      Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
      rectangle.x = queue[k].x * (RECTSIZE + BORDER);
      rectangle.y = queue[k].y * (RECTSIZE + BORDER);
      SDL_RenderFillRect(sw.renderer, &rectangle);
      Neill_SDL_DrawString(&sw, fontdata, count_string, 3*RECTSIZE, RECTSIZE);
      Neill_SDL_UpdateScreen(&sw);
      SDL_Delay(MILLISECOND);
      count++;
    }
    Neill_SDL_Events(&sw);
  }
  /* count - 1 to give the number of moves and not the number of boards. */
  sprintf(count_string, "Total Moves: %d", count-1);
  Neill_SDL_DrawString(&sw, fontdata, count_string, 3*RECTSIZE, RECTSIZE);
  Neill_SDL_UpdateScreen(&sw);
  SDL_Delay(3*MILLISECOND);
  SDL_Quit();
  atexit(SDL_Quit);

  exit(EXIT_SUCCESS);
}



void test(void)
{
  sequence queue[10];
  sequence queue2[10];
  int position = 0, end, next;
  char first_string[9] = "23614 758";
  char correct_string[9] = "12345678 ";
  char invalid[9] = "123456789";
  char invalid2[9] = "123 56788";

  char used1[3][3] = {{'1', '2', '3'},
                      {'4', '5', '6'},
                      {'7', '8', ' '}};
  char used2[3][3] = {{'2', '3', '6'},
                      {'1', '4', ' '},
                      {'7', '5', '8'}};
  char used3[3][3] = {{'1', '2', '3'},
                      {'4', '5', '6'},
                      {'7', '8', '9'}};
  char unused[3][3] = {{'4', '2', '3'},
                      {'4', '5', '6'},
                      {'7', '8', ' '}};


  char child1[3][3] = {{'1', '2', '3'},
                       {'4', '5', '6'},
                       {'7', ' ', '8'}};
  char child2[3][3] = {{'1', '2', '3'},
                       {'4', '5', ' '},
                       {'7', '8', '6'}};

  assert(checkValid(first_string) == true);
  assert(checkValid(correct_string) == true);
  assert(checkValid(invalid) == false);
  assert(checkValid(invalid2) == false);


/* Test convertArray, checkSolution and findSpace functions. */
  convertArray(first_string, queue, position);
  assert(queue[position].grid[0][0] == '2');
  assert(queue[position].grid[2][2] == '8');
  assert(queue[position].grid[0][2] == '6');
  assert(checkSolution(queue[position].grid) == 0);
  findSpace(queue, position);
  assert(queue[position].x == 2);
  assert(queue[position].y == 1);
  position++;

  convertArray(correct_string, queue, position);
  assert(queue[position].grid[0][0] == '1');
  assert(queue[position].grid[2][2] == ' ');
  assert(queue[position].grid[0][2] == '3');
  assert(checkSolution(queue[position].grid) == 1);
  findSpace(queue, position);
  assert(queue[position].x == 2);
  assert(queue[position].y == 2);
  position++;

  convertArray(invalid, queue, position);
  position++;
  convertArray(invalid2, queue, position);

  /* With 4 'boards' in the array, I now check the checkUsed function */
  assert(checkUsed(queue, used1, position) == 1);
  assert(checkUsed(queue, used2, position) == 1);
  assert(checkUsed(queue, used3, position) == 1);
  assert(checkUsed(queue, unused, position) == 0);

/* Will test swapPosition with "12345678 " */
  position = 1;
  next = position + 1;
  findSpace(queue, position);
  assert(queue[position].grid[2][2] == ' ');
  assert(queue[position].grid[2][1] == '8');
  swapPosition(queue, position, &next, queue[position].y, queue[position].x-1);
  assert(queue[position+1].grid[2][2] == '8');
  assert(queue[position+1].grid[2][1] == ' ');
  next = position + 1;
  swapPosition(queue, position, &next, queue[position].y-1, queue[position].x);
  assert(queue[position+1].grid[2][2] == '6');
  assert(queue[position+1].grid[1][2] == ' ');


/* Now that I have checked that checkUsed function works. I can use it to check that
findNext function works by knowing the children (and using a new, empty queue2) */
  position = end = 0;
  convertArray(correct_string, queue2, position);
  end++;
  findSpace(queue2, position);
  findNext(queue2, position, &end);
  assert(checkUsed(queue2, child1, end) == 1);
  assert(checkUsed(queue2, child2, end) == 1);
}
