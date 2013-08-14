#include "cchecklist.h"

namespace cui
{

// Constructor accepting five required and two optional parameters
CCheckList::CCheckList(const char* Format, int Row, int Col, int Width, bool radio, bool Bordered,
                       const char* Border)
    : CField(Row, Col, Width, 0, 0, Bordered, Border)
{
    _data = &_flags;
    bio::strcpy(_format, Format);
    _radio = radio;
    _cnt = _cur = _flags = 0;
}

// Destructor to deallocate memory for checkmarks to avoid memory leaks
CCheckList::~CCheckList(void)
{
    while(_cnt--)
    {
        delete _checkmarks[_cnt];
    }
}

// Adds a CCheckMark to the end of the CCheckList
CCheckList& CCheckList::add(const char* Text, bool selected)
{
    if(_cnt < 32)   // max number of CCheckMarks CCheckList can hold
    {
        _checkmarks[_cnt] = new CCheckMark(selected, _format, Text, _cnt + 1, 1, bio::strlen(Text) + 4,
                                           _radio); // allocate and initialize a new CCheckMark
        
        _checkmarks[_cnt]->frame(this); // set the container for the new CCheckMark to this object

        // increase the frame width of the CCheckList if currently less than that of the CCheckMark
        if(_checkmarks[_cnt]->width() > this->width())
        {
            this->width(_checkmarks[_cnt]->width() + 2);
        }

        this->height(_cnt + 3); // increase the height of current list to accomodate the new CCheckMark

        // change _flags to reflect the checked status of the added CCheckMark
        if(_checkmarks[_cnt]->checked())
        {
            unsigned int i;
            unsigned int j;

            for(i = 1, j = 0; j < _cnt; j++)
            {
                i <<= 1;    // i *= 2
            }

            _flags += i;
        }

        _cnt++; // increase the count to reflect the added CCheckMark
    }

    return *this;
}

// operator<< overloaded to add a CCheckMark to the list
CCheckList& CCheckList::operator<<(const char* Text)
{
    return add(Text);
}

// Draws a list containing the checkmarks
void CCheckList::draw(int fn)
{
    CFrame::draw(fn);
    unsigned int i;

    for(i = 0; i < _cnt; i++)
    {
        _checkmarks[i]->draw();
    }

    // place the cursor under the selected checkmark
    // first checkmark if none is selected
    i = selectedIndex();
    _cur = (i == -1) ? 0 : i;
    
}

// draw()s the list and allows editing of the checkmark(s) along with navigation
int CCheckList::edit()
{
    draw();
    bool done = false;
    int key;

    // continue editing the list until stopped
    while(!done)
    {
        // call the current checkmark's edit() and retrieve the key pressed
        key = _checkmarks[_cur]->edit();

        switch(key)
        {
        case DOWN:
        case RIGHT:
            // stop editing when DOWN or RIGHT key is pressed on the last checkmark
            // otherwise, go to the next checkmark in the list
            if(_cur == _cnt - 1)
            {
                done = true;
            }
            else
            {
                _cur++;
            }

            break;

        case UP:
        case LEFT:
            // stop editing when UP or LEFT key is pressed on the first checkmark
            // otherwise, go to the previous checkmark in the list
            if(_cur == 0)
            {
                done = true;
            }
            else
            {
                _cur--;
            }

            break;

        case SPACE:
            unsigned int i;

            // deselect all checkmarks other than the current one if in _radio (button) mode
            if(_radio)
                for(i = 0; i < _cnt; i++)
                    if(i != _cur)
                    {
                        _checkmarks[i]->checked(false);
                    }

            // draw() all the checkmarks in the list to reflect changes
            for(i = 0; i < _cnt; i++)
            {
                _checkmarks[i]->draw();
            }

            break;

        default:
            done = true;
        }
    }

    return key;
}

// Returns the checked state of the checkmarks in the list
void* CCheckList::data()
{
    unsigned int i;

    for(i = _cnt; i > 0; i--)
        if(_checkmarks[i - 1]->checked())
        {
            _flags += (1 << (i - 1));    // pow(2, i)
        }

    return (void*)_flags;
}

// Sets the checked state of the checkmarks in the list
void CCheckList::set(const void* data)
{
    _flags = *(unsigned int*)data;
    unsigned int i = _flags;
    unsigned int j = _cnt;
    unsigned int k;

    for(k = 0; j >= 1; j--)
    {
        k = i >> (j - 1);                       // start extracting from the leftmost bit and continue right
        _checkmarks[j - 1]->checked(k & 1);     // only the rightmost bit is significant
    }
}

// operator[] overloaded to access individual checkmarks in the list using an index
CCheckMark& CCheckList::operator[](unsigned int index)
{
    return *(_checkmarks[index]);
}

// Returns true since a CCheckList is always editable
bool CCheckList::editable()const
{
    return true;
}

// Returns the behaviour of the object as a radio button list
bool CCheckList::radio()const
{
    return _radio;
}

// Sets the radio behaviour of the list and the contained checkmarks to the parameter
void CCheckList::radio(bool val)
{
    _radio = val;
    unsigned int i;

    for(i = 0; i < _cnt; i++)
    {
        _checkmarks[i]->radio(_radio);
    }
}

// Returns _flags which holds the checked status of the checkmarks in the list
unsigned int CCheckList::flags()
{
    return _flags;
}

// Sets _flags equal to the parameter
void CCheckList::flags(unsigned int theFlags)
{
    _flags = theFlags;
}

// Returns the index of the first checked checkmark in the list
// -1 if none are selected
int CCheckList::selectedIndex()const
{
    unsigned int i;

    for(i = 0; i < _cnt; i++)
        if(_checkmarks[i]->checked())
        {
            return i;
        }

    return -1;
}

// Checks the checkmark at the incoming index
void CCheckList::selectedIndex(int index)
{
    if(_radio || index < 0)
    {
        unsigned int i;

        for(i = 0; i < _cnt; i++)
        {
            _checkmarks[i]->checked(false);
        }
    }

    if(index >= 0)
    {
        _checkmarks[index]->checked(true);
    }
}

// Returns the number of CCheckMarks in the list
unsigned int CCheckList::length()
{
    return _cnt;
}

}
