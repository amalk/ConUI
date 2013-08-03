// Console Input Output Library Tester program for CCheckList
// Test9CCheckList.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9


#include <cstring>
#include <cstdio>
#include "cuigh.h"
#include "console.h"
#include "cframe.h"
#include "cdialog.h"
#include "clabel.h"
#include "cbutton.h"
#include "clineedit.h"
#include "cchecklist.h"

using namespace std;
using namespace cui;

bool Yes(const char* message, CDialog* owner);
void Help(CDialog* owner);

int main() {
  bool insert = true;
  int key;
  int i = 1;
  bool done = false;
  CDialog Screen;
  CDialog FCL(&Screen,2, 5, 40, 20, true);


  Screen<< new CLabel("F1: HELP ", 0, 0);
  for(int k=1;k<console.getRows();k+=2){
    for(int m=0;m<console.getCols()-10;m+=10){
      Screen<<new CLabel((i=!i)?"Hello": "Hi",k, m, 9);
    }
  }
  Screen.draw();  
  CCheckList cl("[X]", 5, 4, 20, true);
  CLabel Mes("Radio Button List",4,4,35);
  cl.add("Option One");
  cl.add("Second Option");
  cl.add("Option Three", true);
  cl.add("The Fourth Option");
  cl.add("And The Fifth");
  cl.add("The sixth");
  cl.add("And finally The seventh");
  FCL<<new CLabel("Name:", 2, 2);
  int nameIx = FCL.add(new CLineEdit(1, 10, 22, 40, &insert,true));
  FCL<<cl<<Mes
    <<new CLabel("Phone Number:", 16,2);
  int phoneIx = FCL.add(new CLineEdit(15, 16, 15, 12, &insert,true));
  FCL.draw();
  while(!done){
    key = FCL.edit(FCL.curIndex()+1);
    switch(key){
      case F(1):
        Help(&FCL);
        break;
      case F(2):
      case ESCAPE:
        if(Yes("Do you really want to quit?", &FCL)){
          done = true;
        }
        break;
      case SPACE:
        break;
      case F(6):
        FCL.move();
        break;
      case F(10):
        cl.radio(!cl.radio());
        Mes.set(cl.radio()?"Radio Button List":"Check List");
        Mes.draw();
        break;
    }
  }
  console.clear();
  int r = 10;
  console.strdsp("Name: ", 3, 5);
  console.strdsp((char*)FCL[nameIx].data(),3, 12);
  console.strdsp("Phone: ", 4, 5);
  console.strdsp((char*)FCL[phoneIx].data(), 4, 13);
  for(unsigned int i=0;i<cl.length();i++){
    console.strdsp((bool)cl[i]?"Checked:":"Not Checked:", r+i, 5, 13); 
    console.strdsp((char*)cl[i],r+i, 19); 
  }
  console.strdsp("Press any key to exit....", console.getRows()-2, 5);
  console.pause();
  return 0;
}

bool Yes(const char* message, CDialog* owner){
  int key;
  bool res = false;
  bool done = false;
  CButton bt_yes("Yes", 4,4 ,true,"     _  ");
  CButton bt_no("No", 4,15 ,true,"     _  ");
  CDialog YesNo(owner,(console.getRows()- 10)/2, (console.getCols()-40)/2, 40, 10, true);
  YesNo<<new CLabel(2, 2, 36)<<bt_yes<<bt_no;
  YesNo[0].set(message);
  YesNo.draw(C_FULL_FRAME);
  while(!done){
    key = YesNo.edit();
    if(key == C_BUTTON_HIT){
      res =  &YesNo.curField() == &bt_yes;
      done = true;
    }
  }
  YesNo.hide();
  return res;
}
void Help(CDialog* owner){
  CDialog help(owner,(console.getRows()- 10)/2, (console.getCols()-40)/2, 40, 11, true);
  
  help<<new CLabel("F1 Key: Open this window.", 2, 3)
      <<new CLabel("F6: Move", 4, 3,36)
      <<new CLabel("F10: Toggle (Radio/Check)list", 6, 3,36)
      <<new CLabel("F2 & Escape: Exit the test program.", 8, 3);
  help.edit(C_FULL_FRAME);
  help.hide();
}

