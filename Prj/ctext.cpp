#include "ctext.h"

namespace cui {
    CText::CText(int Row, int Col, int Width, int Height, bool* Insertmode, 
                 bool displayOnly, const char* Border):
            CField(Row, Col, Width, Height, 0, 0, Border){
        _insertmode = Insertmode;
        _displayOnly = displayOnly;
    }

    CText::CText(const char* Str, int Row, int Col, int Width, int Height,
                 bool* Insertmode, bool displayOnly, 
                 const char* Border) :
            CField(Row, Col, Width, Height, 0, 0, Border){
        _insertmode = Insertmode;
        _displayOnly = displayOnly;
        set(Str);
    }
    
    void CText::draw(int fn) {
        CField::draw(fn);
        int i = 0;
        while(i < _T.textLines()){
            if(_T[i].strlen() > _offset)
                console.strdsp((char*)_T[i], 10, 11, 20, 0);
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
        bool done = false;
        int key = 0;

        while(!done){
            draw();
            console.stredit((char*)_T[_lcurpos + _loffset], absRow() + _lcurpos, absCol() + _curpos, width() - 2, _T[_lcurpos + _loffset].size(), 0, 0, true, _displayOnly);

            key = console.getKey();

            if(key == ESCAPE){
                done = true;
                _curpos = local_curpos;
                _offset = local_offset;
                _lcurpos = local_lcurpos;
                _loffset = local_loffset;
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