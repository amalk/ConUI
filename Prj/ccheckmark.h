#ifndef __CCHECKMARK_H__
#define __CCHECKMARK_H__

#include "cfield.h"
#include "clabel.h"

namespace cui{
  class CCheckMark : public CField{
    bool _flag;
    bool _radio;
    char _format[4];
    CLabel _Label;
  public:
    CCheckMark(bool Checked,const char* Format, const char* Text, int Row, int Col, int Width, bool IsRadio = false);
    CCheckMark(const CCheckMark& C);
    void draw(int fn = C_NO_FRAME) ;
    int edit();
    bool editable()const;
    void set(const void* flag);
    bool checked()const;
    void checked(bool val);
    bool radio(); // addition for R0.5
    void radio(bool isRadio); // addition for R0.5
    operator bool(); // addtion for R0.4
    operator char*();
    bool operator=(bool flag);
  };
}

#endif