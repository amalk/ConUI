// Console Input Output Library Tester program for CValEdit
// Test5ValEdit.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9


#include "cuigh.h"
#include "console.h"
#include "cframe.h"
#include "cdialog.h"
#include "clabel.h"
#include "cveditline.h"
#include "cbutton.h"
#include <cstring>
#include <cctype>

using namespace std;
using namespace cui;

bool Yes(const char* message, CDialog* owner);
void Help(CDialog* owner, CDialog* helping);
void PhoneHelp(MessageStatus st, CDialog& owner);
void LastNameHelp(MessageStatus st, CDialog& owner);
bool ValidPhone(const char* ph , CDialog& owner);

int main()
{
    int key = 0;
    int i = 1;
    bool done = false;
    bool insert = true;
    char str[81] = "I want to edit this thing!";

    for(int k = 0; k < console.getRows(); k += 2)
    {
        for(int m = 0; m < console.getCols() - 10; m += 10)
        {
            console.strdsp((i = !i) ? "Hello" : "Hi", k, m);
        }
    }

    CDialog Screen;
    CDialog FD(&Screen, 5, 5, 70, 15, true);
    CLabel PhHelp(8, 34, 30);
    CLabel LnHelp(5, 37, 30);
    CLabel ErrMes(10, 2, 67);
    Screen << new CLabel("F1: HELP ", 0, 0);
    FD << new CLabel("Name:", 2, 2)
       << new CLineEdit(1, 10, 20, 40, &insert, true)
       << new CLabel("Lastname:", 5, 2)
       << new CValEdit(4, 13, 20, 40, &insert, NO_VALDFUNC, LastNameHelp, true)
       << new CLabel("Phone Number", 8, 2)
       << new CValEdit(7, 16, 15, 12, &insert, ValidPhone, PhoneHelp, true)
       << PhHelp
       << LnHelp
       << ErrMes
       << new CLabel("F1: help, ESCAPE: exit", 11, 2);
    FD.draw();

    while(!done)
    {
        key = FD.edit();

        switch(key)
        {
        case F(1):
            Help(&Screen, &FD);
            break;

        case ESCAPE:
            if(Yes("Do you really want to quit?", &Screen))
            {
                done = true;
            }

            break;

        case F(6):
            FD.move();
            break;
        }
    }

    return 0;
}

bool Yes(const char* message, CDialog* owner)
{
    int key;
    bool res = false;
    bool done = false;
    CButton bt_yes("Yes", 4, 4 , true, "     _  ");
    CButton bt_no("No", 4, 15 , true, "     _  ");
    CDialog YesNo(owner, (console.getRows() - 10) / 2, (console.getCols() - 40) / 2, 40, 10, true);
    YesNo << new CLabel(2, 2, 36) << bt_yes << bt_no;
    YesNo[0].set(message);
    YesNo.draw(C_FULL_FRAME);

    while(!done)
    {
        key = YesNo.edit();

        if(key == C_BUTTON_HIT)
        {
            res =  &YesNo.curField() == &bt_yes;
            done = true;
        }
        else if(key == F(6))
        {
            YesNo.move();
        }
    }

    YesNo.hide();
    return res;
}
void Help(CDialog* owner, CDialog* helping)
{
    CDialog help(owner, (console.getRows() - 10) / 2, (console.getCols() - 40) / 2, 40, 10, true);
    help << new CLabel(2, 3, 36)
         << new CLabel("Escape Key: Exit the test program.", 4, 3)
         << new CLabel("F1 Key: Open this window.", 6, 3);

    switch(helping->curIndex())
    {
    case 1:
        help[0].set("Enter the name here!");
        break;

    case 3:
        help[0].set("Enter the Last name here!");
        break;

    case 5:
        help[0].set("Enter Phone number: 999-999-9999");
    }

    while(help.edit(C_FULL_FRAME) == F(6))
    {
        help.move();
    }

    help.hide();
}

void PhoneHelp(MessageStatus st, CDialog& owner)
{
    if(st == ClearMessage)
    {
        owner[6].set("");
    }
    else
    {
        owner[6].set("Phone Format: 999-999-9999");
    }

    owner.draw(7);
}
void LastNameHelp(MessageStatus st, CDialog& owner)
{
    if(st == ClearMessage)
    {
        owner[7].set("");
    }
    else
    {
        owner[7].set("i.e. Middle name and Surname");
    }

    owner.draw(8);
}

bool ValidPhone(const char* ph , CDialog& owner)
{
    bool ok = true;
    int i;

    for(i = 0; ok && i < 3; ok = (bool)isdigit(ph[i]), i++);

    ok = ph[i++] == '-';

    for(; ok && i < 7; ok = (bool)isdigit(ph[i]), i++);

    ok = ph[i++] == '-';

    for(; ok && i < 12; ok = (bool)isdigit(ph[i]), i++);

    if(ok)
    {
        owner[8].set("");
    }
    else
    {
        owner[8].set("Invlid phone number, please use the specified phone number format!");
    }

    owner.draw(9);
    return ok;
}
