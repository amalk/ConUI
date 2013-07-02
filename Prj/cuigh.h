#ifndef ___CUIGH_H__
#define ___CUIGH_H__
namespace cui{ 
  #ifndef _CRT_SECURE_NO_DEPRECATE
  #define _CRT_SECURE_NO_DEPRECATE
  #endif
  #ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
  #endif
 
  #define C_INITIAL_NO_FIELDS 50
  #define C_DIALOG_EXPANSION_SIZE 20
  #define C_BUTTON_HIT 1
  #define C_MAX_LINE_CHARS  (1024u)

  #define C_REFRESH -2
  #define C_FULL_FRAME -1
  #define C_NO_FRAME 0

  #define C_BORDER_CHARS  "/-\\|/-\\|"
 
  
  enum CDirection {centre, left, right, up, down};


  enum MessageStatus{ClearMessage,SetMessage};


  #ifdef NO_HELPFUNC
  # undef NO_HELPFUNC
  #endif
  #define NO_HELPFUNC ((void(*)(MessageStatus, CDialog&))(0))
  #ifdef NO_VALDFUNC
  # undef NO_VALDFUNC
  #endif
  #define NO_VALDFUNC ((bool(*)(const char*, CDialog&))(0))


  #define C_MAX_LINE_CHARS  (1024u)
  #define C_INITIAL_NUM_OF_LINES (100u)
}
#endif

