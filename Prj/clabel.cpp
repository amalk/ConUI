#include "clabel.h"

namespace cui{

    CLabel::CLabel(const CLabel& L) : CField(L.row(), L.col(), L.width(), L.height()){
        _data = new char[bio::strlen(L._data) + 1];
        bio::strcpy(_data, L._data);
    }

    CLabel::CLabel(const char *Str, int Row, int Col, int Len) : CField(Row, Col){
        if(!Len) {
            _data = new char[bio::strlen(Str)+1];
            bio::strcpy((char*)_data, Str);
            width(bio::strlen(_data));
        }
        else {
            _data = new char[Len+1];
            bio::strncpy((char*)_data, Str, Len);
            ((char*)_data)[Len] = 0;
            width(Len);
        }
    }

    CLabel::CLabel(int Row, int Col, int Len) : CField(Row, Col, Len){
        _data = new char[Len+1]();
    }

    CLabel::~CLabel(){
       if (_data) 
           delete[] _data;
    }

    void CLabel::draw(int fn){
        console.strdsp((const char*)_data, absRow(), absCol(), width());
    }

    int CLabel::edit(){
         draw();
         return 0;
    }

    bool CLabel::editable()const{
         return false;
    }

    void CLabel::set(const void* str){
        if(width() > 0) {
            bio::strncpy((char*)_data, (char*)str, width());
        }
        else if(!width()) {
            delete[] _data;
            _data = new char[bio::strlen(str)+1];
            bio::strcpy((char*)_data, (char*)str);
        }
    }

}