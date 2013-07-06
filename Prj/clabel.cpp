#include "clabel.h"


namespace cui{

	 CLabel::CLabel(const CLabel& L): CField(L.row(), L.col(), L.width(), L.height(), L.data()){		
	 }

     CLabel::CLabel(const char *Str, int Row, int Col, int Len): CField(Row, Col){
		 if(!Len) {
			 char* temp= new char(bio::strlen(&Str));
			 bio::strcpy (temp, Str);
			 _data=temp;
			 delete []temp;
		 }	else
		 {
			 char* temp= new char(bio::strlen(&Str));
			 bio::strncpy (temp, Str, Len);
			 _data=temp;
			 delete []temp;
		 }
		 CFrame::width(bio::strlen(_data));
	 }

	 CLabel::CLabel(int Row, int Col, int Len){
		 char* temp= new char(Len);
		 _data=temp;
		 delete []temp;
	 }

	 CLabel::~CLabel(){
		 delete []_data;
	 }

	 void CLabel::draw(int fn){
		 console.strdsp((const char*)data(), absRow(), absCol(), width());
	 }

	 int CLabel::edit(){
		 draw();
		 return 0;
	 }

	 bool CLabel::editable()const{
		 return false;
	 }

	 void CLabel::set(const void* str){
		if(width()) {
			delete []_data;
			char* temp= new char(bio::strlen(&str));
			bio::strcpy (temp, str);
			_data=temp;
			delete []temp;
		 }	else
		 {
			 char* temp= new char(width());
			 bio::strncpy (temp, str, width());
			 _data=temp;
			 delete []temp;
		 }
		 CFrame::width(bio::strlen(_data));
	 }

}