/* A program that resembles a lock screen */

#include <stdio.h>
#include <assert.h>

#define GRID 3
#define LENGTH 9
#define USED '#'

struct coords
{
  int x;
  int y;
};

int recurse(char pass[GRID][GRID], struct coords *location, int count, int path);
void popPass(char pass[GRID][GRID]);
void test(void);


int main(void)
{
  int count = 0, path = 1;
  struct coords location;
  char pass[GRID][GRID];
  test();
  location.x = 0;
  location.y = 0;
  popPass(pass);
  count = recurse(pass, &location, count, path);
  printf("There are %d different ways of solving a grid of size %d when starting at location %d %d.", count, GRID, location.x, location.y);
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



int recurse(char pass[GRID][GRID], struct coords *location, int count, int path)
{
  if(path == 9){
    count++;
    return 1;
  }
  /* mark current location as already used */
  pass[location->x][location->y] = USED;
  /* Check up */
  if((location->y > 0) && (pass[location->x][location->y - 1] != USED)){
    path++;
    location->y--;
    if(recurse(pass, location, count, path)){
      return 1;
    }
  }
  /* Check right */
  if((location->x < GRID) && (pass[location->x + 1][location->y] != USED)){
    path++;
    location->x++;
    if(recurse(pass, location, count, path)){
      return 1;
    }
  }
  /* Check up */
  if((location->y < GRID) && (pass[location->x][location->y + 1] != USED)){
    path++;
    location->y++;
    if(recurse(pass, location, count, path)){
      return 1;
    }
  }
  /* Check up */
  if((location->x > 0) && (pass[location->x - 1][location->y] != USED)){
    path++;
    location->x--;
    if(recurse(pass, location, count, path)){
      return 1;
    }
  }
  return count;
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
