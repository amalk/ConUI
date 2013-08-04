#include "clabel.h"

namespace cui
{

// Copy-constructor to avoid segmentation faults
CLabel::CLabel(const CLabel& L) : CField(L.row(), L.col(), L.width(), L.height())
{
    _data = new char[bio::strlen(L._data) + 1];
    bio::strcpy(_data, L._data);
}

// Constructor accepting four parameters at most, with an optional last parameter
CLabel::CLabel(const char* Str, int Row, int Col, int Len) : CField(Row, Col)
{
    if(!Len)    // allocate string length memory for _data if the Len parameter was not specified
    {
        _data = new char[bio::strlen(Str) + 1];
        bio::strcpy((char*)_data, Str);
        width(bio::strlen(_data));
    }
    else        // otherwise, copy only Len characters from the Str to _data
    {
        _data = new char[Len + 1];
        bio::strncpy((char*)_data, Str, Len);
        ((char*)_data)[Len] = 0;
        width(Len);
    }
}

// Constructor accepting exactly three parameters
CLabel::CLabel(int Row, int Col, int Len) : CField(Row, Col, Len)
{
    _data = new char[Len + 1]();
}

// Destructor to safely destroy a CLabel object and avoid memory leaks
CLabel::~CLabel()
{
    if(_data)
    {
        delete[] _data;
    }
}

// Displays the string pointed to by _data at absRow(), absCol() for width() characters
void CLabel::draw(int fn)
{
    console.strdsp((const char*)_data, absRow(), absCol(), width());
}

// Calls draw() and returns 0 since a CLabel object cannot be edited
int CLabel::edit()
{
    draw();
    return 0;
}

// Returns false since a CLabel object cannot be edited
bool CLabel::editable()const
{
    return false;
}

// Copies the string pointed to by the parameter into _data
void CLabel::set(const void* str)
{
    if(width() > 0)     // copy width() number of characters from str to _data
    {
        bio::strncpy((char*)_data, (char*)str, width());
    }
    else if(!width())   // copy the whole str to _data
    {
        delete[] _data;
        _data = new char[bio::strlen(str) + 1];
        bio::strcpy((char*)_data, (char*)str);
    }
}

}
