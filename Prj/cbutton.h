#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include "cfield.h"
#include "console.h"

namespace cui{

    class CButton: public CField{

    public:
        CButton(const char *Str, int Row, int Col, 
                bool Bordered = true,
                const char* Border=C_BORDER_CHARS);
        virtual ~CButton();
        void draw(int fn=C_FULL_FRAME);
        int edit();
        bool editable()const;
        void set(const void* str);
    };
}

#endif