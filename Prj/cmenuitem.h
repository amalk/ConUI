#ifndef __CMENUITEM_H__
#define __CMENUITEM_H__
#include "cfield.h"
#include "clabel.h"

namespace cui{
  class CMenuItem:public CField{
    bool  _selected;
    char _format[3];
    CLabel _Label;
  public:
    CMenuItem(bool Selected,const char* Format, const char* Text, int Row, int Col, int Width);
    CMenuItem(const CMenuItem &CM);
    void draw(int fn = C_NO_FRAME) ;
    int edit();
    bool editable()const;
    void set(const void* Selected);
    bool selected()const;
    void selected(bool val);
    const char* Text()const;
  };
}

#endif