void findNext(sequence *queue, int current, int* end)
{
  int x = queue[current].x;
  int y = queue[current].y;
  /*Check right-left movement (change in 'x').
  Loops from 'x-1' to 'x+1' excluding 'x'. */
  for(i = x - 1; i <= x + 1; i = i+2){
    queue[*end] = queue[current];
    queue[*end].grid[y][i] = queue[current].grid[y][x];
    queue[*end].grid[y][x] = queue[current].grid[y][i];
    queue[*end].parent = current;
    queue[*end].x = i;
    if(!checkUsed(queue, queue[*end].grid, *end)){
      if(checkSolution(queue[*end].grid)){
        recordPath(queue, *end);
        printPath(queue, *end);
      }else{
        *end = *end + 1;
      }
    }
  }
  /*Check up-down movement (change in 'y').
  Loops from 'y-1' to 'y+1' excluding 'x'. */
  for(i = y - 1; i <= y + 1; i = i+2){
    queue[*end] = queue[current];
    queue[*end].grid[i][x] = queue[current].grid[y][x];
    queue[*end].grid[y][x] = queue[current].grid[i][x];
    queue[*end].parent = current;
    queue[*end].y = i;
    if(!checkUsed(queue, queue[*end].grid, *end)){
      if(checkSolution(queue[*end].grid)){
        recordPath(queue, *end);
        printPath(queue, *end);
      }else{
        *end = *end + 1;
      }
    }
  }
}
