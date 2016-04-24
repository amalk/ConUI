#include "ccheckmark.h"

namespace cui
{

// Constructor accepting six mandatory and one optional parameter
CCheckMark::CCheckMark(bool Checked, const char* Format, const char* Text, int Row, int Col,
                       int Width, bool IsRadio)
    : CField(Row, Col, Width, 1), _Label(Text, 0, 4, Width - 4)     // initialize the attached label
{
    _Label.frame(this);     // set the container for the _Label to the current object
    _flag = Checked;
    _radio = IsRadio;
    bio::strcpy(_format, Format);
    _data = &_flag;
}

// Copy constructor that passes _Label to the CLabel's copy constructor to safely copy the object
CCheckMark::CCheckMark(const CCheckMark& C) : CField(C), _Label(C._Label)
{
    this->_flag = C._flag;
    this->_radio = C._radio;
    bio::strcpy(this->_format, C._format);
    _data = &_flag;
}

// Displays the format and _Label
// "unchecks" if _flag is false, and puts the cursor under the checkmark
void CCheckMark::draw(int fn)
{
    console.strdsp(_format, absRow(), absCol());

    if(!_flag)
    {
        console.strdsp(" ", absRow(), absCol() + 1);
    }

    _Label.draw();
    console.setPos(absRow(), absCol() + 1);
}

// draw()s and accepts a key, and sets the _flag depending on the key and _radio
int CCheckMark::edit()
{
    draw();
    int key = console.getKey();

    if(key == SPACE)
    {
        _radio && (_flag = true);
        !_radio && (_flag = !_flag);
        draw();
    }

    return key;
}

// Returns true since a CCheckMark is always editable
bool CCheckMark::editable()const
{
    return true;
}

// Sets the _flag to the value of the parameter
void CCheckMark::set(const void* flag)
{
    _flag = *(bool*)flag;
}

// Returns the checked state of the object
bool CCheckMark::checked()const
{
    return _flag;
}

// Sets the checked state to the value of the parameter
void CCheckMark::checked(bool val)
{
    _flag = val;
}

// Returns if the CCheckMark behaves as a radio button
bool CCheckMark::radio()
{
    return _radio;
}

// Sets the radio button behaviour of the CCheckMark to the parameter
void CCheckMark::radio(bool isRadio)
{
    _radio = isRadio;
}

// bool cast overload returning the checked state of the CCheckMark
CCheckMark::operator bool()
{
    return _flag;
}

// char* cast overload returning the object's _Label's data
CCheckMark::operator char*()
{
    return (char*)_Label.data();
}

// operator= overload to change the checked state of the CCheckMark
bool CCheckMark::operator=(bool flag)
{
    return _flag = flag;
}

}