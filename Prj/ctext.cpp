#include "ctext.h"

void goUp(int&, int&);
void goDown(int&, int&, int, int);
void goRight(int&, int&, int, int);

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
    int currentLine;
    int lineLen;
    int numLines;
    int key = 0;
    int i = 0;
    bool done = false;

    while(!done)
    {
        draw();
        currentLine = _lcurpos + _loffset;
        numLines = _T.textLines();
        key = console.stredit((_T[currentLine]), absRow() + _lcurpos + 1, absCol() + 1, width() - 2,
                              _T[currentLine].size(), &_offset, &_curpos, true, _displayOnly, *_insertmode);

        switch(key)
        {
        case UP:
            goUp(_lcurpos, _loffset);
            break;

        case RIGHT:
            lineLen = _T[currentLine].strlen();

            if(_curpos + _offset == lineLen && _T[currentLine] != _T[numLines - 1])
            {
                goDown(_lcurpos, _loffset, height(), numLines);
                _curpos = _offset = 0;
            }

            break;

        case LEFT:
            if(!_curpos && !_offset && (_lcurpos || _loffset))
            {
                goUp(_lcurpos, _loffset);
                goRight(_curpos, _offset, _T[_lcurpos + _loffset].strlen(), width());
            }

            break;

        case ENTER:
            if(!_displayOnly)
            {
                _T.insertAndBreakAt(currentLine + 1, _curpos + _offset);
            }

            _curpos = _offset = 0;

            // fallthrough since ENTER always takes the cursor down if possible

        case DOWN:
            goDown(_lcurpos, _loffset, height(), numLines);
            break;

        case BACKSPACE:
            if(!_curpos && !_offset && (_lcurpos || _loffset) && !_displayOnly)
            {
                lineLen = _T[currentLine - 1].strlen();
                _T[currentLine - 1] += _T[currentLine];
                _T.remove(currentLine);
                goUp(_lcurpos, _loffset);
                goRight(_curpos, _offset, lineLen, width());
            }

            break;

        case ESCAPE:
            _T       = local_T;
            _curpos  = local_curpos;
            _offset  = local_offset;
            _lcurpos = local_lcurpos;
            _loffset = local_loffset;
            break;

        case F(1):
        case F(2):
        case F(6):
        case F(10):
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


void goUp(int& lcurpos, int& loffset)
{
    ((lcurpos > 0) && (lcurpos--)) || ((loffset > 0) && (loffset--));
}


void goDown(int& lcurpos, int& loffset, int height, int numLines)
{
    if(lcurpos < height - 3)
    {
        lcurpos++;
    }
    else if(loffset + lcurpos < numLines - 1)
    {
        loffset++;
    }
}

void goRight(int& curpos, int& offset, int lineLen, int frameWidth)
{
    if(lineLen - offset < frameWidth)
    {
        curpos = lineLen - offset;
    }
    else
    {
        curpos = frameWidth - 3;
        offset = lineLen - curpos;
    }
}
