#include "console.h"
namespace cui{

  void Console::dspstr(const char* str, int row, int col, int len){
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
  }











Console console;




}