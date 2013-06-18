#ifndef _FS_CONSOLE_H_
#define _FS_CONSOLE_H_

#include "bconsole.h"


namespace cui{

    class Console: public bio::BConsole{
        static bool _insertMode;

    public:
        void strdsp(const char* str, int row, int col, int len = 0, int curPosition = -1);
        int  stredit(char* str, int row, int col, int fieldLength, int maxStrLength, 
                     int* strOffset=(int*)0, int* curPosition=(int*)0,  
                     bool InTextEditor = false, bool ReadOnly = false, bool& insertMode = _insertMode);
        static unsigned int _tabsize;
    };

    Console& operator>>(Console& cn, int& ch);
<<<<<<< HEAD
=======

    Console& operator<<(Console& cn, char ch);

    Console& operator<<(Console& cn, const char* str);

	void tabFunction(char* str, int& strLength, int maxStrLength, int& position, int& offset, unsigned int tabsize, const int fieldLength, const bool isInsert);

    extern Console console;
}









>>>>>>> 8f8edb7f3a1344b06890c4ba3bcf9f4e42929988

    Console& operator<<(Console& cn, char ch);

    Console& operator<<(Console& cn, const char* str);

	void tabFunction(char* str, int& strLength, int maxStrLength, int& position, int& offset, unsigned int tabsize, const int fieldLength, const bool isInsert);

    extern Console console;
}


#endif