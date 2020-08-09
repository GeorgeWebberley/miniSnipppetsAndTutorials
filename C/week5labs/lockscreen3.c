/* A program that resembles a lock screen */

#include <stdio.h>
#include <assert.h>

#define GRID 5
#define LENGTH 25
#define USED '#'

struct coords
{
  int x;
  int y;
  int count;
};

int recurse(char pass[GRID][GRID], struct coords *location, int path);
void popPass(char pass[GRID][GRID]);
void test(void);


int main(void)
{
  int path = 1;
  struct coords location;
  char pass[GRID][GRID];
  test();
  location.x = 0;
  location.y = 0;
  location.count = 0;
  popPass(pass);
  recurse(pass, &location, path);
  printf("There are %d different ways of solving a grid of size %d when starting at location %d %d.", location.count, GRID, location.x, location.y);
  return 0;
}

/* Populates the grid with empty space characters. */
void popPass(char pass[GRID][GRID])
{
  int i, j;
  for(i = 0; i < GRID; i++){
    for(j = 0; j < GRID; j++){
      pass[i][j] = ' ';
    }
  }
}



int recurse(char pass[GRID][GRID], struct coords *location, int path)
{
  if(path == LENGTH){
    location->count++;
    printf("count: %d\n", location->count);
    return 1;
  }

  /* mark current location as already used */
  pass[location->y][location->x] = USED;
  /* Check up, then backtrack. */
  if((location->y > 0) && (pass[location->y-1][location->x] != USED)){
    location->y--;
    path++;
    printf("up\n");
    recurse(pass, location, path);
    pass[location->y][location->x] = ' ';
    path--;
    location->y++;
  }
  /* Check right, then backtrack. */
  if(((location->x + 1)< GRID) && (pass[location->y][location->x + 1] != USED)){
    location->x++;
    path++;
    printf("right\n");
    recurse(pass, location, path);
    pass[location->y][location->x] = ' ';
    path--;
    location->x--;
  }
  /* Check down, then backtrack. */
  if(((location->y + 1) < GRID) && (pass[location->y + 1][location->x] != USED)){
    path++;
    location->y++;
    printf("down\n");
    recurse(pass, location, path);
    pass[location->y][location->x] = ' ';
    path--;
    location->y--;
  }
  /* Check left, then backtrack. */
  if((location->x > 0) && (pass[location->y][location->x-1] != USED)){
    path++;
    location->x--;
    printf("left\n");
    recurse(pass, location, path);
    pass[location->y][location->x] = ' ';
    path--;
    location->x++;
  }

  return location->count;
}


void test(void)
{
  char test[GRID][GRID];
  int i, j;
  popPass(test);

  for(i = 0; i < GRID; i++){
    for(j = 0; j < GRID; j++){
      assert(test[i][j] == ' ');
    }
  }




}
