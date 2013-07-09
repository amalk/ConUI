#include "cmenu.h"

namespace cui{

	MNode::MNode(CMenuItem* item,unsigned int index, MNode* prev, MNode* next){
		_item = item; 
		_index = index;
		_prev = prev;
		_next = next;
	}

	MNode::~MNode(){
		if (_item ) delete _item;
	}
		
	CMenu::CMenu(const char* Title, const char* Format, int Row, int Col, int Width, int Height,bool dropdown,  
			  const char* Border) : _Title(Title, -1, 1, false), 
			  CField((!dropdown)?Row:(Row+1), Col, Width, Height, (void*)0, true, Border) {
		_head = _tail = _cur = (__nullptr);
		for (int i=0; i<2; i++)
			_format[i] = Format[i];
		_cnt = 0;
		_selectedIndex = 0;
		_data = MNode::_item;

	}

	CMenu& CMenu::add(const char* Text, bool selected){}

	CMenu& CMenu::operator<<(const char* Text){}

	CMenu& CMenu::operator<<(bool select){}

	void CMenu::draw(int fn){}

	int CMenu::edit(){}

	void CMenu::set(const void* data){}

	int CMenu::selectedIndex(){}

	int CMenu::selectedIndex(int index){}

	const char* CMenu::selectedText(){
		
	}

	bool CMenu::editable()const{
		return (_cnt>0);
	}

	CMenu::~CMenu(){
		while(!(_head == (MNode*)0)) delete ((char*)0);
	}

}

