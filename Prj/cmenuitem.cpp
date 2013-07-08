#include "cmenuitem.h"

namespace cui{

	CMenuItem::CMenuItem(bool Selected,const char* Format, const char* Text, int Row, int Col, int Width)
              : CField(Row, Col, Width, 1), _Label(Text, 0, 1, Width - 2) {
		_selected = Selected;
		bio::strcpy(_format, Format);
		_data = &_format;
		_Label.frame(this);
	}

	CMenuItem::CMenuItem(const CMenuItem &CM) : CField(CM), _Label(CM._Label){
		_selected = CM.selected();
		_data = &_format;
		_Label.frame(this);
	}

	void CMenuItem::draw(int fn){
		
		_Label.draw(fn);

		if(_selected){
			console.strdsp(&_format[0], absRow(), absCol(), 1);
			console.strdsp(&_format[1], absRow(), absCol()+bio::strlen((char*)_Label.data()) + 1, 1);
		}
		else{
			console.strdsp(" ", absRow(), absCol() - 1);
			console.strdsp(" ", absRow(), absCol()+bio::strlen((char*)_Label.data()) + 1);
		}

        console.setPos(absRow(), absCol() + 1);
	}

	int CMenuItem::edit(){
		draw();
		int key = console.getKey();
		
		if(key == SPACE) {
			_selected = true;
			draw();
		}

		return key;

	}

	bool CMenuItem::editable()const{
		return true;
	}

	void CMenuItem::set(const void* Selected){
		_selected = *(bool*)Selected;
	}

	bool CMenuItem::selected()const{
		return _selected;
	}

	void CMenuItem::selected(bool val){
		_selected = val;
	}

	const char* CMenuItem::Text()const{
		return (char*)_Label.data();
	}
}