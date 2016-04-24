#include "cveditline.h"

namespace cui
{

// Constructor accepting six mandatory and four optional parameters
CValEdit::CValEdit(char* Str, int Row, int Col, int Width, int Maxdatalen, bool* Insertmode,
                   bool (*Validate)(const char* , CDialog&), void (*Help)(MessageStatus, CDialog&), bool Bordered,
                   const char* Border): CLineEdit(Str, Row, Col, Width, Maxdatalen, Insertmode, Bordered, Border)
{
    _validate = Validate;
    _help = Help;
}

// Constructor accepting five mandatory and four optional parameters
CValEdit::CValEdit(int Row, int Col, int Width, int Maxdatalen, bool* Insertmode,
                   bool (*Validate)(const char* , CDialog&), void (*Help)(MessageStatus, CDialog&), bool Bordered,
                   const char* Border): CLineEdit(Row, Col, Width, Maxdatalen, Insertmode, Bordered, Border)
{
    _validate = Validate;
    _help = Help;
}

// Allows editing of _data, but with validation and help features
int CValEdit::edit()
{
    bool datavalid = true;
    int key;

    if(container())
    {
        if(_help)
        {
            _help(MessageStatus::SetMessage, *container());  // display help message when the field is focused
        }

        do
        {
            key = CLineEdit::edit();

            if(_validate && (key == UP || key == DOWN || key == TAB || key == ENTER))
            {
                datavalid = _validate((const char*)_data, *container());
            }
        }
        while(!datavalid);    // prevent exiting the field until valid data is entered

        if(_help)
        {
            _help(MessageStatus::ClearMessage, *container());  // hide the help message when data is validated
        }

        return key;
    }
    else
    {
        return CLineEdit::edit();
    }
}

};
