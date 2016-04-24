#ifndef __CLINEEDIT_H__
#define __CLINEEDIT_H__

#include "cfield.h"

namespace cui
{

class CLineEdit : public CField
{
    bool _dyn;
    int _maxdatalen;
    bool* _insertmode;
    int _curpos;
    int _offset;
public:
    CLineEdit(char* Str, int Row, int Col, int Width, int Maxdatalen, bool* Insertmode,
              bool Bordered = false, const char* Border = C_BORDER_CHARS);
    CLineEdit(int Row, int Col, int Width, int Maxdatalen, bool* Insertmode,
              bool Bordered = false, const char* Border = C_BORDER_CHARS);
    ~CLineEdit();
    void draw(int Refresh = C_FULL_FRAME);

    int edit();
    bool editable()const;

    void  set(const void* Str);
};

}

#endif
