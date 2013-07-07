// Console Input Output Library Tester program for CDialog and CButton
// Test4Button.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9


#include "cuigh.h"
#include "console.h"
#include "cframe.h"
#include "cdialog.h"
#include "clabel.h"
#include "cbutton.h"
#include <cstdio>
using namespace std;
using namespace cui;

int main(){
  bool done = false;
  int key = 0;
  int i = 0;
  int fn = C_FULL_FRAME;
  for(int k=0;k< cui::console.getRows();k+=2){
    for(int m=0;m<console.getCols()-10;m+=10){
      console.strdsp((i=!i)?"Hello":"Hi", k, m);
    }
  }
  i = 100;
  CDialog App;
  CDialog D(&App, 5, 10, 50, 15, true, "+-+|+-+|");
  CButton Inc("Increase",9,10);
  CButton Dec("Decrease",9,30);
  D.add(new CLabel("Testing Buttons (bordered)",1, 12,30));
  D.add(new CLabel("Press F10 to toggle button borders visiblity",3,3));
  D.add(new CLabel("Press F6 to move ",4,15));
  int mesIndx = D.add(new CLabel(7, 24, 10));
  int incNum = D.add(Inc) + 1;
  int decNum = D.add(Dec) + 1;
  D << new CLabel("Press Escape to exit", 12, 15);
  D[mesIndx].set("100");
  while(!done){
    key = D.edit(fn);
    switch(key){
    case ESCAPE:
      done = true;
      break;
    case F(10):
      Inc.visible(!Inc.visible());
      Dec.visible(!Dec.visible());
      D[0].set(Inc.visible() ? "Testing Buttons (bordered)" : "Testing Buttons (no border)");
      D.refresh();
      break;
    case F(6):
      D.move();
      break;
    case C_BUTTON_HIT:
      if(&D.curField() == &Inc){
        i++;
        fn = incNum;
      }
      else{
        i--;
        fn = decNum;
      }
      sprintf((char*)D[mesIndx].data(),"%d",i);
      D[mesIndx].draw();
      break;
    }
  }
  D.hide();
  console.pause();
  return 0;
}
