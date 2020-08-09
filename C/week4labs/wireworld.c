/* A program that simulates the cellular automation of a 2D grid of cells
in digital electronics. Each cell can be in one of 4 states: 'empty',
'electron head', 'electron tail' or 'copper' */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <time.h>

#define ROWS 40
#define COLS ROWS
#define GENS 1000
#define HEAD 'H'
#define TAIL 't'
#define COPPER 'c'
#define EMPTY ' '

void test(void);
void runWorld(FILE *file);
void printWire(char grid[ROWS][COLS]);
void convertArray(FILE *file, char grid[ROWS][COLS]);
int checkSurrounding(char grid[ROWS][COLS], int i, int j);
void changeArray(char gridA[ROWS][COLS], char gridB[ROWS][COLS]);

void delay(int milliseconds);




int main(int argc, char *argv[])
{
  FILE *wire_file;
  if (argc != 2){
    fprintf(stderr, "Correct usage is %s <file>.txt\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  wire_file = fopen(argv[1], "r");
  if (wire_file == NULL){
    fprintf(stderr, "Failed to open file '%s'. Please ensure file exists.\n",
    argv[1]);
    exit(EXIT_FAILURE);
  }
  runWorld(wire_file);
  fclose(wire_file);
  return 0;
}


void runWorld(FILE *file)
{
  char grid_a[ROWS][COLS], grid_b[ROWS][COLS];
  int i;
  convertArray(file, grid_a);
  printWire(grid_a);
  /* Each for-loop contains 2 generations. Therefore the number of loops is halved. */
  for(i = 0; i < GENS/2; i++){
    changeArray(grid_a, grid_b);
    printWire(grid_b);
    delay(100000);
    changeArray(grid_b, grid_a);
    printWire(grid_a);
    delay(100000);
  }
}

/* Converts the file into a 2D array and then checks there are a total of 1600 characters used. */
void convertArray(FILE *file, char grid[ROWS][COLS])
{
  int current, column = 0, row = 0, count = 0;
  /* Ensures we don't try to access outside the boundaries of the array
  (taking into account the new line character at the end of each line.) */
  while((current = getc(file)) != EOF && row <= ROWS && column <= COLS){
    if (current == '\n'){
      row++;
      column = 0;
    }
    if (current == HEAD || current == TAIL || current == COPPER || current == EMPTY){
      grid[row][column] = current;
      column++;
      count++;
    }
  }
  if (count != ROWS * COLS){
    printf("%d\n", count);
    fprintf(stderr, "Something went wrong. Check the board is valid (40 x 40 and only contains 'H', 't', 'c' and ' ').\n");
    exit(EXIT_FAILURE);
  }
}


void printWire(char grid[ROWS][COLS])
{
  int r, c;
  for(r = 0; r < ROWS; r++){
    for(c = 0; c < COLS; c++){
      printf("%c", grid[r][c]);
    }
    printf("\n");
  }
}


void changeArray(char current[ROWS][COLS], char next[ROWS][COLS])
{
  int r, c, num;
  for(r = 0; r < ROWS; r++){
    for(c = 0; c < COLS; c++){
      if(current[r][c] == EMPTY){
        next[r][c] = EMPTY;
      }
      if(current[r][c] == HEAD){
        next[r][c] = TAIL;
      }
      if(current[r][c] == TAIL){
        next[r][c] = COPPER;
      }
      if(current[r][c] == COPPER){
        num = checkSurrounding(current, r, c);
        if(num == 1 || num == 2){
          next[r][c] = HEAD;
        }else{
          next[r][c] = COPPER;
        }
      }
    }
  }
}

/*
int newLetter(char grid[ROWS][COLS], int y, int x)
{
  switch(grid[y][x]){
    case EMPTY:
      return EMPTY;
    case HEAD:
      return TAIL;
    case TAIL:
      return COPPER;
    case COPPER:
      if(checkSurrounding(grid, y, x) == 1 || checkSurrounding(grid, y, x) == 2){
        return HEAD;
      }else{
        return COPPER;
      }
    return 0;
}
*/

int checkSurrounding(char grid[ROWS][COLS], int y, int x)
{
  int count = 0;
  int i, j;
  for(i = y-1; i <= y+1; i++){
    for(j = x-1; j <= x+1; j++){
      /* Check to ensure not going off end of array */
      if(j >= 0 && j < ROWS && i >= 0 && i < COLS){
        if(grid[i][j] == HEAD){
          count++;
        }
      }
    }
  }
  return count;
}


void test(void)
{
  char test_grid[ROWS][COLS] = {{' ', 't', ' ', 'c'},
                                {' ', 'H', 'c', 'c'},
                                {'H', 'c', 'c', 't'},
                                {'c', 'H', '-', 'p'}};
  assert(checkSurrounding(test_grid, 2, 1) == 3);
  assert(checkSurrounding(test_grid, 1, 2) == 1);
  assert(checkSurrounding(test_grid, 3, 0) == 2);
  assert(checkSurrounding(test_grid, 0, 3) == 0);


}



void delay(int milliseconds)
{

    clock_t start_time = clock();

    while (clock() < start_time + milliseconds);
}
