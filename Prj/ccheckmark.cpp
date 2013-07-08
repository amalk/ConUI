#include "ccheckmark.h"

namespace cui {

    CCheckMark::CCheckMark(bool Checked, const char* Format, const char* Text, int Row, int Col, int Width, bool IsRadio)
        : CField(Row, Col, Width, 1), _Label(Text, 0, 4, Width-4) {
            _Label.frame(this);
            _flag = Checked;
            _radio = IsRadio;
            bio::strcpy(_format, Format);
            _data = &_flag;
    }

    CCheckMark::CCheckMark(const CCheckMark& C) : CField(C), _Label(C._Label) {
        this->_flag = C._flag;
        this->_radio = C._radio;
        bio::strcpy(this->_format, C._format);
        _data = &_flag;
    }

    void CCheckMark::draw(int fn) {
        console.strdsp(_format, absRow(), absCol());
        if(!_flag)
            console.strdsp(" ", absRow(), absCol()+1);
        _Label.draw();
        console.setPos(absRow(), absCol()+1);
    }

    int CCheckMark::edit() {
        draw();
        int key = console.getKey();
        if(key == SPACE) {
            _radio && (_flag = true);
            !_radio && (_flag = !_flag);
            draw();
        }
        return key;
    }

    bool CCheckMark::editable()const {
        return true;
    }

    void CCheckMark::set(const void* flag) {
        _flag = *(bool*)flag;
    }

    bool CCheckMark::checked()const {
        return _flag;
    }

    void CCheckMark::checked(bool val) {
        _flag = val;
    }

    bool CCheckMark::radio() {
        return _radio;
    }

    void CCheckMark::radio(bool isRadio) {
        _radio = isRadio;
    }

    CCheckMark::operator bool() {
        return _flag;
    }

    CCheckMark::operator char*() {
        return (char*)_Label.data();
    }

    bool CCheckMark::operator=(bool flag) {
        return _flag = flag;
    }

}