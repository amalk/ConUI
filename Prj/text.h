// Console UI Core Classes
// text.h
// a class to convert text string to arrays of string for each line of the text and back
//
// Fardad Soleimanloo, Chris Szalwinski
// Mar 19, 2012
// Version 0.9
#ifndef __CIO_TEXT_H__
#define __CIO_TEXT_H__

#define INIT_TEXT_LINES 128
#define INIT_LINE_CHARS 512

namespace cui{

  class Text;
  class Line{
    char* _data;
    unsigned int _size;
    static void* memcpy(void* des, const void* src, unsigned int len);
    static unsigned int strlen(const char* s);
    static unsigned int strncpy(char* d, const char* s, unsigned int len);
  public:
    Line(const char* data = (char*)0);
    Line(const Line &se);
    Line& operator=(const Line& se);
    Line& operator=(const char* str);
    Line& operator+=(const Line& se);
    Line& operator+=(const char* str);
    virtual ~Line();
    operator char*();
    unsigned int size()const;
    void expand(unsigned int newsize);
    unsigned int strlen()const;
    friend class Text;
  };

  class Text{
    Line** _data;
    unsigned int _size; // number of elements with or without data
    unsigned int _textlines; // number of elelments with data
    void importString(const char* text);
    void initialize();
  public:
    Text(const char* text=(char*)0);
    Text(const Text& sa);
    Text& operator=(const Text& sa);
    Text& operator=(const char* text);
    virtual ~Text();
    char* exportString()const;
    operator void*();
    Line& operator[](unsigned int index);
    bool remove(unsigned int index);
    void insertAt(unsigned int index);
    void insertAndBreakAt(unsigned int index, unsigned int charIndex);
    void expand(unsigned int newsize);
    unsigned int size()const; 
    unsigned int textLines();
    unsigned int strlen()const; // number of characters in text
    void destruct();
  };

}

#endif