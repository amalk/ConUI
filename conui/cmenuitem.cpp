#include "cmenuitem.h"

namespace cui
{

// Constructor accepting six required parameters
CMenuItem::CMenuItem(bool Selected, const char* Format, const char* Text, int Row, int Col,
                     int Width)
    : CField(Row, Col, Width, 1), _Label(Text, 0, 1, Width - 2) // initialize the attached _Label
{
    _selected = Selected;
    bio::strcpy(_format, Format);
    _data = &_format;
    _Label.frame(this);     // set the container for the _Label to the current object
}

// Copy constructor that passes _Label to the CLabel's copy constructor to safely copy the object
CMenuItem::CMenuItem(const CMenuItem& CM) : CField(CM), _Label(CM._Label)
{
    _selected = CM.selected();
    _data = &_format;
    _Label.frame(this);
}

// Draws the attached _Label and surrounds it with _format if _selected
void CMenuItem::draw(int fn)
{
    _Label.draw(fn);

    if(_selected)
    {
        console.strdsp(&_format[0], absRow(), absCol(), 1);
        console.strdsp(&_format[1], absRow(), absCol() + bio::strlen((char*)_Label.data()) + 1, 1);
    }
    else
    {
        console.strdsp(" ", absRow(), absCol());
        console.strdsp(" ", absRow(), absCol() + bio::strlen((char*)_Label.data()) + 1);
    }

    console.setPos(absRow(), absCol() + 1); // put the cursor under the first character of Label
}

// Draw()s and accepts a key, and sets the _selected state depending on the key
int CMenuItem::edit()
{
    draw();
    int key = console.getKey();

    if(key == SPACE)
    {
        _selected = true;
        draw();
    }

    return key;
}

// Returns true since a CMenuItem is always editable
bool CMenuItem::editable()const
{
    return true;
}

// Sets _selected state to the value of the parameter
void CMenuItem::set(const void* Selected)
{
    _selected = *(bool*)Selected;
}

// Returns the _selected state of the object
bool CMenuItem::selected()const
{
    return _selected;
}

// Sets _selected state to the value of the parameter
void CMenuItem::selected(bool val)
{
    _selected = val;
}

// Returns the object's _Label's data
const char* CMenuItem::Text()const
{
    return (char*)_Label.data();
}

// Sets the _Label's data to the incoming parameter
void CMenuItem::setLabel(const char* data)
{
    _Label.set((void*)data);
}

}
