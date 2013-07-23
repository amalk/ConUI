// Console UI Testers
// Test1Frame.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9



#include "cuigh.h"
#include "console.h"
#include "clabel.h"
#include "cdialog.h"
#include <cstdio>
using namespace std;
using namespace cui;


int main(){
  bool done = false;
  int key = 0;
  int i = 0;
  CDialog Screen;
  for(int k=1;k<console.getRows();k+=2){
    for(int m=0;m<console.getCols()-10;m+=10){
      Screen<<new CLabel((i=!i)?"Hello": "Hi",k, m, 9);
    }
  }
  Screen.draw();
  CDialog D(&Screen, 5, 10, 50, 15, true, "+-+|+-+|");
  CLabel L("A Non-dynamic Label goes here",5, 4);

  Screen<<new CLabel("Dialog and Label Tester", 0, 0);

  D.add(new CLabel("Testing Read Only Dialog",1, 12));
  D << new CLabel("Another Dyanmic label goes here, and I want to make sure it will be trimmed", 3, 3, 45) << L;
  int mesIndx = D.add(new CLabel(7, 5, 40));
  D << new CLabel("ESC to exit, F6 to Move, other to loop", 9, 3);
  D[mesIndx].set("Setting the message to see what happens");
  Screen.draw();
  D.draw();
  while(!done){
    key = D.edit(mesIndx+1);
    i++;
    sprintf((char*)D[mesIndx].data(), "LOOP No: %d", i);    
    switch(key){
    case ESCAPE:
      done = true;
      break;
    case F(6):
      D.move();
      break;
    }
  }
  D.hide();
  console.pause();
  return 0;
}

