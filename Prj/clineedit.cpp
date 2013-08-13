#include "clineedit.h"

namespace cui
{

// Constructor accepting six mandatory and two optional parameters
CLineEdit::CLineEdit(char* Str, int Row, int Col, int Width,
                     int Maxdatalen, bool* Insertmode, bool Bordered, const char* Border) 
                     : CField(Row, Col,  Width, (Bordered) ? 3 : 1, Str, Bordered, Border)
{
    _dyn = false;
    _data = Str;
    _maxdatalen = Maxdatalen;
    _insertmode = Insertmode;
    _curpos = _offset = 0;
}

// Constructor accepting five mandatory and two optional parameters
CLineEdit::CLineEdit(int Row, int Col, int Width, int Maxdatalen,
                     bool* Insertmode, bool Bordered, const char* Border)
                     : CField(Row, Col,  Width, (Bordered) ? 3 : 1, 0, Bordered, Border)
{
    _data = new char[Maxdatalen + 1]();    // allocate and initialize memory to accomodate editing of Maxdatalen characters
    _dyn = true;
    _maxdatalen = Maxdatalen;
    _insertmode = Insertmode;
    _curpos = _offset = 0;
}

// Destructor to safely destroy a CLineEdit object and avoid memory leaks
CLineEdit::~CLineEdit()
{
    if(_dyn && _data)
    {
        delete []_data;
    }
}

// Displays the string pointed to by _data
void CLineEdit::draw(int Refresh)
{
    CFrame::draw(Refresh);
    console.strdsp((const char*)_data + _offset, absRow() + ((visible()) ? 1 : 0),
                   absCol() + ((visible()) ? 1 : 0), width() - ((visible()) ? 2 : 0));
}

// Calls stredit() to allow editing of the string pointed to by _data
int CLineEdit::edit()
{
    int i;
    while((i = console.stredit((char*)_data, absRow() + ((visible()) ? 1 : 0),
                   absCol() + ((visible()) ? 1 : 0), width() - ((visible()) ? 2 : 0), _maxdatalen, &_offset, &_curpos,
                   false, false, *_insertmode)) == BACKSPACE);    // loop to prevent exiting the function if BACKSPACE is pressed at curpos 0
    return i;
}

// Returns true since a CLineEdit object is always editable
bool CLineEdit::editable()const
{
    return true;
}

// Copies the characters pointed by the parameter to the object's _data
void CLineEdit::set(const void* Str)
{
    bio::strncpy(_data, Str, _maxdatalen);
}

}
