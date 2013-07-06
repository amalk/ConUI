#include "cdialog.h"

using namespace cui;

CDialog::CDialog(CFrame *Container,
            int Row, int Col, 
            int Width, int Height, 
            bool Bordered,
            const char* Border){

	int ctr = 0;
	_fnum = 0;
	_fldSize = C_INITIAL_NO_FIELDS;
	_curidx = -1;
	_editable = false;

	_dyn = new bool[_fnum];
	_fld = new CField*[_fnum];

	while(ctr < _fnum){
		_dyn[ctr] = false;
		_fld[ctr] = (CField*)0;
		ctr++;
	}
	
}

CDialog::~CDialog(){

	int i = 0;

	for(i; i < _fnum; i ++){
		if(_dyn[i]) 
			delete _fld[i];
	}

	delete [] _dyn;
	delete [] _fld;

}

void CDialog::draw(int fn){

	int i = 0;
	
	if(fn == C_FULL_FRAME){
		while(i < _fldSize){
			_fld[i]->CFrame::draw(fn);	//Calls the CFrame draw for all fields in dialog
		}
	}
	else if(fn == 0){
		while(i < _fldSize){
			_fld[i++]->draw(fn);	//Calls the field's draw for all fields in dialog
		}
	}
	else if(fn > 0){
		_fld[fn]->draw(fn);		//Draws a single field with the fields draw
	}

}

int CDialog::edit(int fn){

	

	return 0;
}
 
int CDialog::add(CField* field, bool dynamic){

	if(_fnum == _fldSize){
		int i = 0;
		CField** tempf = new CField*[_fldSize + C_DIALOG_EXPANSION_SIZE];
		bool* tempd = new bool[_fldSize + C_DIALOG_EXPANSION_SIZE];

		for(; i < _fldSize; i++){
			tempf[i] = _fld[i];
			tempd[i] = _dyn[i];
		}
		
		delete [] _dyn;
		delete [] _fld;

		_fld = tempf;
		_dyn = tempd;
		_fldSize += C_DIALOG_EXPANSION_SIZE;
	}


	_fld[_fnum] = field;
	_dyn[_fnum++] = dynamic;

	return _fnum;
}

int CDialog::add(CField& field, bool dynamic){

	return add(&field, dynamic);
}

CDialog& CDialog::operator<<(CField* field){
	add(field);
	return *this;
}

CDialog& CDialog::operator<<(CField& field){
	add(field);
	return *this;
}