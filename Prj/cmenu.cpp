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

// Adds a new menuitem to the menu through MNodes
CMenu& CMenu::add(const char* Text, bool selected)
{
    // Allocate memory for the MNode and CMenu, with the MNodes index as _cnt and its previous node as _tail
    MNode* newNode = new MNode(new CMenuItem(selected, _format, Text, 1, 1, (width() - 2)), _cnt,
                               _tail);
    newNode->_item->frame(this);    // Set the new menuitems frame as this

    // Will only occur once at the first addition of a menuitem to the menu
    if(_head == 0)
    {
        _cur = _first = _head = newNode;    // The pointers to the current, first and last nodes are all to t
    }
    else    // The previous tail's next is made to point to the new one
    {
        _tail->_next = newNode;
    }

    _tail = newNode;    // The new tail is the new node
    _cnt++; // The menuitem count increases
    selected && selectedIndex(newNode->_index); // Makes the newly added menuitem selected if selected is true
    return *this;
}

// Calls add
CMenu& CMenu::operator<<(const char* Text)
{
    return add(Text);
}

// Selects the tail
CMenu& CMenu::operator<<(bool select)
{
    if(select)
    {
        _tail->_item->selected(true);
        _selectedIndex = _tail->_index;
    }

    return *this;
}

// Draws the menu
void CMenu::draw(int fn)
{
    // Redraw the title if the menu is a dropdown menu
    if(_dropdown)
    {
        _Title.draw();
    }

    // If the menu is dropped or it is not a dropdown menu, draw it
    if(_dropped || !_dropdown)
    {
        CField::draw(fn);
        // The number of fields to draw changes depending on whether it's bordered or not
        int fieldHeight = (_cnt < height() - 2) ? _cnt : height() - 2;
        int i;
        MNode* temp = _first;

        // Iterate through the fields to be drawn, starting from the _first MNode
        for(i = 0; i < fieldHeight; i++)
        {
            // Set the row of the menuitem in the MNode starting from the row of the menu or 1
            temp->_item->row((_dropdown ? row() : 1) + i);
            temp->_item->draw();    // Draw the menuitem
            temp = temp->_next; // Go to the next MNode
        }
    }
}

// Edits the menu
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

        // If the menu is a dropdown menu and it hasn't been dropped yet set the edit to the menu's title button
        if(_dropdown && !_dropped)
        {
            key = _Title.edit();

            switch(key)
            {
            case C_BUTTON_HIT:  // User wants to go into the menu, drop the menu and start editing the menu
                _dropped = true;
                doneBrowsing = false;
                draw();
                break;

            case LEFT:  // User wants to go past the menu, do not drop or edit the menu
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
        else if(!_dropdown) // If the menu isn't a dropdown menu, start editing
        {
            doneBrowsing = false;
        }

        doneEditing = true;

        while(!doneBrowsing)
        {
            key = _cur->_item->edit();  // Edit the current MNodes menuitem

            switch(key)
            {
            case ENTER: // If there is a selected index, the key is changed to SPACE and browsing is done
                (_selectedIndex != -1) && (key = SPACE);
                doneBrowsing = true;
                break;

            case SPACE: // Select the current MNode
                selectedIndex(_cur->_index);
                doneBrowsing = true;
                draw();
                break;

            case DOWN:  // Go to the next MNode, if possible
                // If it is possible, check to see if the offset of the menu should be changed
                if(goNext())
                {
                    if(_cur->_index - _first->_index == height() - 2)
                    {
                        // _first is moved to the next MNode and the menu is redrawn
                        _first = _first->_next;
                        draw();
                    }
                }
                // If it isn't possible, check to see if the user is in a dropdown menu
                else
                {
                    if(_dropdown)
                    {
                        // Change the current and first MNodes to the head
                        _cur = _first = _head;
                        draw();
                    }
                    else
                    {
                        // Stop editing the menu
                        doneBrowsing = true;
                    }
                }

                break;

            case UP:    // Go to the previous MNode, if possible
                // If it is possible, check to see if the offset of the menu should be changed
                if(goPrev())
                {
                    // Check if the current index is lower than the _first index
                    if(_cur->_index < _first->_index)
                    {
                        // If so, change the _first index to the previous index and redraw the menu
                        _first = _first->_prev;
                        draw();
                    }
                }
                // If it isn't possible, check if the menu is a dropdown menu
                else
                {
                    // If it is a dropdown menu: Set the current index as the tail
                    if(_dropdown)
                    {
                        _cur = _tail;
                        int z = _cnt - height() + 2;

                        // Set _first to the MNode that would be at the top of the menu when the current MNode is at the bottom
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

            case ESCAPE:    // Close the menu if it's a dropdown menu and stop editing
                _dropped = false;
                doneEditing = _dropdown ? false : true;

            default:
                doneBrowsing = true;
                break;
            }   // switch
        }   // browsing loop

        if(_dropdown)   // Hide the menu if it's a dropdown menu
        {
            hide();
        }
    }   // editing loop

    if(_dropdown)   // Undrop the menu if it's a dropdown menu and redraw the menu
    {
        _dropped = false;
        draw();
    }

    return key;
}

// Set the selected index to the data entered
void CMenu::set(const void* data)
{
    selectedIndex(*(int*)data);
}

// Return the selected index
int CMenu::selectedIndex()
{
    return _selectedIndex;
}

// Set the selected index as the index entered
int CMenu::selectedIndex(int index)
{
    unsigned int i;
    MNode* temp = _head;    // Create a temporary pointer to the head of the menu

    // Set all the menuitems in the menu to false
    for(i = 0; i < _cnt && temp; i++, temp = temp->_next)
    {
        temp->_item->selected(false);
    }

    // Iterate through the MNodes until the index matches with the index passed in
    for(temp = _head; temp && temp->_index < index; temp = temp->_next);

    // If the MNode pointer isn't null, make it selected
    if(temp)
    {
        temp->_item->selected(true);
    }

    return _selectedIndex = index; // Set and return the _selected index
}

// Return the selected menuitem's text
const char* CMenu::selectedText()
{
    int selectedIdx = selectedIndex();

    if(selectedIdx != -1)
    {
        // Set the current item as the head
        _cur = _head;

        // Iterate through the menu to find the selected MNode
        while(_cur->_index < selectedIdx && goNext());

        // Return the selected MNodes menuitem text
        return _cur->_item->Text();
    }

    // Don't return anything if nothing is selected
    return 0;
}

// Return true if the menu has 1 or more items
bool CMenu::editable() const
{
    return (_cnt > 0);
}

// Delete all the MNodes
CMenu::~CMenu()
{
    while(_head != 0)
    {
        MNode* toDel = _head;
        _head = _head -> _next;
        delete toDel;
    }
}

// Returns true if the current node isn't the tail and automatically goes to the next node
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

// Returns true if the current node isn't the head and automatically goes to the previous node
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

// Edits the MNode at index i
void CMenu::editNode(int i, const char* data)
{
    if(0 <= i && i < _cnt)
    {
        MNode* tNode;

        // Iterate through the nodes until the index matches i
        for(tNode = _head; tNode->_index < i; tNode = tNode->_next);

        // Change the menuitems label to data
        tNode->_item->setLabel(data);
    }
}

// Deletes the MNode at index i
void CMenu::deleteNode(int i)
{
    MNode* toDel;
    MNode* temp;

    // Iterate through the MNodes until its index matches i
    for(toDel = _head; toDel && toDel->_index < i; toDel = toDel->_next);
    // && temp->_index <= _tail->_index

    // Change the index of all the MNodes after the one to be deleted to be one less
    // Starting from the MNode after the one to be deleted
    // As long as temp is not null
    for(temp = toDel->_next; temp; temp = temp->_next)
    {
        temp->_index--;
    }

    // If the head is to be deleted
    if(toDel == _head)
    {
        // Make the head pointer the one after the current head and make current and first point to it
        _head = _head->_next;
        _first = _head;
        _cur = _head;
    }
    // If the tail is to be deleted
    else if(toDel == _tail)
    {
        // Make the tail the one before the current tail and make current point to it
        _tail = toDel->_prev;
        _cur = _tail;
        _tail->_next = 0;

        if(_cnt > height() - 2)
        {
            _first = _first->_prev;
        }

    }
    else
    {
        // Make the MNodes before and after toDel point to eachother
        toDel->_prev->_next = _cur = toDel->_next;  // Make the current MNode the one after the one to be deleted
        toDel->_next->_prev = toDel->_prev;
    }

    delete toDel;
    _cnt--;
}

// Move the current MNode to the previous MNode and select it
void CMenu::navUp(bool allTheWay)
{
    // allTheWay is default false and if true will just point the current MNode and the first to the head
    if(allTheWay || _selectedIndex == -1)
    {
        _cur = _first = _head;
    }
    else
    {
        _cur = _head;

        // Iterate through the MNodes to find the selected menuitem
        while(_cur->_index < _selectedIndex)
        {
            _cur = _cur->_next;
        }

        // Check to see if the selected MNode is the head
        if(goPrev())
        {
            // If not, go up and change the first accordingly
            if(_cur->_index < _first->_index)
            {
                _first = _first->_prev;
            }
        }
        else
        {
            // If so, set the current MNode to the tail and change first accordingly
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

// Move the current MNode to the previous MNode and select it
void CMenu::navDown(bool allTheWay)
{
    // allTheWay is default false and if true will just point the current MNode to the tail and change first accordingly
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

        // Iterate through the MNodes to find the selected menuitem
        while(_cur->_index < _selectedIndex)
        {
            _cur = _cur->_next;
        }

        // Check to see if the selected MNode is the tail
        if(goNext())
        {
            // If not, go down and change the first accordingly
            if(_cur->_index - _first->_index == height() - 2)
            {
                _first = _first->_next;
            }
        }
        else
        {
            // If so, point current and first to the head
            _cur = _first = _head;
        }
    }

    selectedIndex(_cur->_index);
    draw(C_NO_FRAME);
}

// Sets the current MNode to the one with index equal to position
bool CMenu::navTo(int position)
{
    // Returns true if successfully set
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
