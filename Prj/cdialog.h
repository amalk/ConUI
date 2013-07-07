#ifndef __CDIALOG_H__
#define __CDIALOG_H__

#include "cuigh.h"
#include "console.h"
#include "cframe.h"
#include "cfield.h"
namespace cui{
  class CField;
  class CDialog: public CFrame{
    private:
    int _fnum;
    int _curidx;
    CField** _fld;
    bool* _dyn;
    bool _editable;
    unsigned int _fldSize;
    public:
    CDialog(CFrame *Container = (CFrame*)0,
             int Row = -1, int Col = -1, 
             int Width = -1, int Height = -1, 
             bool Bordered = false,
             const char* Border = C_BORDER_CHARS);
    virtual ~CDialog();
    void draw(int fn = C_FULL_FRAME);
    int edit(int fn = C_FULL_FRAME);
 
    int add(CField* field, bool dynamic = true);
    int add(CField& field, bool dynamic = false);
    CDialog& operator<<(CField* field);
    CDialog& operator<<(CField& field);
 
	bool editable()const{ return _editable;}
	int fieldNum()const{ return _fnum;}
	int curIndex()const{ return _curidx;}
 
	CField& operator[](unsigned int index){ return *_fld[index];}
	CField& curField(){ return *_fld[_curidx];}
  };
}

#endif