// Console Input Output Library Tester program for CDialog and CCheckMark
// Test6Check.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9

#include <cstring>
#include <cctype>
#include <iostream>
using namespace std;
#include "cuigh.h"
#include "console.h"
#include "cdialog.h"
#include "ccheckmark.h"
using namespace cui;

int main() {
  int insert = 1;
  int key;
  bool done = false;
  int i = 0;
  CDialog Screen;
  CDialog FD(&Screen,5, 5, 35, 9, true);
  CCheckMark chb(true,"[X]","Check Box", 3,3, 13);
  CCheckMark* chbptr; //Un-commnent line 43 and comment line 42 for alternative way of accessing the checkbox's falg
  Screen<< new CLabel("Esc: EXIT, F2: Reset Data, F6:  move,", 0, 0);
  for(int k=1;k<console.getRows();k+=2){
    for(int m=0;m<console.getCols()-10;m+=10){
      Screen<<new CLabel((i=!i)?"Hello": "Hi",k, m, 9);
    }
  }
  Screen.draw();
  FD<<chb
   <<new CLabel("Checked",3, 17, 15)
   <<new CCheckMark(false,"(O)","Radio Button", 5, 3,16, true)
   <<new CLabel("Not Checked", 5, 20, 13);
  FD.draw();
  chbptr = (CCheckMark*)&FD[2];
  while(!done){
    FD[1].set(chb.checked()?"Checked": "Not Checked");
    FD[3].set(bool(*((bool*)FD[2].data()))?"Checked": "Not Checked");
   // FD[3].set(chbptr->checked()?"Checked": "Not Checked");
    key = FD.edit();
    switch(key){
      case ESCAPE:
        done = true;
        break;
      case F(6):
        FD.move();
        break;
      case F(2):
        chb.checked(false);
        chbptr->checked(false);
        FD.draw();
        break;
    }
  }
  return 0;
}
