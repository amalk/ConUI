// Console Input Output Library Tester program for CMenu
// Test10Menu.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9

#include "cstring"
#include "cstdio"
using namespace std;

#include "cuigh.h"
#include "cframe.h"
#include "cdialog.h"
#include "clabel.h"
#include "cbutton.h"
#include "clineedit.h"
#include "cmenu.h"
#include "console.h"


using namespace cui;


bool Yes(const char* message, CDialog* owner);
void Help(CDialog* owner);
bool Quit(CDialog& owner);
int ShowHelp(CDialog& owner);
int main() {
  bool insert = true;
  int key;
  int i = 1;
  bool done = false;
  char str[100];
  CDialog Screen;
  CDialog FM(&Screen,2, 5, 60, 20, true);
  int fn = 0;

  Screen<< new CLabel("F1: HELP ", 0, 0);
  for(int k=1;k<console.getRows();k+=2){
    for(int m=0;m<console.getCols()-10;m+=10){
      Screen<<new CLabel((i=!i)?"Hello": "Hi",k, m, 9);
    }
  }
  Screen.draw();
  CMenu menu("Options", "<>", 4, 4, 15, 6, false);
  CLabel Mes(13,2,55);
  menu.add("Go Back");
  menu.add("Option One");
  menu.add("Second Option");
  menu.add("Option Three", CMenu::Select);
  menu.add("The Fourth Option");
  menu.add("And The Fifth");
  menu.add("The sixth");
  menu.add("And finally The seventh");
  CMenu AppMenu1("File", "[]", 0, 1, 15, 6, true);
  AppMenu1<<"Load"<<"Save"<<"Help"<<CMenu::Select<<"Quit";
  CMenu AppMenu2("Edit", "[]", 0, 7, 20, 8, true);
  AppMenu2<<"Some option here"<<"Another one here"<<"And yet one more here"<<"How about another one here"<<"Any More left?"<<"Just One more"<<"and yet another"<<"Finaly the last";
  FM<<new CLabel("Name:", 2, 2)
    <<new CLineEdit(1, 10, 22, 40, &insert,true)
    <<menu<<Mes
    <<new CLabel("Phone Number:", 16,2)
    <<new CLineEdit(15, 16, 15, 12, &insert,true);
  int ixAppMenu1 = FM.add(AppMenu1);
  int ixAppMenu2 = FM.add(AppMenu2);
  FM.draw();
  while(!done){
    key = FM.edit(fn);
    switch(key){
      case F(1):
        fn = ShowHelp(FM);
        break;
      case F(2):
        fn = ixAppMenu1 + 1;
        break;
      case F(3):
        fn = ixAppMenu2 + 1;
        break;
      case ESCAPE:
      case F(10):
        if(Quit(FM)){
          done = 1;
        }
        break;
      case SPACE:
        fn = FM.curIndex() + 1;
        if(&FM.curField() == &menu){  // menu active
          if(menu.selectedIndex() == 0){// go back
            fn = FM.curIndex();
          }
          sprintf_s(str,"Sel Idx: %d, Text: %s", menu.selectedIndex(), menu.selectedText());
          Mes.set(str);
          Mes.draw();
        }
        if(&FM.curField() == &AppMenu1){ // AppMenu1 active
          switch(AppMenu1.selectedIndex()){
          case 2: // help
            ShowHelp(FM);
            break;
          case 3: // quit
            if(Quit(FM)){
              done = 1;
            }
            break;
          }
        }
        break;
      case F(6):
        FM.move();
        break;
    }

  }
  return 0;
}
int ShowHelp(CDialog& owner){
   Help(&owner);
    return owner.curIndex() + 1;
}

bool Quit(CDialog& owner){
    return Yes("Do you really want to quit?", &owner);
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
  CDialog help(owner,(console.getRows()- 10)/2, (console.getCols()-50)/2, 50, 14, true);

  help<<new CLabel("F1 Key: Open this window.", 2, 3)
      <<new CLabel("F2: File Menu", 4, 3,36)
      <<new CLabel("F3: Edit Menu", 6, 3,36)
      <<new CLabel("F6: move", 8, 3,36)
      <<new CLabel("F10, Escape Key: Exit the test program.", 10, 3);
  help.edit(C_FULL_FRAME);
  help.hide();
}

