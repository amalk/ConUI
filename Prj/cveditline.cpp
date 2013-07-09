#include "cveditline.h"

namespace cui{
    CValEdit::CValEdit(char* Str, int Row, int Col, int Width, int Maxdatalen, bool* Insertmode,
          bool (*Validate)(const char* , CDialog&), void (*Help)(MessageStatus, CDialog&), bool Bordered, 
		  const char* Border):CLineEdit(Str, Row, Col, Width, Maxdatalen, Insertmode, Bordered, Border){
			  _validate=Validate;
			  _help = Help;
	}
    CValEdit::CValEdit(int Row, int Col, int Width, int Maxdatalen, bool* Insertmode,
          bool (*Validate)(const char* , CDialog&), void (*Help)(MessageStatus, CDialog&), bool Bordered,
		  const char* Border):CLineEdit(Row, Col, Width, Maxdatalen, Insertmode, Bordered, Border){
			  _validate=Validate;
			  _help = Help;
	}
	int CValEdit::edit(){
		bool datavalid = true;
		int key;
		if (container()) {
			if (_help)
				_help(MessageStatus::SetMessage, *container());
			do {
				key = CLineEdit::edit();
				if (_validate && (key == UP || key == DOWN || key == TAB || key == ENTER))
					datavalid = _validate((const char*)_data, *container());
			} while(!datavalid);
			if (_help)
				_help(MessageStatus::ClearMessage, *container());
			return key;
		} else 
			return CLineEdit::edit();
	}
  };
