// Console Input Output Library Tester program for CDialog and CLineEdit
// Test3DialogAndLineEdit.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9

#include "cuigh.h"
#include "console.h"
#include "cframe.h"
#include "cdialog.h"
#include "clabel.h"
#include "clineedit.h"
#include <cstdio>

using namespace std;
using namespace cui;

int main()
{
    bool done = false;
    int key = 0;
    int i = 0;
    bool insert = true;
    char str[81] = "I want to edit this thing!";

    for(int k = 0; k < console.getRows(); k += 2)
    {
        for(int m = 0; m < console.getCols() - 10; m += 10)
        {
            console.strdsp((i = !i) ? "Hello" : "Hi", k, m);
        }
    }

    CDialog App;
    CDialog D(&App, 5, 10, 50, 15, true, "+-+|+-+|");
    CLabel L("Enter some text down here:", 6, 4);
    App << new CLabel("Dialog and Line Editor Tester", 0, 0);
    D.add(new CLabel("Testing Label and Line edit", 1, 12));
    D << new CLabel("Name: ", 4, 3)
      << new CLineEdit(4, 9, 20, 40, &insert) << L
      << new CLineEdit(str, 7, 4, 40, 80, &insert, true);
    int mesIndx = D.add(new CLabel(10, 5, 40));
    D << new CLabel("Press ESC or F2 to exit, press F6 to Move", 2, 3);
    D[mesIndx].set("Setting the message to see what happens");
    D.draw();

    while(!done)
    {
        key = D.edit(mesIndx + 1);
        i++;
        sprintf((char*)D[mesIndx].data(), "LOOP No: %d", i);

        switch(key)
        {
        case ESCAPE:
        case F(2):
            done = true;
            break;

        case F(3):
            D[mesIndx].set("Setting the message to something more than 40 characters to see what happens");
            break;

        case F(6):
            D.move();
            break;
        }
    }

    console.clear();
    console.strdsp("First Lineedit data:", 10, 1);
    console.strdsp((char*)D[2].data(), 11, 0);
    console.strdsp("Second Linedit data:", 14, 1);
    console.strdsp((char*)D[4].data(), 15, 0);
    console.pause();
    return 0;
}
