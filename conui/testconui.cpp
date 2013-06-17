#include "console.h"

using namespace cui;

void prnbox(int row, int col, bool clear = false){
  console.strdsp(clear? "":"*****************", row, col, 17);
  console.strdsp(clear? "":"*****************", row +1, col, 17);
  console.strdsp(clear? "":"*****************", row + 2, col, 17);
}

int main(){
  char str[81] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPabcdefghijklmnopqrstuvwxyz0123456789AB";
  //str[10] = 0;
  bool done = false;
  int row = 0;
  int col = 0;
  int key;
  int offset = 7;
  int curpos = 8;
  prnbox(10, 20);
  key = console.stredit(str, 11, 21, 15, 80, &offset, &curpos);
  console.strdsp(str, 15, 21);
  console.pause();
  return 0;
}