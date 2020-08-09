/* A program that simulates an 8-tile puzzle. A user can input an 8-tile board
as a string (e.g. "123 54678") and the program will print out the correct
node of moves needed to solve the problem or return with failure if it is not
possible. */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define SIZE 3

#define MAX 300000

struct node{
  char grid[SIZE][SIZE];
  int x;
  int y;
  struct node *parent;
  struct node *next;
  int path;
};
typedef struct node node;

void test(void);
void convertArray(char* string, node *first);
void findSpace(node *current);
void printArray(char grid[SIZE][SIZE]);
node *findNext(node *start, node *current, node *end);
int checkSolution(char grid[SIZE][SIZE]);
int checkUsed(node *start, char grid[SIZE][SIZE], node *end);
void recordPath(node *start, node *end);
void printPath(node *start, node *end);
node *createNode(node *parent);




void printsequence(node *start, node *end);








int main(int argc, char** argv)
{
  node *current, *end;
  node start;
  int count = 0;
  test();
  if(argc != 2){
    fprintf(stderr, "Incorrect usage. Please type %s followed by a string of numbers inside double inverted commas.\n", argv[0]);
  }
  convertArray(argv[1], &start);
  findSpace(&start);
  current = end = &start;
  printArray(start.grid);
  while(count < 1000){
    end = findNext(&start, current, end);
    current = current->next;
    /*printArray(current->grid);*/
    count++;
  }
  printf("Unable to find a solution for the string '%s'.\n", argv[1]);
  return 0;
}

/* Converts the inital string into a 2d array to begin the queue. */
void convertArray(char* string, node *current)
{
  int i, j, s = 0;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++, s++){
      current->grid[i][j] = string[s];
    }
  }
  current->path = 0;
  current->parent = NULL;
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
void findSpace(node *current)
{
  int i, j;
  current->x = -1;
  current->y = -1;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      if(current->grid[i][j] == ' '){
        current->y = i;
        current->x = j;
      }
    }
  }
}

node *createNode(node *parent)
{
  node *child;

  child = (node *)malloc(sizeof(node));

  if(child == NULL){
    fprintf(stderr, "Unable to create space.\n");
    exit(EXIT_FAILURE);
  }
  *child = *parent;
  child->parent = parent;
  child->next = NULL;
  return child;
}




node *findNext(node *start, node *current, node *end)
{
  int x = current->x;
  int y = current->y;
  int i;

  for(i = x - 1; i <= x + 1; i = i+2){
    if(i >= 0 && i <= 2){
      end->next = createNode(current);


      end->next->grid[y][i] = current->grid[y][x];
      end->next->grid[y][x] = current->grid[y][i];
      end->next->x = i;

      if(checkUsed(start, end->next->grid, end->next)){
        free(end->next);
      }else{
        end = end->next;
        printArray(end->grid);
        if(checkSolution(end->grid)){
          printf("YES\n");


          recordPath(start, end);
          printPath(start, end);
        }
      }
    }
  }

  for(i = y - 1; i <= y + 1; i = i+2){
    if(i >= 0 && i <= 2){
      end->next = createNode(current);
      end->next->grid[i][x] = current->grid[y][x];
      end->next->grid[y][x] = current->grid[i][x];
      end->next->y = i;
      if(checkUsed(start, end->next->grid, end->next)){
        free(end->next);
      }else{
        end = end->next;
        printArray(end->grid);

        if(checkSolution(end->grid)){
          printf("YES\n");

          recordPath(start, end);
          printPath(start, end);
        }
      }
    }
  }
  return end;
}


/* Checks if the current grid has been used at all previously in the queue. */
int checkUsed(node *start, char grid[SIZE][SIZE], node *end)
{
  int i, j, used_flag;
  node *current = start;
  while(current != end){
    used_flag = 1;
    for(i = 0; i < SIZE; i++){
      for(j = 0; j < SIZE; j++){
        if(current->grid[i][j] != grid[i][j]){
          used_flag = 0;
        }
      }
    }
    /* If reaches end of grid comparison and the flag is still raised then this
    grid has already been used. */
    if(used_flag == 1){
      return 1;
    }
    current = current->next;
  }
  /* If it reaches the end of the queue and it hasn't return 1 then this grid
  has not yet been used. */
  return 0;
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


void recordPath(node *start, node *end)
{
  node *current;
  current = end;
  /*Then need to loop back through the 'parent' elements setting path-flags to 1 */
  while(current->parent != NULL){
    current->path = 1;
    current = current->parent;
  }
  start->path = 1;
}



void printPath(node *start, node *end)
{
  int count = 0;
  node *current = start;
  while(current->next != NULL){
    if(current->path == 1){
      printArray(current->grid);
      current = current->next;
      count++;
    }
  }
  printArray(end->grid);
  printf("This tile can solved in %d moves.\n", count);
  exit(EXIT_SUCCESS);
}


void test(void)
{
  node first;
  node second;
  node test_struct;
  node *current;
  char test_string[9] = "23614 758";
  char test_string2[9] = "1234567 8";
  char no_space[9] = "123456789";
  char correct_string[9] = "12345678 ";
  char unused_grid[3][3] = {{'1', '2', '3'},
                            {'1', '4', ' '},
                            {'7', '5', '8'}};
  char used_grid[3][3] = {{'2', '3', '6'},
                          {'1', '4', ' '},
                          {'7', '5', '8'}};


  convertArray(test_string, &first);
  assert(first.grid[0][0] == '2');
  assert(first.grid[2][2] == '8');
  assert(first.grid[0][2] == '6');


  convertArray(test_string2, &second);
  assert(second.grid[1][2] == '6');
  assert(second.grid[2][1] == ' ');

  findSpace(&second);
  assert(second.x == 1);
  assert(second.y == 2);

  convertArray(no_space, &test_struct);
  findSpace(&test_struct);
  assert(test_struct.x == -1);
  assert(test_struct.y == -1);

  current = &first;
  current->next = createNode(current);
  current = current->next;
  assert(current->grid[0][0] == '2');
  assert(current->grid[2][2] == '8');
  assert(current->grid[0][2] == '6');
  /* Add a third and fourth node on to the list. This is so I can test the function 'checkUsed'. */
  current->next = &second;
  current = current->next;
  current->next = &test_struct;
  current = current->next;
  assert(checkUsed(&first, used_grid, current) == 1);
  assert(checkUsed(&first, unused_grid, current) == 0);
  assert(checkUsed(&first, second.grid, current) == 1);



  assert(checkSolution(test_struct.grid) == 0);
  convertArray(correct_string, &test_struct);
  assert(checkSolution(test_struct.grid) == 1);


}
