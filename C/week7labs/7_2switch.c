void findNext(sequence *queue, int current, int end, int space)
{
  switch(space){
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[end].grid[0][0] = queue[current].grid[0][1];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
    case 2:
    /* Top middle, 3 different movements possible added onto end of queue */
      queue[end].grid[0][1] = queue[current].grid[0][0];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[end].grid[0][0] = queue[current].grid[0][1];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[end].grid[0][0] = queue[current].grid[0][1];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[end].grid[0][0] = queue[current].grid[0][1];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[end].grid[0][0] = queue[current].grid[0][1];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[end].grid[0][0] = queue[current].grid[0][1];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[end].grid[0][0] = queue[current].grid[0][1];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
    case 1:
    /* Top left corner, 2 different movements possible added onto end of queue */
      queue[end].grid[0][0] = queue[current].grid[0][1];
      queue[end].grid[0][1] = queue[current].grid[0][0];
      end++;
      queue[end].grid[0][0] = queue[current].grid[1][0];
      queue[end].grid[1][0] = queue[current].grid[0][0];
      end++;
      break;
  }
}
