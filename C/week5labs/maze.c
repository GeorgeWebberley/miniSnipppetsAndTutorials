/* A program that accepts a text file as argv[1] which contains a maze.
The maze height and width is specified at the start of the text file (max 20x20).
The program returns 0 if it succeeds in escaping the maze and 1 if not */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define MAXSIZE 20
#define WALL '#'
#define EMPTY ' '

enum direction {above = -1, right = 2, below = 1, left = -2};
typedef enum direction direction;



void test(void);
int escaped(int x, int y, int width, int height);
int checkSurrounding(char maze[MAXSIZE][MAXSIZE], int width, int height, int x, int y, direction prev, int history);
int countSpaces(char maze[MAXSIZE][MAXSIZE], int width, int height);


int main(void)
{
  /*if(argc != 2){
    printf("Please type %s followed by a tex file containing a valid maze\n", agrv[0]);
  }
  */

  test();

  return 0;
}


int escaped(int x, int y, int width, int height)
{
  if(x >= width || y >= height || x < 0 || y < 0){
    return 1;
  }else{
    return 0;
  }
}

/* a function that checks surrounding cells for empty spaces. It will always check
in clockwise direction so will return 'above' even if a cell also has an empty
'below' cell. */
int checkSurrounding(char maze[MAXSIZE][MAXSIZE], int width, int height, int x, int y, direction prev, int history)
{

  if (escaped(x, y, width, height)){
    printf("Success!\n");
    (exit(1));
  }
  while(history < countSpaces(maze, width, height)){
    /* check above */
    if(prev != below){
      if(((y - 1) >= 0) && (maze[y-1][x] != WALL)){
        checkSurrounding(maze, width, height, x, y - 1, above, ++history);
      }
    }
    /* check right */
    if(prev != left){
      if(((x + 1) < MAXSIZE) && (maze[y][x+1] != WALL)){
        checkSurrounding(maze, width, height, x + 1, y, right, ++history);
      }
    }
    /* check below */
    if(prev != above){
      if(((y + 1) < MAXSIZE) && (maze[y+1][x] != WALL)){
        checkSurrounding(maze, width, height, x, y + 1, below, ++history);
      }
    }
    /* check left */
    if(prev != right){
      if(((x - 1) >= 0) && (maze[y][x-1] != WALL)){
        checkSurrounding(maze, width, height, x - 1, y, left, ++history);
      }
    }
    return 0;
  }
  printf("Failure\n");
  return 0;
}

/* This function checks the total spaces in the maze and is important for ensuring
that all possible routes have been explored. */
int countSpaces(char maze[MAXSIZE][MAXSIZE], int width, int height)
{
  int i, j, count = 0;
  for(i = 0; i < height; i++){
    for(j = 0; j < width; j++){
      if(maze[i][j] == EMPTY){
        count++;
      }
    }
  }
  return count;
}

void test(void)
{
  int height = 10, width = 10;
  int history = 0, prev = 0;
  int x = 0, y = 1;
  char test_maze[MAXSIZE][MAXSIZE] = {{'#', '#', '#', '#','#', '#','#', '#','#', '#'},
                                      {' ', ' ', '#', ' ',' ', ' ',' ', ' ',' ', '#'},
                                      {'#', ' ', '#', ' ','#', ' ','#', '#',' ', '#'},
                                      {'#', ' ', '#', ' ','#', '#','#', '#',' ', '#'},
                                      {'#', ' ', '#', ' ',' ', ' ',' ', '#',' ', '#'},
                                      {'#', ' ', '#', ' ','#', '#','#', '#',' ', '#'},
                                      {'#', ' ', '#', ' ',' ', ' ',' ', '#',' ', '#'},
                                      {'#', ' ', '#', '#','#', '#',' ', '#',' ', '#'},
                                      {'#', ' ', ' ', ' ',' ', ' ',' ', '#',' ', ' '},
                                      {'#', '#', '#', '#','#', '#','#', '#','#', '#'}};

  assert(escaped(5, 7, width, height) == 0);
  assert(escaped(9, 9, width, height) == 0);
  assert(escaped(10, 9, width, height) == 1);
  assert(escaped(0, 0, width, height) == 0);
  assert(escaped(0, 20, width, height) == 1);
  assert(escaped(-1, 5, width, height) == 1);
  assert(escaped(2, -1, width, height) == 1);

  assert(countSpaces(test_maze, width, height) == 41);

  assert(checkSurrounding(test_maze, width, height, x, y, prev, history) == 1);

}
