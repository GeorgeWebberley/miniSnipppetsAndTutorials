/* A program that simulates the cellular automation of a 2D grid of cells
in digital electronics. Each cell can be in one of 4 states: 'empty',
'electron head', 'electron tail' or 'copper' */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "neillncurses.h"

#include <time.h>

#define ROWS 40
#define COLS ROWS
#define GENS 1000
#define HEAD 'H'
#define TAIL 't'
#define COPPER 'c'
#define EMPTY ' '

void runWorld(FILE *file);
void convertToArray(FILE *file, char grid[ROWS][COLS]);
void changeArray(char gridA[ROWS][COLS], char gridB[ROWS][COLS]);
int convertLetter(char grid[ROWS][COLS], int y, int x);
int checkSurrounding(char grid[ROWS][COLS], int i, int j);
void test(void);


int main(int argc, char *argv[])
{
  FILE *wire_file;
  if (argc != 2){
    fprintf(stderr, "Correct usage is %s <file>.txt\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  test();
  wire_file = fopen(argv[1], "r");
  if (wire_file == NULL){
    fprintf(stderr, "Failed to open file '%s'. Please ensure file exists and relevant permissions.\n",
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
  NCURS_Simplewin sw;

  convertToArray(file, grid_a);
  Neill_NCURS_Init(&sw);
  Neill_NCURS_CharStyle(&sw, "t", COLOR_RED, COLOR_BLACK, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, "H", COLOR_BLUE, COLOR_BLACK, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, "c", COLOR_YELLOW, COLOR_BLACK, A_NORMAL);

  Neill_NCURS_PrintArray(&grid_b[0][0], ROWS, COLS, &sw);
  /* Each for-loop contains 2 generations. Therefore the number of loops is halved. */
  do{
    changeArray(grid_a, grid_b);
    Neill_NCURS_PrintArray(&grid_b[0][0], ROWS, COLS, &sw);
    Neill_NCURS_Delay(100.0);
    changeArray(grid_b, grid_a);
    Neill_NCURS_PrintArray(&grid_a[0][0], ROWS, COLS, &sw);
    Neill_NCURS_Delay(100.0);
    Neill_NCURS_Events(&sw);
  }while(!sw.finished);

  atexit(Neill_NCURS_Done);

  exit(EXIT_SUCCESS);
}

/* Converts the file into a 2D array and then checks there are a total of 1600 characters used. */
void convertToArray(FILE *file, char grid[ROWS][COLS])
{
  int current, column = 0, row = 0, count = 0;
  /* Ensures we don't try to access outside the boundaries of the array */
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
    fprintf(stderr, "Something went wrong. Check the board is valid (40 x 40 and only contains 'H', 't', 'c' and ' ').\n");
    exit(EXIT_FAILURE);
  }
}


void changeArray(char current[ROWS][COLS], char next[ROWS][COLS])
{
  int r, c;
  for(r = 0; r < ROWS; r++){
    for(c = 0; c < COLS; c++){
      if(convertLetter(current, r, c) == 0){
        fprintf(stderr, "Something went wrong. The character %c is not recognised).\n", current[r][c]);
        exit(EXIT_FAILURE);
      }else{
        next[r][c] = convertLetter(current, r, c);
      }
    }
  }
}


int convertLetter(char grid[ROWS][COLS], int y, int x)
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

  }
  return 0;
}


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

  assert(convertLetter(test_grid, 0, 0) == ' ');
  assert(convertLetter(test_grid, 0, 1) == 'c');
  assert(convertLetter(test_grid, 3, 1) == 't');
  assert(convertLetter(test_grid, 3, 3) == 0);

  /*
  changeArray(test_grid, test_grid_out);
  assert(test_grid_out[1][1] == 't')
  assert(test_grid_out[0][0] == ' ')
  assert(test_grid_out[1][1] == 'c')
  assert(test_grid_out[1][1] == 't')
  */

}
