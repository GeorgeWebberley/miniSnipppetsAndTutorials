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

struct coords{
  int x;
  int y;
};

void test(void);

void convertArray(char maze[MAXSIZE][MAXSIZE], FILE *fp, int width, int height);

int escaped(struct coords *p, int width, int height);
int checkEscape(char maze[MAXSIZE][MAXSIZE], int width, int height, struct coords *p, direction prev, int history);
int countSpaces(char maze[MAXSIZE][MAXSIZE], int width, int height);
void findStart(char maze[MAXSIZE][MAXSIZE], struct coords *p, int width, int height);




int main(int argc, char **argv)
{
  FILE *fp;
  int width, height, history = 0;
  char maze[MAXSIZE][MAXSIZE];
  struct coords position = {0, 0};
  /* At the start there is no previous location so prev set to 0 */
  int prev = 0;

  if(argc != 2){
    fprintf(stderr, "Please type %s followed by a tex file containing a valid maze\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  fp = fopen(argv[1], "r");
  if(fp == NULL){
    fprintf(stderr, "Not able to open file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  test();
  fscanf(fp, "%d %d", &width, &height);
  convertArray(maze, fp, width, height);
  fclose(fp);
  findStart(maze, &position, width, height);

  checkEscape(maze, width, height, &position, prev, history);

  return 0;
}

void convertArray(char maze[MAXSIZE][MAXSIZE], FILE *fp, int width, int height)
{
  int i, j;
  /* moves the file pointer to the start of the array */
  while(getc(fp) != WALL){
  }
  for(i = 0; i < width; i++){
    for(j = 0; j < width; j++){
      maze[i][j] = getc(fp);
    }
  }
}

void findStart(char maze[MAXSIZE][MAXSIZE], struct coords *p, int width, int height)
{
  int i, y, x;
  for(i = (width-1); i >= 0; i--){
    if(maze[0][i] == ' ')
    x = i;
  }
  for(i = (height-1); i >= 0; i--){
    if(maze[i][0] == ' ')
    y = i;
  }
  if(y < x){
    p->x = 0;
    p->y = y;
  }else{
    p->x = x;
    p->y = y;
  }
}


int escaped(struct coords *p, int width, int height)
{
  if(p->x >= width || p->y >= height || p->x < 0 || p->y < 0){
    return 1;
  }else{
    return 0;
  }
}

/* A function that checks surrounding cells for empty spaces. It will always check
in clockwise direction so will return 'above' even if a cell also has an empty
'below' cell. */
int checkEscape(char maze[MAXSIZE][MAXSIZE], int width, int height, struct coords *p, direction prev, int history)
{

  if (escaped(p, width, height)){

    printf("Success!\n");
    exit(1);
  }
  while(history < countSpaces(maze, width, height)){
    /* check above */
    if(prev != below){
      if(maze[y-1][x] != WALL){
        p->y--;
        checkEscape(maze, width, height, p, above, ++history);
      }
    }
    /* check right */
    if(prev != left){
      if(maze[y][x+1] != WALL){
        p->x++;
        checkEscape(maze, width, height, p, right, ++history);
      }
    }
    /* check below */
    if(prev != above){
      if(maze[y+1][x] != WALL){
        p->y++;
        checkEscape(maze, width, height, p, below, ++history);
      }
    }
    /* check left */
    if(prev != right){
      if(maze[y][x-1] != WALL){
        p->x--;
        checkEscape(maze, width, height, &position, left, ++history);
      }
    }
    printf("Failure\n");
    exit(0);
  }

  printf("Failure\n");
  exit(0);
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
  struct coords position = {0, 1};

  char test_maze[MAXSIZE][MAXSIZE] = {{'#', '#', '#', '#', '#', '#','#', '#', '#', '#'},
                                      {' ', ' ', '#', ' ', ' ', ' ',' ', ' ', ' ', '#'},
                                      {'#', ' ', '#', ' ', '#', ' ','#', '#', ' ', '#'},
                                      {'#', ' ', '#', ' ', '#', '#','#', '#', ' ', '#'},
                                      {'#', ' ', '#', ' ', ' ', ' ',' ', '#', ' ', '#'},
                                      {'#', ' ', '#', ' ', '#', '#','#', '#', ' ', '#'},
                                      {'#', ' ', '#', ' ', ' ', ' ',' ', '#', ' ', '#'},
                                      {'#', ' ', '#', '#', '#', '#',' ', '#', ' ', '#'},
                                      {'#', ' ', ' ', ' ', ' ', ' ',' ', '#', ' ', ' '},
                                      {'#', '#', '#', '#', '#', '#','#', '#', '#', '#'}};


  char no_exit[MAXSIZE][MAXSIZE] =   {{'#', '#', '#', '#', '#', '#','#', '#', '#', '#'},
                                      {'#', ' ', '#', ' ', ' ', ' ',' ', ' ', ' ', '#'},
                                      {'#', ' ', '#', ' ', '#', ' ','#', '#', ' ', '#'},
                                      {'#', ' ', '#', ' ', '#', '#','#', '#', ' ', '#'},
                                      {'#', ' ', '#', ' ', ' ', ' ',' ', '#', ' ', '#'},
                                      {'#', ' ', '#', ' ', '#', '#','#', '#', ' ', '#'},
                                      {'#', ' ', '#', ' ', ' ', ' ',' ', '#', ' ', '#'},
                                      {'#', ' ', '#', '#', '#', '#',' ', '#', ' ', '#'},
                                      {'#', ' ', ' ', ' ', ' ', ' ',' ', '#', ' ', '#'},
                                      {'#', '#', '#', '#', '#', '#','#', '#', '#', '#'}};

  assert(escaped(5, 7, width, height) == 0);
  assert(escaped(9, 9, width, height) == 0);
  assert(escaped(10, 9, width, height) == 1);
  assert(escaped(0, 0, width, height) == 0);
  assert(escaped(0, 20, width, height) == 1);
  assert(escaped(-1, 5, width, height) == 1);
  assert(escaped(2, -1, width, height) == 1);

  assert(countSpaces(test_maze, width, height) == 41);
  assert(countSpaces(no_exit, width, height) == 39);

  /* This checks if possible to escape the maze. However it will escape from the
  entrance if it is not blocked off. */
  assert(checkEscape(test_maze, width, height, &position, prev, history) == 1);
  /* To test that I must test a maze without an entrance. (The program will
  block the entrance with a WALL when ran properly.)
  To test the below you need to comment out line above (146). */
  x = 1;
  y = 1;
  assert(checkEscape(no_exit, width, height, &position, prev, history) == 0);

}
