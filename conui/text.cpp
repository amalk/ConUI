// Console UI Core Classes
// text.cpp
// a class to convert text string to arrays of string for each line of the text and back
//
// Fardad Soleimanloo, Chris Szalwinski
// Dec 12, 2012
// Version 0.91

#include "text.h"
namespace cui
{

// returns the lentgth of a string but
unsigned int Line::strlen(const char* str)
{
    unsigned int len = 0;

    while(*str && *str != '\n')
    {
        str++;
        len++;
    }

    return len;
}

void* Line::memcpy(void* des, const void* src, unsigned int len)
{
    for(unsigned int i = 0; i < len; i++)
    {
        ((char*)des)[i] = ((char*)src)[i];
    }

    return des;
}
unsigned int Line::strncpy(char* des, const char* src, unsigned int len)
{
    unsigned int i = 0;

    for(i = 0; src[i] && src[i] != '\n' && i < len; i++)
    {
        des[i] = src[i];
    }

    if(i != len)
    {
        des[i] = 0;
    }

    return i;
}
Line::Line(const char* data)
{
    _size = INIT_LINE_CHARS;

    if(data)
    {
        unsigned int len = strlen(data) + 1;

        while(_size < len)
        {
            _size = _size * 2;
        }

        _data = new char[_size];
        strncpy(_data, data, _size);
    }
    else
    {
        _data = new char[_size];
        _data[0] = 0;
    }
}
Line::Line(const Line& se)
{
    _size = se.size();
    _data = new char[_size];
    Line::memcpy(_data, se._data, _size);
}
Line& Line::operator=(const Line& se)
{
    if(this != &se)
    {
        delete[] _data;
        _size = se.size();
        _data = new char[_size];
        Line::memcpy(_data, se._data, _size);
    }

    return *this;
}
Line& Line::operator=(const char* str)
{
    if(str != _data)
    {
        unsigned int len = strlen(str) + 1;

        if(_size < len)
        {
            delete[] _data;

            while(_size < len)
            {
                _size = _size * 2;
            }

            _data = new char[_size];
        }

        strncpy(_data, str, _size);
    }

    return *this;
}
Line& Line::operator+=(const Line& se)
{
    if(this != &se)
    {
        int size = _size;
        expand(_size + se._size + 1);
        Line::strncpy(_data + strlen(_data), se._data, se._size);
    }

    return *this;
}
Line& Line::operator+=(const char* str)
{
    if(str != _data)
    {
        int size = _size;
        int strsize = strlen(str) + 1;
        expand(_size + strsize);
        Line::strncpy(_data + strlen(_data), str, strsize);
    }

    return *this;
}
Line::~Line()
{
    delete[] _data;
}

Line::operator char* ()
{
    return _data;
}

unsigned int Line::size()const
{
    return _size;
}
unsigned int Line::strlen()const
{
    return strlen(_data);
}
void Line::expand(unsigned int newsize)
{
    if(_size < newsize)
    {
        unsigned int oldsize = _size;

        while(_size < newsize)
        {
            _size = _size * 2;
        }

        char* newdata = new char[_size];
        memcpy(newdata, _data, oldsize);
        delete[] _data;
        _data = newdata;
    }
}
/********* Text *********************************/
void Text::initialize()
{
    _data = new Line*[INIT_TEXT_LINES];
    _size = INIT_TEXT_LINES;
    _textlines = 0;

    for(int i = 0; i < INIT_TEXT_LINES; i++)
    {
        _data[i] = (Line*)0;
    }
}
void Text::importString(const char* text)
{
    int i = 0;
    int lines = 0;
    bool hasText = false;

    if(_data == (Line**)0)
    {
        initialize();
    }

    for(i = 0; text && text[i]; i++)
    {
        lines += (text[i] == '\n');
        hasText = true;
    }

    if(hasText)
    {
        lines += (_textlines + 1);
        expand(lines);

        for(i = _textlines; i < lines; i++)
        {
            if(_data[i] == (Line*)0)
            {
                _data[i] = new Line(text);
            }
            else
            {
                *_data[i] = text;
            }

            text += (Line::strlen(text) + 1);
            _textlines++;
        }
    }
}
char* Text::exportString()const
{
    char* text = new char[strlen() + 1];
    char* t = text;

    for(unsigned int i = 0; i < _textlines; i++)
    {
        if(_data[i])
        {
            t += Line::strncpy(t, _data[i]->_data, -1);
        }

        *t = '\n';
        t++;
    }

    if(*(t - 1) == '\n')
    {
        *(t - 1) = 0;
    }

    return text;
}
Text::Text(const char* text)
{
    initialize();

    if(text)
    {
        importString(text);
    }
}
Text::Text(const Text& sa)
{
    _data = new Line*[_size = sa._size];
    _textlines = sa._textlines;

    for(unsigned int i = 0; i < _size; i++)
    {
        if(_data[i] && sa._data[i])
        {
            _data[i] = new Line(*sa._data[i]);
        }
        else
        {
            _data[i] = (Line*)0;
        }
    }
}
Text& Text::operator=(const Text& sa)
{
    if(this != &sa)
    {
        destruct();
        _data = new Line*[_size = sa._size];
        _textlines = sa._textlines;

        for(unsigned int i = 0; i < _size; i++)
        {
            if(_data[i] && sa._data[i])
            {
                _data[i] = new Line(*sa._data[i]);
            }
            else
            {
                _data[i] = (Line*)0;
            }
        }
    }

    return *this;
}
Text& Text::operator=(const char* text)
{
    destruct();
    importString(text);
    return *this;
}
Text::~Text()
{
    destruct();
}
Text::operator void* ()
{
    return (void*)exportString();
}
Line& Text::operator[](unsigned int index)
{
    if(index > _size - 1)
    {
        expand(index + 1);
    }

    if(_data[index] == (Line*)0)
    {
        _data[index] = new Line;
    }

    if(index + 1 > _textlines)
    {
        _textlines = index + 1;
    }

    return *_data[index];
}
bool Text::remove(unsigned int index)
{
    bool res = false;

    if(index < _size)
    {
        unsigned int i = index;
        res = true;
        delete _data[index];

        for(; i < _size - 1; i++)
        {
            _data[i] = _data[i + 1];
        }

        _data[i] = (Line*)0;
        _textlines--;
    }

    return res;
}
void Text::insertAt(unsigned int index)
{
    expand(index + 1);
    unsigned int i = _size - 1;

    for(; i > index; i--)
    {
        _data[i] = _data[i - 1];
    }

    _data[i] = (Line*)0;

    if(index > _textlines - 1)
    {
        _textlines = index + 1;
    }
    else
    {
        _textlines++;
    }
}
void Text::insertAndBreakAt(unsigned int index, unsigned int charIndex)
{
    Text& T = *this;
    insertAt(index);

    if(index && T[index - 1].strlen() > charIndex)
    {
        T[index] = &T[index - 1][charIndex];
        T[index - 1][charIndex] = 0;
    }
}
void Text::expand(unsigned int newsize)
{
    if(_size < newsize)
    {
        unsigned int i;
        unsigned int oldsize = _size;

        while(_size < newsize)
        {
            _size = _size * 2;
        }

        Line** newdata = new Line*[_size];

        for(i = 0; i < oldsize; i++)
        {
            newdata[i] = _data[i];
        }

        for(; i < _size; i++)
        {
            newdata[i] = (Line*)0;
        }

        delete[] _data;
        _data = newdata;
    }
}
unsigned int Text::size()const
{
    return _size;
}
unsigned int Text::textLines()
{
    return _textlines;
}
unsigned int Text::strlen()const
{
    unsigned int len = 0;

    for(unsigned int i = 0; i < _textlines; i++)
    {
        if(_data[i])
        {
            len += (_data[i]->strlen() + 1);
        }
        else
        {
            len++;
        }
    }

    return --len;
}
void Text::destruct()
{
    for(unsigned int i = 0; i < _size; i++)
    {
        if(_data[i])
        {
            delete _data[i];
        }
    }

    delete[] _data;
    _size = 0;
    _textlines = 0;
    _data = (Line**)0;
}
}



































