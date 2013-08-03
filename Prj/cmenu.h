#ifndef __CMENU_H__
#define __CMENU_H__

#include "cuigh.h"
#include "cfield.h"
#include "cmenuitem.h"
#include "cbutton.h"

namespace cui
{
class Cmenu;

class MNode
{
    CMenuItem* _item;
    MNode* _next;
    MNode* _prev;
    unsigned int _index;
    MNode(CMenuItem* item, unsigned int index, MNode* prev, MNode* next = ((MNode*)0));
    ~MNode(void);
    friend class CMenu;
};

class CMenu : public CField
{
    MNode* _first;
    MNode* _head;
    MNode* _tail;
    MNode* _cur;
    char _format[3];
    unsigned int _cnt;
    int _selectedIndex;
    bool _dropdown;
    bool _dropped;
    bool goNext();
    bool goPrev();
    CButton _Title;

public:
    static const bool Select;
    CMenu(const char* Title, const char* Format, int Row, int Col,
          int Width, int Height, bool dropdown,
          const char* Border = C_BORDER_CHARS);
    CMenu& add(const char* Text, bool selected = false);
    CMenu& operator<<(const char* Text);
    CMenu& operator<<(bool select);
    void draw(int fn = C_FULL_FRAME);
    int edit();
    void set(const void* data);
    int selectedIndex();
    int selectedIndex(int index);
    const char* selectedText();
    bool editable() const;
    ~CMenu(void);
};

extern const bool Select;
}

#endif
