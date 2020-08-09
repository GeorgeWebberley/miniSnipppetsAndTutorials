#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define N 20

void fillBoard(int board[N][N]);
int randomNum(void);
void printBoard(int board[N][N]);
void mutBoard(int board[N][N]);

int pythag(int num1, int num2, int middle);




int main(void)
{
  int board[N][N];
  int i;
  srand(time(0));
  fillBoard(board);
  printBoard(board);
  for(i = 0; i < (N*N*N); i++){

    mutBoard(board);

  }
  printf("\n\n");
  printBoard(board);
  return 0;
}

/* fills board size N with random numbers between 1-9 */
void fillBoard(int board[N][N])
{
  int i, j;

  for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      board[i][j] = randomNum();
    }
  }
}

/* genegerates random number between 0 and 9 */
int randomNum(void)
{
  int i;
  i = rand() % 10;
  return i;
}

int randomLoc(void)
{
  int i;
  i = rand() % N;
  return i;
}

void printBoard(int board[N][N])
{
  int i, j;

  for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      printf("%d", board[i][j]);
    }
    printf("\n");
  }
}


void mutBoard(int board[N][N])
{
  int i1, j1, i2, j2, temp, middle, g1, g2;
  middle = N/2;
  i1 = randomLoc();
  j1 = randomLoc();
  i2 = randomLoc();
  j2 = randomLoc();

  g1 = pythag(i1, j1, middle);
  g2 = pythag(i2, j2, middle);


  temp = board[i1][j1];
  if(g1 > g2){
    if(board[i1][j1] < board[i2][j2]){
      board[i1][j1] = board[i2][j2];
      board[i2][j2] = temp;
    }
  }else{
    if(board[i1][j1] > board[i2][j2]){
      board[i1][j1] = board[i2][j2];
      board[i2][j2] = temp;
    }
  }
}

int pythag(int num1, int num2, int middle)
{
  int n1, n2;

  n1 = (middle - num1) * (middle - num1);
  n2 = (middle - num2) * (middle - num2);

  return n1 + n2;
}
