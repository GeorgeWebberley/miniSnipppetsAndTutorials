#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define N 20

void fillBoard(int board[N][N]);
int randomNum(void);
void printBoard(int board[N][N]);
void mutVert(int board[N][N]);
void mutHor(int board[N][N]);


int main(void)
{
  int board[N][N];
  int i;
  srand(time(0));
  fillBoard(board);
  printBoard(board);
  for(i = 0; i < (N*N*N); i++){

    mutHor(board);
    mutVert(board);
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


void mutVert(int board[N][N])
{
  int i, j, temp;
  i = randomLoc();
  j = randomLoc();
  temp = board[i][j];
  if(i == 0){
    if(board[i][j] > board[i+1][j]){
      board[i][j] = board[i+1][j];
      board[i+1][j] = temp;
    }
  }else{
    if(board[i][j] < board[i-1][j]){
      board[i][j] = board[i-1][j];
      board[i-1][j] = temp;
    }
  }
}

void mutHor(int board[N][N])
{
  int i, j, temp;
  i = randomLoc();
  j = randomLoc();
  temp = board[i][j];
  if(j == 0){
    if(board[i][j] > board[i][j+1]){
      board[i][j] = board[i][j+1];
      board[i][j+1] = temp;
    }
  }else{
    if(board[i][j] < board[i][j-1]){
      board[i][j] = board[i][j-1];
      board[i][j-1] = temp;
    }
  }
}
