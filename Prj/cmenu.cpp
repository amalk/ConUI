#include "cmenu.h"

namespace cui{

    MNode::MNode(CMenuItem* item, unsigned int index, MNode* prev, MNode* next) : 
        _item(item), _index(index), _prev(prev), _next(next) {

    }

    MNode::~MNode(){
        if(_item)
            delete _item;
    }
        
    CMenu::CMenu(const char* Title, const char* Format, int Row, int Col, int Width, int Height, bool dropdown, const char* Border) : 
            _Title(Title, -1, 1, false),
            CField(Row + ((dropdown) ? 1 : 0), Col, Width, Height, 0, true, Border), _dropdown(dropdown), _dropped(false) {
        
        _head = _tail = _cur = 0;
        _cnt = 0;
        _selectedIndex = -1;
        _data = &_selectedIndex;
        bio::strcpy(_format, Format);
        _Title.frame(this);
    }

    CMenu& CMenu::add(const char* Text, bool selected) {
        MNode* newNode = new MNode(new CMenuItem(selected, _format, Text, 1, 1, (width() - 2)), (_cnt - 1), _tail, 0);
        
        if(_head == 0)
            _head = newNode;
        else
            _tail->_next = newNode;

        _tail = newNode;

        _cnt++;

        return *this;
    }

    CMenu& CMenu::operator<<(const char* Text) {
        return add(Text);
    }

    CMenu& CMenu::operator<<(bool select) {
        // select && (_selectedIndex = _cnt - 1);
        return *this;
    }

    void CMenu::draw(int fn) {
        if(_dropdown)
            _Title.draw();

        if(_dropped) {

        }
    }

    int CMenu::edit() {

    }

    void CMenu::set(const void* data) {
        _selectedIndex = *(int*)data;
    }

    int CMenu::selectedIndex() const {
        
    }

    int CMenu::selectedIndex(int index) {
        return _selectedIndex = index;
    }

    const char* CMenu::selectedText() {

    }

    bool CMenu::editable() const {
        return (_cnt > 0);
    }

    CMenu::~CMenu() {
        while(_head != 0) {
            MNode* toDel = _head;
            _head = _head -> _next;
            delete toDel;
        }
    }

}
