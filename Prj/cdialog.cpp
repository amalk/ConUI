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
	
	_dyn = new bool[_fldSize];
	_fld = new CField*[_fldSize];

	while(ctr < _fldSize){
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
		CFrame::draw();
		while(i < _fnum){
			_fld[i]->draw(fn);	//Calls the CField draw for all fields in dialog
			i++;
		}
	}
	else if(fn == 0){
		while(i < _fnum){
			_fld[i++]->draw(fn);	//Calls the field's draw for all fields in dialog
		}
	}
	else if(fn > 0){
		_fld[fn]->draw(fn);		//Draws a single field with the fields draw
	}
}

int CDialog::edit(int fn){

	int key = 0;
	int done = false;
	int i = _curidx;

	if(!_editable){			//CDialog has no editable fields
		key = console.getKey();
	}
	else if(fn < 1){		//Go to the 
		draw(fn);
		while(!done){
			if(_fld[i]->editable()){
				_fld[i]->edit();
				done = true;
			}
			i++;
		}
		_curidx = i - 1;
	}
	else {
		i = fn - 1;
		while(!done){
			if(_fld[i]->editable()){
				key = _fld[i]->edit();
				switch(key){
				case UP:
					i--;
					while(i >= 0){
						if(_fld[i]->editable()){
							_curidx = i;
							i = -1;
						}
						else if(i == 0){
							i = _fnum;
						}
						i--;
					}
					break;
				case ENTER:
				case DOWN:
				case TAB:
					while(i < _fnum){
						i++;
						if(_fld[i]->editable()){
							_curidx = i;
							i = -2;
						}
						else if(i == _fnum){
							i = -1;
						}
					}
					break;
				}

			}
			i++;
			if(i == _fnum)
				i = 0;
		}
	}

	return key;
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
	_dyn[_fnum] = dynamic;
	if(!_editable && field->editable())
		_editable = true;

	field->container();

	return _fnum++;
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