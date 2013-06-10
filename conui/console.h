#ifndef _FS_CONSOLE_H_
#define _FS_CONSOLE_H_

#include "bconsole.h"


namespace cui{


    class Console: public bio::BConsole{
<<<<<<< HEAD
      static bool _insertMode;

    public:
      void strdsp(const char* str, int row, int col, int len = 0, int curPosition = -1);
      int  stredit(char *str, int row, int col, int fieldLength, int maxStrLength, 
                   int* strOffset=(int*)0, int* curPosition=(int*)0,  
                   bool InTextEditor = false, bool ReadOnly = false, bool& insertMode=_insertMode );
      static unsigned int _tabsize;
=======

    public:
      void dspstr(const char* str, int row, int col, int len = 0);
>>>>>>> 45024a30197c3ec8714c7865d8433e99a18d7383
    };

    extern Console console;
}















#endif