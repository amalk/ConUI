#include "cbutton.h"

namespace cui
{

CButton::CButton(const char* Str, int Row, int Col,
                 bool Bordered, const char* Border) :
    CField(Row, Col, bio::strlen(Str) + (Bordered ?  4 : 2),
           Bordered ? 3 : 1, (void*)Str, Bordered, Border)
{
    char* temp = new char [bio::strlen(Str) + 1];
    CField::_data = temp;
    bio::strcpy(temp, Str);
}

CButton::~CButton()
{
    if(CField::_data)
    {
        delete[] CField::_data;
    }
}

void CButton::draw(int fn)
{
    CFrame::draw(fn);
    console.strdsp(" ", absRow() + (visible() ? 1 : 0),
                   absCol() + bio::strlen(_data) + (visible() ? 2 : 0));
    console.strdsp(" ", absRow() + (visible() ? 1 : 0), absCol() + (visible() ? 1 : -1), 0, 1);
    console.strdsp((const char*)_data, absRow() + (visible() ? 1 : 0), absCol() + (visible() ? 2 : 0),
                   0, 0);
}

int CButton::edit()
{
    console.strdsp("]", absRow() + (visible() ? 1 : 0),
                   absCol() + bio::strlen(_data) + (visible() ? 2 : 0));
    console.strdsp("[", absRow() + (visible() ? 1 : 0), absCol() + (visible() ? 1 : -1));
    int key = console.getKey();
    draw();
    return (key == ENTER || key == SPACE) ? C_BUTTON_HIT : key;
}

bool CButton::editable()const
{
    return true;
}

void CButton::set(const void* str)
{
    char* temp = new char [bio::strlen(str) + 1];
    CField::_data = temp;
    bio::strcpy(temp, str);
}

}