#ifndef _FS_CONSOLE_H_
#define _FS_CONSOLE_H_

#include "bconsole.h"


namespace cui{


    class Console: public bio::BConsole{
      static bool _insertMode;

    public:
      void strdsp(const char* str, int row, int col, int len = 0, int curPosition = -1);
      int  stredit(char *str, int row, int col, int fieldLength, int maxStrLength, 
                   int* strOffset=(int*)0, int* curPosition=(int*)0,  
                   bool InTextEditor = false, bool ReadOnly = false, bool& insertMode=_insertMode );
      static unsigned int _tabsize;
    };

    Console& operator>>(Console&, int&);
    Console& operator<<(Console&, char);
    Console& operator<<(Console&, const char*);

    extern Console console; // console object - external linkage
}















#endif