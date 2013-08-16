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

// Draws the ctext
void CText::draw(int fn)
{
    CField::draw(fn);
    int i;
    int actHeight = height() - 2;   // Always bordered

    for(i = 0; i < actHeight; i++)
    {
        // Only prints anything if the line is longer than its offset
        if(_T[_loffset + i].strlen() > _offset)
        {
            // Prints the part of the line after the offset at a length that fits in the box
            console.strdsp(&_T[_loffset + i][_offset], absRow() + i + 1, absCol() + 1, width() - 2, _curpos);
        }
    }
}

// Assigns the argument data to the Text held in CText
void CText::set(const void* Str)
{
    _T = (char*)Str;
}

// Returns data from the Text
void* CText::data()const
{
    return (void*)_T.exportString();
}

// Begins editing the CText
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
        numLines = _T.textLines();  // Keeping track of numLines manually because Text::remove() 
                                    // doesn't seem to accurately reduce _textlines
        key = console.stredit((_T[currentLine]), absRow() + _lcurpos + 1, absCol() + 1, width() - 2,
                              _T[currentLine].size(), &_offset, &_curpos, true, _displayOnly, *_insertmode);

        // The user will only ever go into this switch statement if he exits the stredit
        switch(key)
        {
        case UP: // Go up to the previous line
            goUp(_lcurpos, _loffset);
            break;

        case RIGHT: // RIGHT only comes out of stredit() if it's pressed at the end of a line
            // Only go down and change the cursor position/offset if NOT on the last line
            if(_T[currentLine] != _T[numLines - 1])
            {
                goDown(_lcurpos, _loffset, height(), numLines);
                _curpos = _offset = 0;
            }

            break;

        case LEFT:  // LEFT only comes out of stredit() if it's pressed at the start of a line
            // Only go up and to the end of the line if the cursor isn't at the first character of the first line
            if(_lcurpos || _loffset)    // Both are false if cursor is at first character of first line
            {
                goUp(_lcurpos, _loffset);
                goRight(_curpos, _offset, _T[_lcurpos + _loffset].strlen(), width());
            }

            break;

        case ENTER: // Insert a new line after the current line
            // Don't insert anything if not editable
            if(!_displayOnly)
            {
                _T.insertAndBreakAt(currentLine + 1, _curpos + _offset);
            }

            _curpos = _offset = 0;

            // fallthrough since ENTER always takes the cursor down if possible

        case DOWN:
            goDown(_lcurpos, _loffset, height(), numLines);
            break;

        case BACKSPACE: // BACKSPACE only comes out of stredit() if it's pressed at the start of a line
            // Only do backspace things if the text is editable and the cursor isn't at the first character of the first line
            // Backspace things being:
            if((_lcurpos || _loffset) && !_displayOnly)
            {
                lineLen = _T[currentLine - 1].strlen();
                _T[currentLine - 1] += _T[currentLine]; // Overwrite the previous lines newline character with everything in the current line
                _T.remove(currentLine);
                goUp(_lcurpos, _loffset);   // Go up and to the end of the previous line
                goRight(_curpos, _offset, lineLen, width());
            }

            break;

        case ESCAPE:    // Revert all changes
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

// _displayOnly getter
bool CText::displayOnly()
{
    return _displayOnly;
}

// _displayOnly setter
void CText::displayOnly(bool val)
{
    _displayOnly = val;
}

}

// Go up to a previous line changing the offset or cursor position as necessary
void goUp(int& lcurpos, int& loffset)
{
    ((lcurpos > 0) && (lcurpos--)) || ((loffset > 0) && (loffset--));
}

// Go down to the next line changing the offset or cursor position as necessary
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

// Send the cursor to the end of the line
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