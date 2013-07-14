#include "ctext.h"

namespace cui {
    CText::CText(int Row, int Col, int Width, int Height, bool* Insertmode, 
                 bool displayOnly, const char* Border):
            CField(Row, Col, Width, Height, 0, 0, Border){
        _curpos = 0;
        _lcurpos = 0;
        _offset = 0;
        _loffset = 0;
        _insertmode = Insertmode;
        _displayOnly = displayOnly;
    }

    CText::CText(const char* Str, int Row, int Col, int Width, int Height,
                 bool* Insertmode, bool displayOnly, 
                 const char* Border) :
            CField(Row, Col, Width, Height, 0, true, Border){
        _curpos = 0;
        _lcurpos = 0;
        _offset = 0;
        _loffset = 0;
        _insertmode = Insertmode;
        _displayOnly = displayOnly;
        set(Str);
    }
    
    void CText::draw(int fn) {

        CField::draw(fn);
        int i = 0;
        int z = _T[i].strlen();
        while(i < height() - 2){
            if(_T[i].strlen() > _offset){
                if(_T[i].strlen() >= width() - 2){
                    console.strdsp(&(((char*)_T[_loffset + i])[_offset]), absRow() + i + 1, absCol() + 1, width() - 2, _curpos);
                }
                else{
                    console.strdsp(&(((char*)_T[_loffset + i])[_offset]), absRow() + i + 1, absCol() + 1, _T[i].strlen(), _curpos);
                }
            }
            i++;
        }
    }

    void CText::set(const void *Str) {
        _T = (char*)Str;
    }

    void* CText::data()const {
        return (void*)&_T;
    }

    int CText::edit() {

        Text local_T = _T;
        int local_curpos = _curpos;
        int local_offset = _offset;
        int local_lcurpos = _lcurpos;
        int local_loffset = _loffset;
        int key = 0;
        int i = 0;
        bool done = false;

        while(!done){
            draw();
            
            key = console.stredit((char*)(_T[_lcurpos + _loffset]), 
                    absRow() + _lcurpos + 1, 
                    absCol() + 1, 
                    width() - 2, 
                    _T[_lcurpos + _loffset].size(), 
                    &_offset, 
                    &_curpos,
                    true,
                    _displayOnly);

            switch(key){
            case UP:
                if(_lcurpos > 0){
                    _lcurpos--;
                }
                else if(_loffset > 0){
                    _loffset--;
                }
                break;
            case DOWN:
                if(_lcurpos < height() - 3){
                    _lcurpos++;
                }
                else if(_loffset + _lcurpos < _T.textLines() - 1){
                    _loffset++;
                }
                break;
            case ENTER:
                _T.insertAt(_loffset + _lcurpos);
                bio::strncpy((char*)_T[_lcurpos + _loffset], (char*)_T[_lcurpos + _loffset + 1], _curpos + _offset);   //Current line is set to everything before the cursor before enter was pressed
                _T[_lcurpos + _loffset + 1] = &((char*)_T[_lcurpos + _loffset + 1])[_curpos + _offset];
                ((char*)_T[_lcurpos + _loffset])[_curpos + _offset] = '\n';
                ((char*)_T[_lcurpos + _loffset])[_curpos + _offset + 1] = '\0';
                
                if(_lcurpos < height() - 3){
                    _lcurpos++;
                }
                else {
                    _loffset++;
                }
                _curpos = 0;
                _offset = 0;
                break;
            case PGUP:
                if(_lcurpos + _loffset - height() >= 0){
                    _lcurpos = 0;
                    _loffset -= height();
                }
                done = true;
                break;
            case PGDN:
                if(_lcurpos + _loffset + height() <= _T.textLines() - 1){
                    _lcurpos += height() - 3;
                    _loffset += height();
                }
                done = true;
                break;
            case ESCAPE:
                _T = local_T;
                _curpos = local_curpos;
                _offset = local_offset;
                _lcurpos = local_lcurpos;
                _loffset = local_loffset;
            case F (1):
            case F (2):
            case F (3):
            case F (4):
            case F (5):
            case F (6):
            case F (7):
            case F (8):
            case F (9):
            case F (10):
            case F (11):
            case F (12):
                done = true;
                break;
            }

        }

        return key;
    }

    bool CText::editable()const {
        return true;
    }

    bool CText::displayOnly() {
        return _displayOnly;
    }

    void CText::displayOnly(bool val) {
        _displayOnly = val;
    }

}