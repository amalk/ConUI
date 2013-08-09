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
    selected&&  (_selectedIndex = newNode->_index);
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

    if((_dropped || !_dropdown) && (_cnt > 0))
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
                for(i = 0, temp = _head; i < _cnt; i++)
                {
                    temp->_item->selected(false);
                    temp = temp->_next;
                }

                _cur->_item->selected(true);
                _selectedIndex = _cur->_index;
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
    MNode* temp = _first;

    for(i = 0; i < _cnt; i++, temp=temp->_next)
        temp->_item->selected(false);

    for(temp = _first; temp->_index < index; temp = temp->_next)
        ;

    temp->_item->selected(true);

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

}
