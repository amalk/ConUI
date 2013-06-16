#include "console.h"

namespace cui{
    bool Console::_insertMode = true;
    unsigned int Console::_tabsize = 4;

    void Console::strdsp(const char* str, int row, int col, int len, int curpos){
        int i = 0;
        setPos(row, col);
        if(len){
            for(;i<len && str[i]; i++){
                putChar(str[i]);
            }
            for(;i<len; i++){
                putChar(' ');
            }
        }
        else{
            for(;str[i]; i++){
                putChar(str[i]);
            }
        }
        if(curpos >= 0){
            setPos(row, col + curpos);
        }
    }

    int Console::stredit(char *str, int row, int col, int fieldLength, int maxStrLength,
        int* strOffset, int* curPosition,
        bool InTextEditor, bool ReadOnly, bool& insertMode ){
            bool done = false;
            int localOffset = 0;
            int localCurPos = 0;
            int key = 0;
            int i = 0;
            unsigned int strLength = bio::strlen(str);

            // *curPosition = 100;
            //*strOffset = 100;
            // start - initial corrections

            if(strOffset == (int*)0){
                strOffset = &localOffset;
            }
            else if(*strOffset > strLength){
                //*strOffset = strLength;
            }

            if(curPosition == (int*)0){
                curPosition = &localCurPos;
            }
            /*else if(*curPosition > fieldLength) {
                *strOffset = strLength - fieldLength;
                *curPosition = fieldLength - 1;
            }*/

            // end - initial corrections


            char* strOriginal = new char [strLength + 1];
            bio::strcpy(strOriginal, str);

            while(!done){
                strdsp(str+*strOffset, row, col, fieldLength, *curPosition);

                key = console.getKey();

                switch(key){
                case LEFT:
                    if(*curPosition){
                        (*curPosition)--;
                    }
                    else if(*strOffset){
                        (*strOffset)--;
                    }
                    break;
                case RIGHT:
                    if(*curPosition != fieldLength - 1){
                        (*curPosition)++;
                    }
                    else if(*strOffset + fieldLength < (signed int)strLength + 1){
                        (*strOffset)++;
                    }
                    //(strLength == maxStrLength) && *strOffset && (*strOffset)--;
                    break;
                case HOME:
                    *strOffset = *curPosition = 0;
                    break;
                case END:
                    if(strLength > fieldLength){
                        *strOffset = strLength - fieldLength + 1;
                    }    
                    *curPosition = strLength - *strOffset;
                    (strLength == maxStrLength) && (*strOffset)--;
                    break;
                case ESCAPE:
                    done = true;
                    break; 

                case BACKSPACE:
                    for(i=-1; str[*strOffset+*curPosition + i] ; i++){
                        str[*strOffset+*curPosition + i] = str[*strOffset+*curPosition + 1 + i];
                    }
                    (*curPosition)--;
                    break;
                case DEL:
                    for(i=0; str[*strOffset+*curPosition + i] ; i++){
                        str[*strOffset+*curPosition + i] = str[*strOffset+*curPosition + 1 + i];
                    }
                    break;
                case INSERT:
                    Console::_insertMode = !Console::_insertMode;
                    break;
                case ENTER:
                case F(1):
                case F(2):
                case F(3):
                case F(4):
                case F(5):
                case F(6):
                case F(7):
                case F(8):
                case F(9):
                case F(10):
                case F(11):
                case F(12):
                    done = true;
                    break;
                default:
                    if(key>= ' ' && key <='~'){
                        if(Console::_insertMode){
                        }
                        else{
                            str[*strOffset+*curPosition] = key;
                            (*curPosition)++;
                        }
                    }
                    break;
                }
            }
            
            (key == ESCAPE) && bio::strcpy(str, strOriginal);

            delete [] strOriginal;

            return key;
    }

    Console console;

}