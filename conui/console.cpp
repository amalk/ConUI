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
            int strLength = bio::strlen(str);

            //*curPosition = 100;
            //*strOffset = 100;
            // start - initial corrections

            if(strOffset == (int*)0){
                strOffset = &localOffset;
            }
            else if(*strOffset > strLength){
                *strOffset = strLength;
            }

            if(curPosition == (int*)0){
                curPosition = &localCurPos;
            }
            else if(*curPosition > fieldLength || *curPosition > strLength){
                *curPosition = strLength - *strOffset;
                if(*curPosition >= fieldLength)
                    *curPosition = fieldLength - 1;
            }

            // end - initial corrections


            char* strOriginal = new char [strLength + 1];
            bio::strcpy(strOriginal, str);

            while(!done){
                strdsp(str+*strOffset, row, col, fieldLength, *curPosition);
                key = console.getKey();
                strLength = bio::strlen(str);

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
                    if((*curPosition != fieldLength - 1) && (str[*curPosition + *strOffset])){
                        (*curPosition)++;
                    }
                    else if(*strOffset + fieldLength < strLength + 1){
                        (*strOffset)++;
                    }
                    break;
                case HOME:
                    *strOffset = *curPosition = 0;
                    break;
                case END:
                    if(*strOffset + fieldLength < strLength){
                        *strOffset = strLength - fieldLength + 1;
                    }
                    *curPosition = strLength - *strOffset;
                    (strLength == maxStrLength) && (*strOffset)--;
                    break;
                case ESCAPE:
                    bio::strcpy(str, strOriginal);
                    delete[] strOriginal;
                    done = true;
                    break; 
                case BACKSPACE:
                    if(*curPosition >= 1 || (*curPosition == 1 && *strOffset == 0)){
                        for(i=-1; str[*strOffset+*curPosition + i] ; i++){
                            str[*strOffset+*curPosition + i] = str[*strOffset+*curPosition + 1 + i];
                        }
                        (*curPosition)--;
                    }
                    if(*curPosition == 0 || *curPosition == 1){
                        if(*strOffset >= (signed int)_tabsize) {
                            (*strOffset) -= _tabsize;
                            (*curPosition) += _tabsize;
                        }
                        else{
                            (*curPosition) += *strOffset;
                            (*strOffset) = 0;
                        }
                    }
                    break;
                case DEL:
                    for(i=0; str[*strOffset+*curPosition + i] ; i++){
                        str[*strOffset+*curPosition + i] = str[*strOffset+*curPosition + 1 + i];
                    }
                    break;
                case INSERT:
                    Console::_insertMode = !Console::_insertMode;
                    insertMode = !insertMode;
                    break;
                case ENTER:
                case UP:
                case DOWN:
                case PGUP:
                case PGDN:
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
                    if(key >= ' ' && key <= '~'){
                        if(insertMode){
                            if(strLength < maxStrLength){
                                for(i = strLength; i != *curPosition + *strOffset; i--){
                                    str[i] = str[i - 1];
                                }
                                strLength++;
                                str[*strOffset+*curPosition] = key;
                                if(*curPosition != fieldLength - 1){
                                    (*curPosition)++;
                                }
                                else if(*curPosition == fieldLength - 1 && str[*strOffset + *curPosition] != '\0'){
                                    (*strOffset)++;
                                }
                            }
                        }
                        else{
                            str[*strOffset+*curPosition] = key;
                            if(*curPosition != fieldLength - 1){
                              (*curPosition)++;
                            }
                            else if(*curPosition == fieldLength - 1 && strLength < maxStrLength){
                                  (*strOffset)++;
                                  strLength++;
                            } 
                        }
                    }
                    str[strLength] = '\0';
                    break;
                  }
            }

            return key;
    }

    Console& operator>>(Console& cn, int& ch){
        ch = cn.getKey();
        return cn;
    }

    Console& operator<<(Console& cn, char ch){
        cn.putChar(ch);
        return cn;
    }

    Console& operator<<(Console& cn, const char* str){
        for(int i = 0; str[i]; i++)
            cn.putChar(str[i]);
        return cn;
    }

    Console console;

}