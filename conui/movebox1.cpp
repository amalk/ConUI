#include "console.h"

using namespace cui;

void prnbox(int row, int col){
  console.dspstr("*****************", row, col);
  console.dspstr("*****************", row +1, col);
  console.dspstr("*****************", row + 2, col);
}

int main(){
  bool done = false;
  int row = 0;
  int col = 0;
  int key;
  while(!done){
    prnbox(row, col);
    key = console.getKey();
    switch(key){
    case DOWN:
      row++;
      console.clear();
      break;
    case UP:
      row--;
      console.clear();
      break;
    case RIGHT:
      col++;
      console.clear();
      break;
    case LEFT:
      col--;
      console.clear();
      break;
    case ESCAPE:
      done = true;
      break;
    }
  }
  return 0;
}