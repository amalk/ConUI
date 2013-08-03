#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "bconsole.h"

namespace cui
{

class Console: public bio::BConsole
{
    static bool _insertMode;

public:
    void strdsp(const char* str, int row, int col, int len = 0, int curPosition = -1);
    int  stredit(char* str, int row, int col, int fieldLength, int maxStrLength,
                 int* strOffset = (int*)0, int* curPosition = (int*)0,
                 bool InTextEditor = false, bool ReadOnly = false, bool& insertMode = _insertMode);
    static unsigned int _tabsize;
};

Console& operator>>(Console&, int&);
Console& operator<<(Console&, char);
Console& operator<<(Console&, const char*);

void tabFunction(char* str, int& strLength, int maxStrLength, int& position, int& offset,
                 unsigned int tabsize, const int fieldLength, const bool isInsert);

extern Console console; // console object - external linkage
}















#endif