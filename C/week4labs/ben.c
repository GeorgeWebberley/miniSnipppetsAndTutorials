#include <stdio.h>
/*#define NDEBUG*/
#include <assert.h>
#include <stdlib.h>
#define GRIDSIZE 40
#define ITERATIONS 1000
/*USE NDEBUG to turn off asserts*/

void populateInitialArray(char grid1[][GRIDSIZE], FILE* gridFile);
char returnNextGenCell(char grid[][GRIDSIZE], int row, int col);
int countHeads(char grid[][GRIDSIZE], int row, int col);
void createNextGenArray(char grid[][GRIDSIZE], char nextGrid[][GRIDSIZE]);
void printArray(char grid[][GRIDSIZE]);
void runSimulation(char const *argv[], int iterations);
void test(char grid[][GRIDSIZE]);

int main(int argc, char const *argv[])
{

  if (argc == 2)
  {
    printf("Running Simluation...\n");
    runSimulation(argv, ITERATIONS);
  }
  else
  {
    printf("ERROR: Invalid number of arguments. Enter %s followed by the name of the initial state txt file", argv[0]);
  }

  return 0;
}
/* add in error check to that \n in in space 40*/
void populateInitialArray(char grid1[][GRIDSIZE], FILE* gridFile)
{
   int i = 0, j = 0;
   char c;

   for (i = 0; i <= GRIDSIZE; i++)
   {
      for (j = 0; j <= GRIDSIZE; j++)
      {
        if ((c = getc(gridFile))!= EOF && c != '\n')
        {
          grid1[i][j] = c;
        }
      }
   }
}

void runSimulation(char const *argv[],int iterations)
{
  int i;
  char grid1[GRIDSIZE][GRIDSIZE];
  char grid2[GRIDSIZE][GRIDSIZE];
  FILE* gridFile;
  if ((gridFile = fopen(argv[1], "r")) == NULL)
  {
    printf("Cannot open file\n");
    exit(1);
  }
  /*Fill the first array with the contents of the text file*/
  populateInitialArray(grid1, gridFile);
  printArray(grid1);
  /*Now run the simulation for 1000 generations*/
  for (i = 2; i < iterations + 2; i++)
  {
    if (i % 2 == 0)
    {
       createNextGenArray(grid1, grid2);
    }
    else
    {
       createNextGenArray(grid2, grid1);
    }
  }
  /*Print the correct array for the last iteration*/
  if (i % 2 == 0)
  {
    printArray(grid2);
  }
  else
  {
    printArray(grid1);
  }
}

void createNextGenArray(char grid[][GRIDSIZE], char nextGrid[][GRIDSIZE])
{
   int i, j;

   for (i = 0; i < GRIDSIZE; i++)
   {
      for (j = 0; j < GRIDSIZE; j++)
      {
         nextGrid[i][j] = returnNextGenCell(grid, i, j);
      }
   }
}

void printArray(char grid[][GRIDSIZE])
{
   int i, j;
   for (i = 0; i < GRIDSIZE; i++)
   {
      for (j = 0; j < GRIDSIZE; j++)
      {
        printf("%c", grid[i][j]);
      }
      printf("\n");
   }
}

char returnNextGenCell(char grid[][GRIDSIZE], int row, int col)
{
   char nextCell;
   if (grid[row][col] == ' ')
   {
      nextCell = ' ';
   }
   if (grid[row][col] == 'H')
   {
      nextCell = 't';
   }
   if (grid[row][col] == 't')
   {
      nextCell = 'c';
   }
   if (grid[row][col] == 'c')
   {
      if ((countHeads(grid, row, col) == 1) || (countHeads(grid, row, col) == 2))
      {
         nextCell = 'H';
      }
      else
      {
         nextCell = 'c';
      }
   }

   return nextCell;
}

/*Function used to count the number of heads around a 'c' cell*/
int countHeads(char grid[][GRIDSIZE], int row, int col)
{
   int i, j, numHeads = 0;

   for (i = row - 1; i < row + 2; i++)
   {
      if (i > -1 && i < GRIDSIZE)
      {
         for (j = col - 1; j < col + 2 ; j++)
         {
            if (j > -1 && j < GRIDSIZE)
            {
               if (grid[i][j] == 'H')
               {
                  numHeads += 1;
               }
            }
         }

      }
   }

   return numHeads;
}

void test(char grid[][GRIDSIZE])
{
  assert(returnNextGenCell(grid, 4, 7) == 'c');
  assert(returnNextGenCell(grid, 4, 9) == 't');
  assert(countHeads(grid, 4, 10) == 1);
  assert(grid[4][7] == 'c');
  assert(countHeads(grid, 5, 4) == 0);
  assert(countHeads(grid, 0, 9) == 0);

}
