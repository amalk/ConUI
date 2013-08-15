#include "cmenu.h"

namespace cui
{

const bool CMenu::Select = true;

MNode::MNode(CMenuItem* item, unsigned int index, MNode* prev, MNode* next) :
    _item(item), _index(index), _prev(prev), _next(next)
{
}

MNode::~MNode()
{
    if(_item)
    {
        delete _item;
    }
}

CMenu::CMenu(const char* Title, const char* Format, int Row, int Col, int Width, int Height,
             bool dropdown, const char* Border) :
    _Title(Title, -1, 1, false),
    CField(Row + ((dropdown) ? 1 : 0), Col, Width, Height, 0, true, Border), _dropdown(dropdown),
    _dropped(false)
{
    _first = _head = _tail = _cur = 0;
    _cnt = 0;
    _selectedIndex = -1;
    _data = &_selectedIndex;
    bio::strcpy(_format, Format);
    _Title.frame(this);
}

CMenu& CMenu::add(const char* Text, bool selected)
{
    MNode* newNode = new MNode(new CMenuItem(selected, _format, Text, 1, 1, (width() - 2)), _cnt,
                               _tail);
    newNode->_item->frame(this);

    if(_head == 0)
    {
        _cur = _first = _head = newNode;
    }
    else
    {
        _tail->_next = newNode;
    }

    _tail = newNode;
    _cnt++;
    selected && selectedIndex(newNode->_index);
    return *this;
}

CMenu& CMenu::operator<<(const char* Text)
{
    return add(Text);
}

CMenu& CMenu::operator<<(bool select)
{
    if(select)
    {
        _tail->_item->selected(true);
        _selectedIndex = _tail->_index;
    }

    return *this;
}

void CMenu::draw(int fn)
{
    if(_dropdown)
    {
        _Title.draw();
    }

    if(_dropped || !_dropdown)
    {
        CField::draw(fn);
        int fieldHeight = (_cnt < height() - 2) ? _cnt : height() - 2;
        int i;
        MNode* temp = _first;

        for(i = 0; i < fieldHeight; i++)
        {
            temp->_item->row((_dropdown ? row() : 1) + i);  //use row if the dropdown is true, otherwise use 1
            temp->_item->draw();
            temp = temp->_next;
        }
    }
}

int CMenu::edit()
{
    int key = 0;
    int i;
    MNode* temp;
    bool doneEditing = false;
    bool doneBrowsing = false;

    while(!doneEditing)
    {
        draw();

        if(_dropdown && !_dropped)
        {
            key = _Title.edit();

            switch(key)
            {
            case C_BUTTON_HIT:
                _dropped = true;
                doneBrowsing = false;
                draw();
                break;

            case LEFT:
                key = UP;
                doneBrowsing = true;
                break;

            case RIGHT: //fallthrough
                key = DOWN;

            default:
                doneBrowsing = true;
                break;
            }
        }
        else if(!_dropdown)
        {
            doneBrowsing = false;
        }

        doneEditing = true;

        while(!doneBrowsing)
        {
            key = _cur->_item->edit();

            switch(key)
            {
            case ENTER:
                (_selectedIndex != -1) && (key = SPACE);
                doneBrowsing = true;
                break;

            case SPACE:
                selectedIndex(_cur->_index);
                doneBrowsing = true;
                draw();
                break;

            case DOWN:
                if(goNext())
                {
                    if(_cur->_index - _first->_index == height() - 2)
                    {
                        _first = _first->_next;
                        draw();
                    }
                }
                else
                {
                    if(_dropdown)
                    {
                        _cur = _first = _head;
                        draw();
                    }
                    else
                    {
                        doneBrowsing = true;
                    }
                }

                break;

            case UP:
                if(goPrev())
                {
                    if(_cur->_index < _first->_index)
                    {
                        _first = _first->_prev;
                        draw();
                    }
                }
                else
                {
                    if(_dropdown)
                    {
                        _cur = _tail;
                        int z = _cnt - height() + 2;

                        for(i = 0; i < z; i++)
                        {
                            _first = _first->_next;
                        }

                        draw();
                    }
                    else
                    {
                        doneBrowsing = true;
                    }
                }

                break;

            case ESCAPE:
                _dropped = false;
                doneEditing = _dropdown ? false : true;

            default:
                doneBrowsing = true;
                break;
            }   // switch
        }   // browsing loop

        if(_dropdown)
        {
            hide();
        }
    }   // editing loop

    if(_dropdown)
    {
        _dropped = false;
        draw();
    }

    return key;
}

void CMenu::set(const void* data)
{
    selectedIndex(*(int*)data);
}

int CMenu::selectedIndex()
{
    return _selectedIndex;
}

int CMenu::selectedIndex(int index)
{
    unsigned int i;
    MNode* temp = _head;

    for(i = 0; i < _cnt && temp; i++, temp = temp->_next)
    {
        temp->_item->selected(false);
    }

    for(temp = _head; temp && temp->_index < index; temp = temp->_next)
        ;

    if(temp)
    {
        temp->_item->selected(true);
    }

    return _selectedIndex = index;
}

const char* CMenu::selectedText()
{
    int selectedIdx = selectedIndex();

    if(selectedIdx != -1)
    {
        _cur = _head;

        while(_cur->_index < selectedIdx && goNext())
            ;

        return _cur->_item->Text();
    }

    return 0;
}

bool CMenu::editable() const
{
    return (_cnt > 0);
}

CMenu::~CMenu()
{
    while(_head != 0)
    {
        MNode* toDel = _head;
        _head = _head -> _next;
        delete toDel;
    }
}

bool CMenu::goNext()
{
    bool rv = false;

    if(_cur != _tail)
    {
        _cur = _cur->_next;
        rv = true;
    }

    return rv;
}

bool CMenu::goPrev()
{
    bool rv = false;

    if(_cur != _head)
    {
        _cur = _cur->_prev;
        rv = true;
    }

    return rv;
}

void CMenu::editNode(int i, const char* data)
{
    if(0 <= i && i < _cnt)
    {
        MNode* tNode;

        for(tNode = _head; tNode->_index < i; tNode = tNode->_next)
            ;

        tNode->_item->setLabel(data);
    }
}

void CMenu::deleteNode(int i)
{
    if(0 <= i && i < _cnt)
    {
        MNode* toDel;
        MNode* temp;

        for(toDel = _head; toDel && toDel->_index < i; toDel = toDel->_next)
            ;

        if(_cnt > 1)
        {
            for(temp = toDel->_next; temp && temp->_index <= _tail->_index; temp = temp->_next)
            {
                temp->_index--;
            }
        }

        if(toDel == _head)
        {
            _head = _head->_next;
            _first = _head;
            _cur = _head;
        }
        else
        {
            toDel->_prev->_next = toDel->_next;
        }

        if(toDel == _tail)
        {
            _tail = toDel->_prev;
        }
        else
        {
            toDel->_next->_prev = toDel->_prev;
        }

        delete toDel;
        _cnt--;
    }
}

void CMenu::navUp(bool allTheWay)
{
    if(allTheWay || _selectedIndex == -1)
    {
        _cur = _first = _head;
    }
    else
    {
        _cur = _head;

        //iterate through the MNodes to find the selected menuitem
        while(_cur->_index < _selectedIndex)
        {
            _cur = _cur->_next;
        }

        if(goPrev())
        {
            if(_cur->_index < _first->_index)
            {
                _first = _first->_prev;
            }
        }
        else
        {
            _cur = _tail;
            int z = _cnt - (height() - 2);

            for(int i = 0; i < z; i++)
            {
                _first = _first->_next;
            }
        }
    }

    selectedIndex(_cur->_index);
    draw(C_NO_FRAME);
}

void CMenu::navDown(bool allTheWay)
{
    if(allTheWay || _selectedIndex == -1)
    {
        _cur = _tail;
        _first = _head;
        int z = _cnt - height() + 2;

        for(int i = 0; i < z; i++)
        {
            _first = _first->_next;
        }
    }
    else
    {
        _cur = _head;

        //iterate through the MNodes to find the selected menuitem
        while(_cur->_index < _selectedIndex)
        {
            _cur = _cur->_next;
        }

        if(goNext())
        {
            if(_cur->_index - _first->_index == height() - 2)
            {
                _first = _first->_next;
            }
        }
        else
        {
            _cur = _first = _head;
        }
    }

    selectedIndex(_cur->_index);
    draw(C_NO_FRAME);
}

bool CMenu::navTo(int position)
{
    bool temp = false;

    if(position <= _cnt && position >= 1)
    {
        (_selectedIndex == -1) && (_selectedIndex = 0);

        while(position - 1 > _selectedIndex)
        {
            navDown();
        }

        while(position - 1 < _selectedIndex)
        {
            navUp();
        }

        selectedIndex(_cur->_index);
        draw(C_NO_FRAME);
        temp = true;
    }

    return temp;
}

}
