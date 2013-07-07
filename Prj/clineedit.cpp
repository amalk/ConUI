#include "clineedit.h"

namespace cui{

    CLineEdit::CLineEdit(char* Str, int Row, int Col, int Width,
      int Maxdatalen, bool* Insertmode, 
      bool Bordered,
	  const char* Border): CField(Row, Col,  Width, (Bordered)?3:1, Str, Bordered, Border){
		 _data=Str;
		 _dyn=false;
		 _maxdatalen=Maxdatalen;
		 _bordered=Bordered;
		 _insertmode=Insertmode;
	}

    CLineEdit::CLineEdit(int Row, int Col, int Width,
      int Maxdatalen, bool* Insertmode, 
      bool Bordered,
	  const char* Border){
		_data = new char[Maxdatalen + 1];
		_dyn=true;
		_maxdatalen=Maxdatalen;
		_bordered=Bordered;
		_insertmode=Insertmode;
	}

	CLineEdit::~CLineEdit(){
		if (_dyn && _data)
			delete []_data;
	}

	void CLineEdit::draw(int Refresh){
		CFrame::draw(Refresh);
		console.strdsp((const char*)_data, (_bordered)?absRow()+1:absRow(), 
			(_bordered)?absCol()+1:absCol(), (_bordered)?width()-2:width());
	}
 
	int CLineEdit::edit(){
		return console.stredit((char*)_data, (_bordered)?absRow()+1:absRow(), 
			(_bordered)?absCol()+1:absCol(), (_bordered)?width()-2:width(), _maxdatalen, &_offset, &_curpos, *_insertmode);
	}

	bool CLineEdit::editable()const{
		return true;
	}
	
	void CLineEdit::set(const void* Str){		
		bio::strncpy(_data, Str, _maxdatalen);
		CFrame::width(bio::strlen(_data));
	}
  
}