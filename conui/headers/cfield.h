#ifndef __CFIELD_H__
#define __CFIELD_H__

#include "cframe.h"

namespace cui
{
class CDialog;
class CField : public CFrame
{
protected:
    void* _data;
public:
    CField(int Row = 0, int Col = 0,
           int Width = 0, int Height = 0,
           void* Data = (void*) 0,
           bool Bordered = false,
           const char* Border = C_BORDER_CHARS);
    virtual ~CField();
    virtual int edit() = 0;
    virtual bool editable() const = 0;


    virtual void set(const void* data) = 0;
    virtual void* data()const;

    void container(CDialog* theContainer);
    CDialog* container();
};

}

#endif
