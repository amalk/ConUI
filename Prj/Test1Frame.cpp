// Console UI Testers
// Test1Frame.cpp
//
// Fardad Soleimanloo, Chris Szalwinski
// Oct 10 2012
// Version 0.9



#include "cframe.h"
#include "console.h"

using namespace cui;
int main()
{
    bool done = false;
    int key = 0;
    int i = 0;
    CFrame Cont;
    CFrame B(5, 10, 50, 15, true, "+-+|+-+|", &Cont);
    CFrame C(5, 10, 20, 5, true, C_BORDER_CHARS, &B);
    Cont.draw();

    for(int k = 0; k < console.getRows(); k += 2)
    {
        for(int m = 0; m < console.getCols() - 10; m += 10)
        {
            console.setPos(k, m);
            i = !i;
            console << (i ? "Hello" : "Hi");
        }
    }

    B.draw();
    C.draw();
    console.strdsp("Press any key...", 0, 0, 0);
    console.pause();

    while(!done)
    {
        console.strdsp("ESC: exit, F6: Move Container, F7: Move Inner border", 0, 0, 0);
        key = console.getKey();

        switch(key)
        {
        case ESCAPE:
            done = true;
            break;

        case F(6):
            B.move();
            C.draw();
            break;

        case F(7):
            C.move();
            break;
        }
    }

    return 0;
}



