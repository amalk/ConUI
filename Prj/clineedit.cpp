#include "clineedit.h"

namespace cui{

    CLineEdit::CLineEdit(char* Str, int Row, int Col, int Width,
      int Maxdatalen, bool* Insertmode, 
      bool Bordered,
	  const char* Border): CField(Row, Col,  Width, (Bordered)?3:1, Str, Bordered, Border){
		 _dyn=false;
		 _data = Str;
		 _maxdatalen=Maxdatalen;
		 _insertmode=Insertmode;
	}

    CLineEdit::CLineEdit(int Row, int Col, int Width,
      int Maxdatalen, bool* Insertmode, 
      bool Bordered,
	  const char* Border): CField(Row, Col,  Width, (Bordered)?3:1, 0, Bordered, Border){
		_data = new char[Maxdatalen + 1]();
		_dyn=true;
		_maxdatalen=Maxdatalen;
		_insertmode=Insertmode;
	}

	CLineEdit::~CLineEdit(){
		if (_dyn && _data)
			delete []_data;
	}

	void CLineEdit::draw(int Refresh){
		CFrame::draw(Refresh);
		console.strdsp((const char*)_data, absRow()+((visible())?1:0), 
			absCol()+((visible())?1:0), width()-((visible())?2:0));
	}
 
	int CLineEdit::edit(){
		return console.stredit((char*)_data, absRow()+((visible())?1:0), 
			absCol()+((visible())?1:0), width()-((visible())?2:0), _maxdatalen, &_offset, &_curpos, *_insertmode);
	}

	bool CLineEdit::editable()const{
		return true;
	}
	
	void CLineEdit::set(const void* Str){		
		bio::strncpy(_data, Str, _maxdatalen);
		CFrame::width(bio::strlen(_data));
	}
  
}