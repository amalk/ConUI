// Console Input Output Library Tester program for CText
// Test8Text.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9


#include "cuigh.h"
#include "cframe.h"
#include "cdialog.h"
#include "clabel.h"
#include "cveditline.h"
#include "cbutton.h"
#include "ctext.h"
#include <cstring>
#include <cctype>
#include <iostream>
using namespace std;
#include "console.h"


using namespace cui;
bool Yes(const char* message, CDialog* owner);
void Help(CDialog* owner);
void PhoneHelp(MessageStatus st, CDialog& owner);
void LastNameHelp(MessageStatus st, CDialog& owner);
bool ValidPhone(const char* ph , CDialog& owner);

int main() {
  char str[32768]="Homer Jay Simpson, the patriarch of the Simpson household on the"
              " Fox series\n\"The Simpsons\" is a childish, lazy man, whose hobbies"
              " include eating donuts,\ndrinking Duff Beer, watching television, "
              "and sleeping.\nA victim of the \"Simpsons gene\" which allows for "
              "only Simpson women to possess the trait of intelligence,\nHomer is "
              "unfortunately as \"dumb as a chimp\" according to his father,\nAbe "
              "Simpson.\nHowever, it is mainly through the analysis of his simplistic "
              "thoughts\nand nature, that one can gain a real perspective on Homer’s "
              "complex personality.\n\n"
              "Spending most of his time in high school smoking,\ndrinking beer,\nand "
              "getting into trouble,\n(He even met his wife, Marge, while serving detention.)\n"
              "Homer’s lack of motivation for achievement grew with him into adulthood.\n"
              "The fat, balding character ends up working in Sector 7G of the Springfield "
              "Nuclear Power Plant,\nwhere he holds the record for most years worked at an"
              " entry level position.\nEven in the opening credits of the show,\nhe is seen"
              " negligently tossing aside radioactive waste as the whistle blows to end the"
              " workday.\nIn addition to his laziness at work, his sloth is also displayed"
              " in his free time\nwhere he is seen either lounging on his couch while"
              " indulging in donuts\nand watching anything that comes on television or"
              " drinking at Moe\'s Tavern\nwith his lifelong friends, "
              "Barney, Carl, Lenny, and Moe.";

  bool insert = 1;
  int key;
  bool done = false;
  CDialog Screen;
  CDialog TD(&Screen,2, 3, 70, 20, true);
  Screen<< new CLabel("F1: HELP ", 0, 0);
  int i =0;
  for(int k=1;k<console.getRows();k+=2){
    for(int m=0;m<console.getCols()-10;m+=10){
      Screen<<new CLabel((i=!i)?"Hello": "Hi",k, m, 9);
    }
  }
  CText txt(str,2,3,60,14,&insert);
  TD<<txt;
  Screen.draw();
  while(!done){
    key = TD.edit();
    switch(key){
      case F(1):
        Help(&TD);
        break;
      case F(2):
        txt.displayOnly(!txt.displayOnly());
        break;
      case F(10):
        if(Yes("Do you really want to quit?", &TD)){
          done = true;
        }
        break;
      case F(6):
        TD.move();
        break;
    }
  }
  char* data = (char*)TD[0].data();
  cout<<data<<endl;
  cin.get();
  delete[] data;
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
  CDialog help(owner,(console.getRows()- 10)/2, (console.getCols()-40)/2, 40, 12, true);
  
  help<<new CLabel("F1 Key: Open this window.", 2, 3)
      <<new CLabel("F2 Key: Toggle readonly mode", 4, 3)
      <<new CLabel("F6: Move", 6, 3)
      <<new CLabel("F10: Exit the test program.", 8, 3);
  help.edit(C_FULL_FRAME);
  help.hide();
}


