// Console Input Output Library Tester program for CDialog and CMenuItem
// Test7MenuItem.cpp
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
#include "clabel.h"
#include "cdialog.h"
#include "cmenuitem.h"
using namespace cui;

int main()
{
    int insert = 1;
    int key;
    bool done = false;
    char si[] = "Selected Index:  ";
    CDialog Screen;
    CDialog FD(&Screen, 5, 5, 35, 9, true);
    CMenuItem m1(false, "()", "One", 2, 2, 7);
    CMenuItem m2(true, "()", "Two", 3, 2, 7);
    CMenuItem m3(false, "()", "Three", 4, 2, 7);
    CMenuItem* m[3] = {&m1, &m2, &m3};
    Screen << new CLabel("Esc: EXIT, Enter/Space: s F6:  move,", 0, 0);
    int i = 0;

    for(int k = 1; k < console.getRows(); k += 2)
    {
        for(int m = 0; m < console.getCols() - 10; m += 10)
        {
            Screen << new CLabel((i = !i) ? "Hello" : "Hi", k, m, 9);
        }
    }

    Screen.draw();
    FD << m1 << m2 << m3 << new CLabel(5, 2, 20);
    FD.draw();

    while(!done)
    {
        key = FD.edit();

        switch(key)
        {
        case ESCAPE:
            done = true;
            break;

        case F(6):
            FD.move();
            break;

        case ENTER:
        case SPACE:
            for(int i = 0; i < 3; i++)
            {
                if(&FD.curField() == m[i])
                {
                    si[16] = '0' + i;
                    FD[3].set(si);
                }
                else
                {
                    m[i]->selected(false);
                }
            }

            break;
        }
    }

    return 0;
}

