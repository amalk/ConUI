#include "cchecklist.h"

namespace cui {
    CCheckList::CCheckList(const char* Format, int Row, int Col, int Width, bool radio, bool Bordered, const char* Border)
               : CField(Row, Col, Width, 0, 0, Bordered, Border) {
        _data = &_flags;
        bio::strcpy(_format, Format);
        _radio = radio;
        _cnt = _cur = _flags = 0;
    }

    CCheckList::~CCheckList(void) {
        while(_cnt--)
            delete _checkmarks[_cnt];
    }

    CCheckList& CCheckList::add(const char* Text, bool selected) {
        if(_cnt < 32) {
            _checkmarks[_cnt] = new CCheckMark(selected, _format, Text, _cnt + 1, 1, bio::strlen(Text) + 4);
            _checkmarks[_cnt]->frame(this);
            
            if(_checkmarks[_cnt]->width() > this->width())
                this->width(_checkmarks[_cnt]->width());
            
            this->height(_cnt+3);
            
            if(_checkmarks[_cnt]->checked()) {
                unsigned int i;
                unsigned int j;

                for(i = 1, j = 0; j < _cnt; j++)
                    i *= 2;

                _flags += i;
            }

            _cnt++;
        }
        return *this;
    }

    CCheckList& CCheckList::operator<<(const char* Text) {
        return add(Text);
    }

    void CCheckList::draw(int fn) {
        CFrame::draw(fn);
        unsigned int i;
        for(i = 0; i < _cnt; i++)
            _checkmarks[i]->draw();

        //console.setPos(absRow(), absCol()+1);   // to first selected index
    }

    int CCheckList::edit() {
        draw();
        bool done = false;
        int key;
        while(!done) {
            key = _checkmarks[_cur]->edit();
            switch(key) {
            case DOWN:
            case RIGHT:
                if(_cur == _cnt - 1)
                    done = true;
                else
                    _cur++;
                break;
            case UP:
            case LEFT:
                if(_cur == 0)
                    done = true;
                else
                    _cur--;
                break;
            case SPACE:
                selectedIndex(_cur);
                break;
            }
        }
        return key;
    }

    void* CCheckList::data()const {
        return (void*)_flags;
    }

    void CCheckList::set(const void* data) {
        _flags = *(unsigned int*)data;



    }

    CCheckMark& CCheckList::operator[](unsigned int index) {
        return *(_checkmarks[index]);
    }

    bool CCheckList::editable()const {
        return true;
    }

    bool CCheckList::radio()const {
        return _radio;
    }

    void CCheckList::radio(bool val) {
        _radio = val;
        unsigned int i = 0;
        while(i < _cnt)
            _checkmarks[i]->radio(_radio);
    }

    unsigned int CCheckList::flags() {
        return _flags;
    }

    void CCheckList::flags(unsigned int theFlags) {
        _flags = theFlags;
    }

    unsigned int CCheckList::selectedIndex()const {
        unsigned int i;
        for(i = 0; i < _cnt; i++)
            if(_checkmarks[i]->checked())
                return i;

        return -1;
    }

    void CCheckList::selectedIndex(unsigned int index) {
        if(_radio) {
            unsigned int i;
            for(i = 0; i < _cnt; i++)
                _checkmarks[i]->checked(false);
        }
        _checkmarks[index]->checked(true);
    }

    unsigned int CCheckList::length() {
        return _cnt;
    }

}