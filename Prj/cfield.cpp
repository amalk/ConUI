#include "cfield.h"
namespace cui{
  CField::CField(int Row, int Col,int Width, int Height,
    void* Data,bool Bordered,const char* Border)
    :CFrame(Row, Col, Width, Height,Bordered, Border){
    _data = Data;
    container((CDialog*)0);
  }
  CField::~CField(){
  }

  void* CField::data()const{
    return _data;
  }
 
  void CField::container(CDialog* theContainer){
    CFrame::frame((CFrame*)theContainer);
  }

  CDialog* CField::container(){
    return (CDialog*)frame();
  }
}