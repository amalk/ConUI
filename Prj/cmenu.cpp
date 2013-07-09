#include "cmenu.h"

namespace cui{

	MNode::MNode(CMenuItem* item,unsigned int index, MNode* prev, MNode* next){}

	MNode::~MNode(){}
		
	void CMenu::drawItems(int fn){}

	CMenu::CMenu(const char* Title, const char* Format, int Row, int Col, 
              int Width, int Height,bool dropdown,  
			  const char* Border){}

	CMenu& CMenu::add(const char* Text, bool selected){}

	CMenu& CMenu::operator<<(const char* Text){}

	CMenu& CMenu::operator<<(bool select){}

	void CMenu::draw(int fn){}

	int CMenu::edit(){}

	void CMenu::set(const void* data){}

	int CMenu::selectedIndex(){}

	int CMenu::selectedIndex(int index){}

	const char* CMenu::selectedText(){}

	bool CMenu::editable()const{}

	CMenu::~CMenu(){}

}

