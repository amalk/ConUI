/*

   BConsole Input Output Library tester program
   cui_text.cpp

   Fardad Soleimanloo
   Jun 12, 2013
   Version 0.95


   This program may have bugs, if you find one please report it at


   This tester program is to be compiled with bconsole.h and bconsole.c to test your cui functions.

   Start testing your functions and correct its problems.

   you can comment define statements below to select individual tests but:


*/


/* Comment any of the following to do partial testing */

/*#define DO_ALL  /*  comment this to be able to do partial testing,
                     if this is not commented, all tests will happen
                     no matter what you comment below*/
/*#define DO_01
#define DO_02
#define DO_03
#define DO_04//       comment this to jump over all 09 tests
#define DO_0401
#define DO_0402
#define DO_0403
#define DO_0404
#define DO_0405
#define DO_0406
#define DO_0407
#define DO_0408
#define DO_0409
#define DO_0410
#define DO_0411
#define DO_0412
#define DO_0413
#define DO_0414
#define DO_0415
#define DO_0416
#define DO_0417
#define DO_0418
#define DO_0419
#define DO_0420
#define DO_0421
#define DO_0422
#define DO_0423
#define DO_0424*/
#define DO_5
//*/

#define _CRT_SECURE_NO_WARNINGS 1

#include <cstdarg>
#include <cstdio>
#include "console.h"
using namespace std;
using namespace cui;

#define _IOL_PROGRAM_TITLE "OOP344 20132 CUI tester"
#define RELEASE_VERSION "R0.95"


/* replace the following information with your own: */

#define TEAM_NAME "CUI testing program, No Team!"   

/* replace the above information with your own: */


#define WHITESPACE " \t\n\r"

#define PrnBox(r, c, w, h) PrintHollowBox(r, c, w,h, '+', '-', '|');

void PrnMessages(int row, int col, const char *name);
const char *keycode(int ch);
const char *keyname(int ch);
void PrintKeyInfo(int row, int col, int key);
void bio_printf(int row, int col, int len, char *format, ...);
void PrintHollowBox(int row, int col, int width, int height, char corner, char horizontal, char vertical);
void PrintLine(int row, int col, int length, int ch, int endings);
int isWhiteSpace(char ch);
int StrLen(const char *str);
int getLineLength(const char *src, int maxLen);
void PrnTextBox(int row, int col, int width, int height, const char *text);
int CheckIoEditMinMax(int curmin, int curmax, int curposIs, int insertShouldBe, int insertIs, int offmin, int offmax, int offsetIs);
int CheckIoEditVals(int curposShouldBe, int curposIs, int insertShouldBe, int insertIs, int offsetShouldBe, int offsetIs);
int Yes();
void resetStr(char* str, int len);
char *MyStrCpy(char* des, const char* src);

int runtest(int n);
void cls();


int welcome();
int test1();
int test2();
int test3();
int test4();
int test5();
int test6();
int test7();
int test8();
int test4();
int test5();

int tn = 0; /* test number */


int main(){
  int r;
  int c;
  int num = 0;
  int ok = 1;
  int done = 0;
  r = console.getRows();
  c = console.getCols();
  do{
    if(runtest(-1) && (r<24 || c<80)){
      console.setPos(1, 1);
      console<<"The row and column of the terminal";
      console.setPos(2, 1);
      console<<"must be minimum of 24 by 80.";
      console.setPos(3, 1);
      console<<"press any key to quit";
      console.pause();
    }
    else if(runtest(-1) && (r>30 || c > 100)){
      console.setPos(1, 1);
      console<<"The row and column of the terminal";
      console.setPos(2, 1);
      console<<"must be maximum of 30 by 100.";
      console.setPos(3, 1);
      console<<"press any key to quit";
      console.pause();
    }
    else{
  #ifdef DO_ALL
      if(runtest(0)){
        ok = welcome();
      }
  #endif
  #if defined(DO_ALL) || defined(DO_01)
      if(ok && runtest(1)){
        num++;
        ok = test1();
      }
  #endif
  #if defined(DO_ALL) || defined(DO_02)
      if(ok && runtest(2)){
        num++;
        ok = test2();
      }
  #endif
  #if defined(DO_ALL) || defined(DO_03)
      if(ok && runtest(3)){
        num++;
        ok = test3();
      }
  #endif
  #if defined(DO_ALL) || defined(DO_04)
      if(ok){
        ok = test4();
        num++;
      }
  #endif
  #if defined(DO_ALL) || defined(DO_5)
      if(ok && runtest(29)){
        ok = test5();
        num++;
      }
  #endif
      if(ok){
        num++;
      }
    }
    cls();
    console.setPos(10, 5);
    if(num == 6){
      console<<"Well done!";
      console.setPos(11, 5);
      console<<"You passed all the tests, wait for the final tester to submit assignment";
      done = 1;
    }
    else{
      console<<"Test terminated prematurely, run the last test again? (Y/N)";
      ok = 1;
      tn--;
      num--;
      if(!Yes()){
        console.setPos(11, 5);
        console<<"Keep working on your assignment until it meets all the requirements.";
        done = 1;
      }
    }
  }while(!done);
  console.pause();
  return 0;
}

int test5(){
  char str[81];
  int curpos = 0;
  bool insert = true;
  int offset = 0;
  int key = 0;
  bool isTextEditor = false;
  bool readonly = false;
  resetStr(str, 25);
  do{
    if(key == 't' || key == 'T'){
      resetStr(str, 25);
      curpos = 0;
      insert = 1;
      offset = 0;
    }
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 10: console.stredit() General Test, Test console.stredit anyway you can and make sure it does not crash.  "
                            "Press F5 to toggle InTextEditor, press F6 to toggle ReadOnly. " );
    PrnBox(10, 29, 22, 3);
    bio_printf(9, 28, -1, "TextEdtor: %d, ReadOnly: %d       ", isTextEditor, readonly);
    key = console.stredit(str,11,30, 20, 60, &offset, &curpos, isTextEditor, readonly,insert);
    (key == F(5)) && (isTextEditor = !isTextEditor);
    (key == F(6)) && (readonly = !readonly);
    if(key != F(5) && key != F(6)){
      bio_printf(13, 5, -1, "console.stredit() returned %s, %s", keyname(key), keycode(key));
      bio_printf(14, 5, -1, "Curpos: %d, insert: %d, offset: %d, TextEdtor: %d, ReadOnly: %d       ",curpos, insert, offset, isTextEditor, readonly);
      bio_printf(16, 5, -1, "Str value after stredit:");
      bio_printf(17, 5, -1, "         1         2         3         4         5         6");
      bio_printf(18, 5, -1, "123456789012345678901234567890123456789012345678901234567890");
      bio_printf(19, 5, -1, "%s",str);
      bio_printf(21, 5, -1, "What to do next? (E)xit, (R)epeat, Repea(T) and Reset the data:");
      key = console.getKey();
    }
  }while(key != 'e' && key != 'E');
  cls();
  PrnTextBox(1, 5, 70, 8, "Did everything work correctly?");
  return Yes();
}

void strdspText(int from, int offset, char test[][61]){
  int i;
  for(i=from;i<from+10 && i<20;i++){
    if(from + offset >= StrLen(test[i]))
      console.strdsp("", 10 + i- from, 10, 20);
    else
      console.strdsp(&test[i][from]+offset ,10 + i- from, 10, 20);
  }
  for(;i<from+10;i++){
    console.strdsp("", 10 + i- from, 10, 20);
  }
}


void resetStr(char* str, int len){
  int i=0;
  while(str[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz Garbage after data"[i])i++;
  str[len] = 0;
}

int CheckIoEditVals(int curposShouldBe, int curposIs, int insertShouldBe, int insertIs, int offsetShouldBe, int offsetIs){
  int i = 0;
  int ok = 1;
  if(curposShouldBe != curposIs){
    ok = 0;
    bio_printf(16,5,-1, "  curpos is %d, but should be %d", curposIs, curposShouldBe );
    i++;
  }
  if(insertShouldBe != insertIs){
    ok = 0;
    bio_printf(16 + i++ ,5,-1, "  insert is %d, but should be %d", insertIs , insertShouldBe );
  }
  if(offsetShouldBe != offsetIs){
    ok = 0;
    bio_printf(16 + i ,5,-1, "  offset is %d, but should be %d", offsetIs, offsetShouldBe );
  }
  if(!ok){
    bio_printf(15, 5, -1, "- console.stredit() was supposed to have the following values but it doesn't:");
  }
  return ok;
}
int CheckIoEditMinMax(int curmin, int curmax, int curposIs, int insertShouldBe, int insertIs, int offmin, int offmax, int offsetIs){
  int i = 0;
  int ok = 1;
  if(curmin > curposIs || curposIs > curmax){
    ok = 0;
    if(curmin == curmax){
      bio_printf(16,5,-1, "  curpos is %d, but should be %d", curposIs, curmin);
    }
    else{
      bio_printf(16,5,-1, "  curpos is %d, but should be betwee %d and %d", curposIs, curmin, curmax );
    }
    i++;
  }
  if(insertShouldBe != insertIs){
    ok = 0;
    bio_printf(16 + i++ ,5,-1, "  insert is %d, but should be %d", insertIs , insertShouldBe );
  }
  if(offmin > offsetIs || offsetIs > offmax){
    ok = 0;
    if(offmin == offmax){
      bio_printf(16 + i ,5,-1, "  offset is %d, but should be %d", offsetIs, offmin);
    }
    else{
      bio_printf(16 + i ,5,-1, "  offset is %d, but should be  between %d and %d", offsetIs, offmin, offmax );
    }
  }
  if(!ok){
    bio_printf(15, 5, -1, "- console.stredit() was supposed to have the following values but it doesn't:");
  }
  return ok;
}

int CheckReturnedKey(int KeyShouldBe, int KeyIs){
  int ok = 1;
  if(KeyShouldBe != KeyIs){
    bio_printf(13, 5, -1, "- console.stredit() was supposed to return %s, %s", keyname(KeyShouldBe), keycode(KeyShouldBe));
    bio_printf(14, 5, -1, "  but instead it returned: %s, %s", keyname(KeyIs), keycode(KeyIs));
    ok = 0;
  }
  return ok;
}

int CheckStrValue(const char* strShoulBe, const char* strIs){
  int diff = 0;
  int i = 0;
  while(!diff && strShoulBe[i]){
    diff = strShoulBe[i] - strIs[i];
    i++;
  }
  if(!diff) diff = strShoulBe[i] - strIs[i];
  if(diff){
    bio_printf(19, 5, -1, "- console.stredit() does not have correct value for its data:");
    bio_printf(20, 0, 75, "Shoule be:%s",strShoulBe);
    bio_printf(21, 0, 75, " Value is:%s",strIs);
  }
  return !diff;
}

void pause(int ok){
  bio_printf(22, 5, -1, " %s, hit any key to continue", ok? "PASSED": "FAILED");
  console.pause();
}

int test4(){
  char str[81];
  int curpos = 40;
  bool insert = true;
  int offset = 0;
  int key;
  int ok = 1;
  resetStr(str, 25);
  cls();
#if defined(DO_ALL) || defined(DO_0401)
  if(runtest(4)){
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.1: console.stredit() initial corrections, Curpos: The curpos is set to 40 that is invalid and it should be corrected to 19. "
                            "HIT ENTER ONLY to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 60, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(19, curpos, 1, insert, 0, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXY", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0402)
  if(ok && runtest(5)){
    resetStr(str, 5);
    offset = 10;
    curpos = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.2: console.stredit() initial corrections, Offset: The offset is set to 10 that is invalid and it should be corrected to 5. "
                            "HIT ENTER ONLY to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 60, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(0, curpos, 1, insert, 5, offset) && ok;
    ok = CheckStrValue("ABCDE", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0403)
  if(ok && runtest(6)){
    resetStr(str, 5);
    offset = 0;
    curpos = 10;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.3: console.stredit() initial corrections, Curpos: The curpos is set to 10 that is invalid and it should be corrected to 5. "
                            "HIT ENTER ONLY to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 60, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(5, curpos, 1, insert, 0, offset) && ok;
    ok = CheckStrValue("ABCDE", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0404)
  if(ok && runtest(7)){
    resetStr(str, 5);
    offset = 0;
    curpos = 0;
    insert = 1;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.4: console.stredit() toggling insert: The insert is set to 1, hit insert key once and then hit ENTER only to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 60, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(0, curpos, 0, insert, 0, offset) && ok;
    ok = CheckStrValue("ABCDE", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0405)
  if(ok && runtest(8)){
    resetStr(str, 5);
    offset = 0;
    curpos = 4;
    insert = 1;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.5: console.stredit() deleting chars : Hit Del key twice and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 60, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(4, curpos, 1, insert, 0, offset) && ok;
    ok = CheckStrValue("ABCD", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0406)
  if(ok && runtest(9)){
    resetStr(str, 20);
    offset = 0;
    curpos = 10;
    insert = 1;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.6: console.stredit() inserting chars in insert mode : Hit 'a', 'b' and 'c' and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 22, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(12, curpos, 1, insert, 0, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJabKLMNOPQRST", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0407)
  if(ok && runtest(10)){
    resetStr(str, 20);
    offset = 0;
    curpos = 10;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.7: console.stredit() inserting chars in overstrike mode : Hit 'a', 'b' and 'c' and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 20, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(13, curpos, 0, insert, 0, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJabcNOPQRST", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0408)
  if(ok && runtest(11)){
    resetStr(str, 25);
    offset = 5;
    curpos = 10;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.8: console.stredit() Home key : Hit Home key and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(0, curpos, 0, insert, 0, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXY", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0409)
  if(ok && runtest(12)){
    resetStr(str, 25);
    offset = 10;
    curpos = 10;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.9: console.stredit() End key : Hit END and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(15, curpos, 0, insert, 10, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXY", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0410)
  if(ok && runtest(13)){
    resetStr(str, 25);
    offset = 0;
    curpos = 10;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.10: console.stredit() End key : Hit END and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(19, curpos, 0, insert, 6, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXY", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0411)
  if(ok && runtest(14)){
    resetStr(str, 45);
    offset = 20;
    curpos = 3;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.11: console.stredit() Scrolling right : Hit left Arrow 4 times and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditMinMax(0,10, curpos, 0, insert,9,19, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0412)
  if(ok && runtest(15)){
    resetStr(str, 45);
    offset = 10;
    curpos = 3;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.12: console.stredit() Scrolling right : Hit left Arrow 4 times and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditMinMax(0,10, curpos, 0, insert,0,9, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0413)
  if(ok && runtest(16)){
    resetStr(str, 45);
    offset = 10;
    curpos = 16;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.13: console.stredit() Scrolling left : Hit right Arrow 4 times and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditMinMax(10,19, curpos, 0, insert,11,21, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0414)
  if(ok && runtest(17)){
    resetStr(str, 30);
    offset = 10;
    curpos = 16;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.14: console.stredit() Scrolling left : Hit right Arrow 6 times and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditMinMax(30-offset,30-offset, curpos, 0, insert,11,21, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXYZabcd", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0415)
  if(ok && runtest(18)){
    resetStr(str, 45);
    offset = 10;
    curpos = 9;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.15: console.stredit() Backspace : Hit backspace  and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditVals(8, curpos, 0, insert, 10, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRTUVWXYZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0416)
  if(ok && runtest(19)){
    resetStr(str, 45);
    offset = 10;
    curpos = 3;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.16: console.stredit() Backspace : Hit Backspace 4 times and then hit ENTER to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ENTER, key);
    ok = CheckIoEditMinMax(0,10, curpos, 0, insert,0,9, offset) && ok;
    ok = CheckStrValue("ABCDEFGHINOPQRSTUVWXYZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_04)
  if(ok && runtest(20)){
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4 (continued), console.stredit() InTextEditor set to true. Press any key to continue:");
    console.pause();
  }
#endif
#if defined(DO_ALL) || defined(DO_0417)
  if(ok && runtest(21)){
    resetStr(str, 5);
    offset = 10;
    curpos = 0;
    insert  = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.17: console.stredit() initial corrections, Offset: The offset is set to 10 that is invalid and it should be corrected to 5. "
                            "The function should terminate returning 0.");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 60, &offset, &curpos, 1,0,insert);
    ok = CheckReturnedKey(0, key);
    ok = CheckIoEditVals(0, curpos, 0, insert, 5, offset) && ok;
    ok = CheckStrValue("ABCDE", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0418)
  if(ok  && runtest(22)){
    resetStr(str, 25);
    offset = 5;
    curpos = 10;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.18: console.stredit() Home key : Hit Home key to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 1,0,insert);
    ok = CheckReturnedKey(0, key);
    ok = CheckIoEditVals(0, curpos, 0, insert, 0, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXY", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0419)
  if(ok && runtest(23)){
    resetStr(str, 25);
    offset = 0;
    curpos = 10;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.19: console.stredit() End key : Hit END  to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 1 ,0,insert);
    ok = CheckReturnedKey(0, key);
    ok = CheckIoEditVals(19, curpos, 0, insert, 6, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXY", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0420)
  if(ok && runtest(24)){
    resetStr(str, 45);
    offset = 20;
    curpos = 3;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.20: console.stredit() Scrolling right : Hit left Arrow 4 times (but stop if fucntion exits) to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 1, 0,insert);
    ok = CheckReturnedKey(0 , key);
    ok = CheckIoEditMinMax(0,Console::_tabsize, curpos, 0, insert,20-Console::_tabsize,20, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0421)
  if(ok && runtest(25)){
    resetStr(str, 45);
    offset = 10;
    curpos = 16;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.21: console.stredit() Scrolling left : Hit right Arrow 4 times (but stop if fucntion exits) to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 1 ,0,insert);
    ok = CheckReturnedKey(0, key);
    ok = CheckIoEditMinMax(19-Console::_tabsize,19, curpos, 0, insert,10,10+Console::_tabsize, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0422)
  if(ok && runtest(26)){
    char value[81];
    int lastIndex;
    resetStr(str, 45);
    MyStrCpy(value, str);
    offset = 10;
    curpos = 3;
    insert = 0;
    lastIndex = offset + curpos;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.22: console.stredit() Backspace : Hit Backspace 4 times (but stop if fucntion exits) to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 1 ,0,insert);
    ok = CheckReturnedKey(0, key);
    ok = CheckIoEditMinMax(0,Console::_tabsize, curpos, 0, insert,10 - Console::_tabsize ,10, offset) && ok;
    MyStrCpy(&value[offset + curpos], "NOPQRSTUVWXYZabcdefghijklmnopqrs");
    ok = CheckStrValue(value, str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0423)
  if(ok && runtest(27)){
    resetStr(str, 45);
    offset = 10;
    curpos = 10;
    insert = 1;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.23: console.stredit() Escape when InTextEditor = 0, hit HOME key, then 'a', 'b', 'c' and then ESCAPE to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 0, 0,insert);
    ok = CheckReturnedKey(ESCAPE, key);
    ok = CheckIoEditVals(10, curpos, 1, insert,10, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
#if defined(DO_ALL) || defined(DO_0424)
  if(ok  && runtest(28)){
    resetStr(str, 45);
    offset = 10;
    curpos = 10;
    insert = 0;
    cls();
    PrnTextBox(1, 5, 70, 8, "Test 4.24: console.stredit() Escape when InTextEditor = 1 , hit right arrow 2 times, then 'a', 'b', 'c' and then ESCAPE to test!");
    PrnBox(10, 29, 22, 3);
    key = console.stredit(str,11,30, 20, 50, &offset, &curpos, 1, 0, insert);
    ok = CheckReturnedKey(ESCAPE, key);
    ok = CheckIoEditVals(15, curpos, 0, insert,10, offset) && ok;
    ok = CheckStrValue("ABCDEFGHIJKLMNOPQRSTUVabcZabcdefghijklmnopqrs", str) && ok;
    pause(ok);
  }
#endif
  return ok;
}







int test3(){
  char str[] = "ABCDEFGHIJKLMNOP";
  cls();
  PrnTextBox(1,10,50,5, "Test 3: Testing console.strdsp(): make sure all the A:(your console.strdsp) "
                        "fields match exactly thier corresponding B:(Tester strdsp) fields;");
  bio_printf(8, 6, -1, "A:");
  PrnBox(7,10, 12, 3);
  console.strdsp(str, 8, 11, 10);
  bio_printf(11, 6, -1, "B:");
  PrnBox(10, 10, 12, 3);
  bio_printf(11, 11, 10, str);


  bio_printf(8, 26, -1, "A:");
  PrnBox(7, 30, 12, 3);
  console.strdsp(str, 8, 31, 0);
  bio_printf(11, 26, -1, "B:");
  PrnBox(10, 30, 12, 3);
  bio_printf(11, 31, -1, str);

  str[5] = 0;
  bio_printf(14, 6, -1, "A:");
  PrnBox(13,10, 12, 3);
  bio_printf(14,11,-1,"**********");
  console.strdsp(str, 14, 11, 10);
  bio_printf(17, 6, -1, "B:");
  PrnBox(16, 10, 12, 3);
  bio_printf(17,11,-1,"**********");
  bio_printf(17, 11, 10, str);

  bio_printf(14, 26, -1, "A:");
  PrnBox(13,30, 12, 3);
  bio_printf(14,31,-1,"**********");
  console.strdsp(str, 14, 31, 0);
  bio_printf(17, 26, -1, "B:");
  PrnBox(16, 30, 12, 3);
  bio_printf(17,31,-1,"**********");
  bio_printf(17, 31, -1, str);

  bio_printf(console.getRows()-3, 2, -1, "Do all the A and B fields match? ");
  return Yes();
}

int test2(){
  int
    done = 0,
    key = 0,
    row = 0,
    col = 0,
    escPressed = 0;
  cls();
  PrnTextBox(5,10,50,10, "Test 2: Hit all the keys and make sure they return the correct key value according to the platfrom. "
          "Move the 'X' around with the arrow keys, it should be able to move everywhere except the bottom-right corner. "
          "When you are done with testing, Hit ESCAPE or 'Q' key twice to exit the tester. Press anykey to start the tester.");
  key = console.getKey();
  cls();
    console.setPos(row, col);
    console.putChar('X');
    do{
    key = console.getKey();
    console.setPos(row, col);
    console.putChar(' ');
    switch(key){
      case DOWN:
        if(row<console.getRows()-(1+(col==console.getCols()-1))){
          row++;
        }
        break;
      case UP:
        if(row>0){
          row--;
        }
        break;
      case LEFT:
        if(col>0){
          col--;
        }
        break;
      case RIGHT:
        if(col<console.getCols()-(1+(row==console.getRows()-1))){
          col++;
        }
        break;
    }
    if(key == 'Q' ||key == 'q' || key == ESCAPE){
      if(escPressed){
        done = 1;
      }
      else{
        console.setPos(12,11);
        console<<"Press Escape or 'Q' again to exit";
        escPressed = 1;
      }
    }
    else{
      console.setPos(12,11);
      console<<"                                 ";
      escPressed = 0;
    }
    PrintKeyInfo(10, 11, key);
    console.setPos(row, col);
    console.putChar('X');
  }while(!done);
  cls();
  PrnTextBox(5,10,50,6, "Test 2: Did all the keys work correcty according to the specs of the "
             "assignment and the platfrom the program is running on? "
             "Hit 'Y' for Yes or any other key to continue: ");
  return Yes();
}


int test1(){
  cls();
  PrnTextBox(5,10,50,8, "Test 1: There should be three 'X' characters shown on left-top, "
             "right-top and left-buttom of the screen. Fourth 'X' should "
             "be one space to the buttom-right corner with cursor "
             "blinking on bottom-right corner. If this is true hit 'Y', otherwise hit any other key to continue.");
  console.setPos(0,0);
  console.putChar('X');
  console.setPos(console.getRows()-1, 0);
  console.putChar('X');
  console.setPos(0, console.getCols()-1);
  console.putChar('X');
  console.setPos(console.getRows()-1, console.getCols()-2);
  console.putChar('X');
  return Yes();
}



int welcome(){
  cls();
  console.setPos(1, 1);
  console<<TEAM_NAME;
  console.setPos(2,1);
  console<<RELEASE_VERSION;
  console.setPos(5, 1);
  console<<_IOL_PROGRAM_TITLE;
  PrnTextBox(10,10,50,6,
    "During the test, answer all the [Yes / No] questions by pressing 'Y' or 'y' for \"yes\" response or any other key for \"no\" response. "
    "Hit 'Y' to start the test or any other key to exit.");
  return Yes();
}



int Yes(){
  int key = console.getKey();
  return key == 'y' || key == 'Y';
}

int Escape(){
  int key = console.getKey();
  return key == ESCAPE;
}

void PrnTextBox(int row, int col, int width, int height, const char *text){
  int len;
  int i = 0;
  int j = 0;
  const char *from = text;
  PrintHollowBox(row, col, width, height, '+', '-', '|');
  while(i < height - 2 && *from){
    len = getLineLength(from, width - 2);
    for(console.setPos(row + 1+ i++ , col + 1),j=0;j<len;console.putChar(from[j]),j++);
    for(;j<len;console.putChar(' '),j++);
    from = from + len + 1 * !!from[len] ;
  }
}

int isWhiteSpace(char ch){
  int res = 0;
  int i=0;
  while(WHITESPACE[i] && !res){
    res = WHITESPACE[i] == ch;
    i++;
  }
  return res;
}

int StrLen(const char *str){
  int i = 0;
  while(str[i++]);
  return i-1;
}

int getLineLength(const char *src, int maxLen){
  const char* last = StrLen(src) > maxLen  ? src + maxLen : src + StrLen(src);
  while(*last && !isWhiteSpace(*last)){
    last--;
  }
  return (int)last - (int)src;
}

void PrintHollowBox(int row, int col, int width, int height, char corner, char horizontal, char vertical){
  int i;
  PrintLine(row, col, width, horizontal, corner);
  for(i=0;i<height-2;i++){
    PrintLine(row + i + 1, col, width, ' ', vertical);
  }
  PrintLine(row + height - 1, col,  width,horizontal, corner);
}

void PrintLine(int row, int col, int length, int ch, int endings){
  int i;
  console.setPos(row, col);
  console.putChar(endings);
  for(i=0;i<length-2;i++){
    console.putChar(ch);
  }
  console.putChar(endings);
}

void PrintKeyInfo(int row, int col, int key){
  bio_printf(row, col, 40, "Key code: %s", keycode(key));
  bio_printf(row+1, col, 40, "Key Name: %s", keyname(key));
}

const char *keycode(int ch){
  static char code[30];
  sprintf(code," Dec: %d   Hex: %X",ch ,ch);
  return code;
}

const char *keyname(int ch){
  static char kname[28][20]=
      {"UP",
       "DOWN",
       "LEFT",
       "RIGHT",
       "PGUP",
       "PGDN",
       "ENTER",
       "TAB",
       "BS",
       "DEL",
       "HOME",
       "END",
       "ESC",
       "INS",
       "F1",
       "F2",
       "F3",
       "F4",
       "F5",
       "F6",
       "F7",
       "F8",
       "F9",
       "F10",
       "F11",
       "F12",
       "X",
       "Unknown Keycode"};
  switch(ch){
       case UP:
	 return kname[0];
       case DOWN:
	 return kname[1];
       case LEFT:
	 return kname[2];
       case RIGHT:
	 return kname[3];
       case PGUP:
	 return kname[4];
       case PGDN:
	 return kname[5];
       case ENTER:
	 return kname[6];
       case TAB:
	 return kname[7];
       case BACKSPACE:
	 return kname[8];
       case DEL:
	 return kname[9];
       case HOME:
	 return kname[10];
       case END:
	 return kname[11];
       case ESCAPE:
	 return kname[12];
       case INSERT:
	 return kname[13];
       case F(1):
	 return kname[14];
       case F(2):
	 return kname[15];
       case F(3):
	 return kname[16];
       case F(4):
	 return kname[17];
       case F(5):
	 return kname[18];
       case F(6):
	 return kname[19];
       case F(7):
	 return kname[20];
       case F(8):
	 return kname[21];
       case F(9):
	 return kname[22];
       case F(10):
	 return kname[23];
       case F(11):
	 return kname[24];
       case F(12):
	 return kname[25];
       default:
	 if(ch >= ' ' && ch <= '~'){
	   kname[26][0] = ch;
	   return kname[26];
	 }
	 else{
	   return kname[27];
	 }
  }
}

void bio_printf(int row, int col, int len, char *format, ...){
  char buf[256] = "";
  char* str = buf;
  int i = 0;
  va_list
    ap;
  va_start(ap, format);
  while(*format){
    if(*format == '%'){
      format++;
      switch(*format){
        case 'd':
          sprintf(&buf[i],"%d", va_arg(ap, int));
          break;
        case 'f':
          sprintf(&buf[i],"%lf",va_arg(ap,double));
          break;
        case 's':
          sprintf(&buf[i],"%s",va_arg(ap,char*));
          break;
        case 'x':
          sprintf(&buf[i],"%x",va_arg(ap,int));
          break;
        case 'X':
          sprintf(&buf[i],"%X",va_arg(ap,int));
          break;
        case 'p':
          sprintf(&buf[i],"%p",va_arg(ap,void*));
          break;
        default:
          sprintf(&buf[i],"%c",*format);
      }
    }
    else{
      sprintf(&buf[i],"%c",*format);
    }
    format++;
    i = StrLen(buf);
  }
  va_end(ap);
  for(console.setPos(row, col);(len < 0 && *str) || (len>=0 && len--) ;console.putChar(*str?*str++:' '));
}

char *MyStrCpy(char* des, const char* src){
  char* str = des;
  while(*des++ = *src++);
  return str;
}

int runtest(int n){
  int ret = 1;
# if defined(DO_ALL)
  if(n >=0 ){
    if(tn != n){
      ret = 0;
    }
    else{
      tn++;
    }
  }
# endif
  return ret;
}

void cls(){
  console.clear();
  bio_printf(0, 20, 3,"%d", tn);
}

