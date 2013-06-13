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

  int  Console::stredit(char *str, int row, int col, int fieldLength, int maxStrLength, 
        int* strOffset, int* curPosition,  
        bool InTextEditor, bool ReadOnly, bool& insertMode ){
    bool done = false;
    int localOffset = 0;
    int key = 0;
    int i = 0;
    if(strOffset == (int*)0){
      strOffset = &localOffset;
    }
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
        else{
         // console.alarm();  bad for your ears
        }
        break;
      case HOME:
        *strOffset = *curPosition = 0;
        break;
      case ESCAPE:
        done = true;
        break;
      case DEL:
        for(i=0; str[*strOffset+*curPosition + i] ; i++){
          str[*strOffset+*curPosition + i] = str[*strOffset+*curPosition + 1 + i]; 
        }
        break;
      case INSERT:
        Console::_insertMode = !Console::_insertMode;
        break;
      case F(1):
      case F(2):
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
    return key;
  }






Console console;




}