/* A program that simulates the cellular automation of a 2D grid of cells
in digital electronics. Each cell can be in one of 4 states: 'empty',
'electron head', 'electron tail' or 'copper' */

#include <stdio.h>
#include <stdlib.h>
#include "neillncurses.h"
/* #define NDEBUG */
#include <assert.h>

#define ROWS 40
#define COLS ROWS
#define GENS 1000
#define HEAD 'H'
#define TAIL 't'
#define COPPER 'c'
#define EMPTY ' '


void convertToArray(FILE *file, char grid[ROWS][COLS]);
void runWorld(char grid_a[ROWS][COLS]);
/* void printWire(char grid[ROWS][COLS]); */
void changeArray(char current[ROWS][COLS], char next[ROWS][COLS]);
int convertLetter(char grid[ROWS][COLS], int y, int x);
int isValid(char c);
int checkSurrounding(char grid[ROWS][COLS], int i, int j);
void test(void);


int main(int argc, char *argv[])
{
  FILE *wire_file;
  char grid_a[ROWS][COLS];
  if (argc != 2){
    fprintf(stderr, "Correct usage is %s <file>.txt\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  test();
  wire_file = fopen(argv[1], "r");
  if (wire_file == NULL){
    fprintf(stderr, "Failed to open file '%s'. Please ensure file exists and the relevant permissions.\n",
            argv[1]);
    exit(EXIT_FAILURE);
  }
  convertToArray(wire_file, grid_a);
  fclose(wire_file);
  runWorld(grid_a);
  return 0;
}


/* Converts the file to a 2D array and checks there are a total of 1600 characters used. */
void convertToArray(FILE *file, char grid[ROWS][COLS])
{
  int current, column = 0, row = 0, count = 0;
  /* Ensures we don't try to write outside the boundaries of the array (ignores new line character). */
  while((current = getc(file)) != EOF && row <= ROWS && column <= COLS){
    if (current == '\n'){
      row++;
      column = 0;
    }
    if(isValid(current)){
      grid[row][column] = current;
      column++;
      count++;
    }
  }
  if (count != ROWS * COLS){
    fprintf(stderr, "Something went wrong. Check the board is valid (40 x 40 and only contains 'H', 't', 'c' and ' ').\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }
}


void runWorld(char grid_a[ROWS][COLS])
{
  char grid_b[ROWS][COLS];
  int i;
  NCURS_Simplewin sw;

  Neill_NCURS_Init(&sw);
  Neill_NCURS_CharStyle(&sw, "t", COLOR_RED, COLOR_BLACK, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, "H", COLOR_BLUE, COLOR_BLACK, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, "c", COLOR_YELLOW, COLOR_BLACK, A_NORMAL);

  Neill_NCURS_PrintArray(&grid_a[0][0], ROWS, COLS, &sw);
  /* Each for-loop contains 2 generations. Therefore the number of loops is halved. */
  for(i = 0; i < GENS/2 && !sw.finished; i++){
    changeArray(grid_a, grid_b);
    Neill_NCURS_PrintArray(&grid_b[0][0], ROWS, COLS, &sw);
    Neill_NCURS_Delay(1000.0);
    changeArray(grid_b, grid_a);
    Neill_NCURS_PrintArray(&grid_a[0][0], ROWS, COLS, &sw);
    Neill_NCURS_Delay(1000.0);
    Neill_NCURS_Events(&sw);
  }
  atexit(Neill_NCURS_Done);
  exit(EXIT_SUCCESS);
}


/* Previous print function before ncurses

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
*/


void changeArray(char current[ROWS][COLS], char next[ROWS][COLS])
{
  int row, column;
  for(row = 0; row < ROWS; row++){
    for(column = 0; column < COLS; column++){
      next[row][column] = convertLetter(current, row, column);
    }
  }
}

int isValid(char c)
{
  if (c == HEAD || c == TAIL || c == COPPER || c == EMPTY){
    return 1;
  }
  return 0;
}


int convertLetter(char grid[ROWS][COLS], int y, int x)
{
  int new;
  switch(grid[y][x]){
    case EMPTY:
      new = EMPTY;
      break;
    case HEAD:
      new = TAIL;
      break;
    case TAIL:
      new = COPPER;
      break;
    case COPPER:
      if(checkSurrounding(grid, y, x) == 1 || checkSurrounding(grid, y, x) == 2){
        new = HEAD;
      }else{
        new = COPPER;
      }
      break;
    /* Should not be reached in the program after 'isValid' is called but
    included for testing and error checking. */
    default:
      new = EMPTY;
  }
  return new;
}


int checkSurrounding(char grid[ROWS][COLS], int y, int x)
{
  int count = 0;
  int i, j;
  for(i = y-1; i <= y+1; i++){
    for(j = x-1; j <= x+1; j++){
      /* Check to ensure not going off end of array */
      if(i >= 0 && i < ROWS && j >= 0 && j < COLS){
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

  char next_test[ROWS][COLS];

  assert(checkSurrounding(test_grid, 2, 1) == 3);
  assert(checkSurrounding(test_grid, 1, 2) == 1);
  assert(checkSurrounding(test_grid, 3, 0) == 2);
  assert(checkSurrounding(test_grid, 0, 3) == 0);

  assert(convertLetter(test_grid, 0, 0) == EMPTY);
  assert(convertLetter(test_grid, 0, 1) == COPPER);
  assert(convertLetter(test_grid, 3, 1) == TAIL);
  assert(convertLetter(test_grid, 1, 2) == HEAD);
  assert(convertLetter(test_grid, 3, 3) == EMPTY);

  assert(isValid(' ') == 1);
  assert(isValid('H') == 1);
  assert(isValid('t') == 1);
  assert(isValid('-') == 0);
  assert(isValid('p') == 0);

  changeArray(test_grid, next_test);
  /* Not able to test row/column 3 as I do not know what has been stored in row/column 4 */
  assert(next_test[0][0] == EMPTY);
  assert(next_test[0][1] == COPPER);
  assert(next_test[0][2] == EMPTY);
  assert(next_test[1][0] == EMPTY);
  assert(next_test[1][1] == TAIL);
  assert(next_test[1][2] == HEAD);
  assert(next_test[2][0] == TAIL);
  assert(next_test[2][1] == COPPER);
  assert(next_test[2][2] == HEAD);
}
