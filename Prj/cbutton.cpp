#include "cbutton.h"

namespace cui
{

// Constructor accepting three mandatory and two optional parameters
CButton::CButton(const char* Str, int Row, int Col,
                 bool Bordered, const char* Border) :
    CField(Row, Col, bio::strlen(Str) + (Bordered ?  4 : 2),
           Bordered ? 3 : 1, (void*)Str, Bordered, Border)
{
    char* temp = new char [bio::strlen(Str) + 1];   // allocate memory equal to Str's length characters
    CField::_data = temp;                           // make _data point to the allocated memory
    bio::strcpy(temp, Str);                         // copy the string to the allocated memory
}

// Destructor to safely destroy a CButton object and avoid memory leaks
CButton::~CButton()
{
    if(CField::_data)
    {
        delete[] CField::_data;
    }
}

// Displays the string pointed to by _data, and spaces to the left and right of it
void CButton::draw(int fn)
{
    CFrame::draw(fn);
    console.strdsp(" ", absRow() + (visible() ? 1 : 0),
                   absCol() + bio::strlen(_data) + (visible() ? 2 : 0));
    console.strdsp(" ", absRow() + (visible() ? 1 : 0), absCol() + (visible() ? 1 : -1), 0, 1);
    console.strdsp((const char*)_data, absRow() + (visible() ? 1 : 0), absCol() + (visible() ? 2 : 0),
                   0, 0);
}

// draw()s the button and surrounds it with [ ], returning the key pressed
int CButton::edit()
{
    console.strdsp("]", absRow() + (visible() ? 1 : 0),
                   absCol() + bio::strlen(_data) + (visible() ? 2 : 0));
    console.strdsp("[", absRow() + (visible() ? 1 : 0), absCol() + (visible() ? 1 : -1));
    int key = console.getKey();
    draw();
    return (key == ENTER || key == SPACE) ? C_BUTTON_HIT : key;
}

// Returns true since a CButton object is always editable
bool CButton::editable()const
{
    return true;
}

// Copies the string pointed to by the parameter into _data
void CButton::set(const void* str)
{
    char* temp = new char [bio::strlen(str) + 1];
    CField::_data = temp;
    bio::strcpy(temp, str);
}

}