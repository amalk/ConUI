#include "ctext.h"

namespace cui
{
CText::CText(int Row, int Col, int Width, int Height, bool* Insertmode,
             bool displayOnly, const char* Border) :
    CField(Row, Col, Width, Height, 0, true, Border), _insertmode(Insertmode), _displayOnly(displayOnly)
{
    _curpos = _offset = _lcurpos = _loffset = 0;
}

CText::CText(const char* Str, int Row, int Col, int Width, int Height,
             bool* Insertmode, bool displayOnly, const char* Border) :
    CField(Row, Col, Width, Height, 0, true, Border), _insertmode(Insertmode), _displayOnly(displayOnly)
{
    _curpos = _offset = _lcurpos = _loffset = 0;
    set(Str);
}

void CText::draw(int fn)
{
    CField::draw(fn);
    int i;
    int actHeight = height() - 2;

    for(i = 0; i < actHeight; i++)
        if(_T[_loffset + i].strlen() > _offset)
        {
            console.strdsp(&_T[_loffset + i][_offset], absRow() + i + 1, absCol() + 1, width() - 2, _curpos);
        }
}

void CText::set(const void* Str)
{
    _T = (char*)Str;
}

void* CText::data()const
{
    return (void*)_T.exportString();
}

int CText::edit()
{
    Text local_T      = _T;
    int local_curpos  = _curpos;
    int local_offset  = _offset;
    int local_lcurpos = _lcurpos;
    int local_loffset = _loffset;
    int draw_offset   = -1;
    int draw_loffset  = -1;
    int key = 0;
    int i = 0;
    bool done = false;

    while(!done)
    {
        if(draw_offset != _offset || draw_loffset != _loffset)
        {
            draw();
            draw_offset = _offset;
            draw_loffset = _loffset;
        }

        key = console.stredit((char*)(_T[_lcurpos + _loffset]), absRow() + _lcurpos + 1, absCol() + 1, width() - 2,
                              _T[_lcurpos + _loffset].size(), &_offset, &_curpos, true, _displayOnly, *_insertmode);

        switch(key)
        {
        case UP:
            if(_lcurpos > 0)
            {
                _lcurpos--;
            }
            else if(_loffset > 0)
            {
                _loffset--;
            }

            break;

        case ENTER:
            if(!_displayOnly)
            {
                _T.insertAt(_loffset + _lcurpos);
                bio::strncpy((char*)_T[_lcurpos + _loffset], (char*)_T[_lcurpos + _loffset + 1],
                             _curpos + _offset);   //Current line is set to everything before the cursor before enter was pressed
                _T[_lcurpos + _loffset + 1] = &_T[_lcurpos + _loffset + 1][_curpos + _offset];
                _T[_lcurpos + _loffset][_curpos + _offset] = '\0';
            }

            _curpos = _offset = 0;

            // fallthrough

        case DOWN:
            if(_lcurpos < height() - 3)
            {
                _lcurpos++;
            }
            else if(_loffset + _lcurpos < _T.textLines() - 1)
            {
                _loffset++;
            }

            break;

        case ESCAPE:
            _T       = local_T;
            _curpos  = local_curpos;
            _offset  = local_offset;
            _lcurpos = local_lcurpos;
            _loffset = local_loffset;
            break;

        default:
            done = true;
            break;
        }
    }

    return key;
}

bool CText::editable()const
{
    return true;
}

bool CText::displayOnly()
{
    return _displayOnly;
}

void CText::displayOnly(bool val)
{
    _displayOnly = val;
}

}
