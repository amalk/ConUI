#include "cdialog.h"

namespace cui
{

CDialog::CDialog(CFrame* Container, int Row, int Col, int Width, int Height,
                 bool Bordered, const char* Border)
    : CFrame(Row, Col, Width, Height, Bordered, Border, Container)
{
    _fnum = 0;
    _fldSize = C_INITIAL_NO_FIELDS;
    _curidx = -1;
    _editable = false;
    _dyn = new bool[_fldSize];
    _fld = new CField*[_fldSize];
    unsigned int i;

    for(i = 0; i < _fldSize; i++)
    {
        _dyn[i] = false;
        _fld[i] = 0;
    }
}

CDialog::~CDialog()
{
    int i;

    for(i = 0; i < _fnum; i++)
    {
        if(_dyn[i])
        {
            delete _fld[i];
        }
    }

    delete [] _dyn;
    delete [] _fld;
}

void CDialog::draw(int fn)
{
    int i = 0;

    switch(fn)
    {
    case C_FULL_FRAME:
        CFrame::draw();     // fallthrough

    case 0:
        while(i < _fnum)
        {
            _fld[i++]->draw(fn);
        }

        break;

    default:
        if(fn > 0)
        {
            _fld[fn - 1]->draw(fn);
        }

        break;
    }
}

int CDialog::edit(int fn)
{
    int key = 0;
    draw(fn);

    if(!_editable)
    {
        key = console.getKey();
    }
    else
    {
        bool done = false;
        bool foundEditable;

        if(fn <= 0)
        {
            draw(fn);
            _curidx = 0;
        }
        else
        {
            _curidx = fn - 1;
        }

        for(foundEditable = false; !foundEditable; _curidx++)
        {
            (_curidx >= _fnum) && (_curidx = 0);
            (_fld[_curidx]->editable()) && (foundEditable = true);
        }

        _curidx--;

        while(!done)
        {
            key = _fld[_curidx]->edit();

            switch(key)
            {
            case LEFT:
            case RIGHT:
                break;

            case UP:
                for(--_curidx, foundEditable = false; !foundEditable; _curidx--)
                {
                    (_curidx < 0) && (_curidx = _fnum - 1);
                    (_fld[_curidx]->editable()) && (foundEditable = true);
                }

                _curidx++;
                break;
            
            case ENTER:
            case DOWN:
            case TAB:
                for(++_curidx, foundEditable = false; !foundEditable; _curidx++)
                {
                    (_curidx >= _fnum) && (_curidx = 0);
                    (_fld[_curidx]->editable()) && (foundEditable = true);
                }

                _curidx--;
                break;

            default:
                done = true;
                break;
            }   // switch
        }   // while
    }   // else

    return key;
}

int CDialog::add(CField* field, bool dynamic)
{
    if(_fnum == _fldSize)
    {
        CField** tempf = new CField*[_fldSize + C_DIALOG_EXPANSION_SIZE];
        bool* tempd = new bool[_fldSize + C_DIALOG_EXPANSION_SIZE];
        unsigned int i;

        for(i = 0; i < _fldSize; i++)
        {
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
    (field->editable()) && (_editable = true);
    field->container(this);
    return _fnum++;
}

int CDialog::add(CField& field, bool dynamic)
{
    return add(&field, dynamic);
}

CDialog& CDialog::operator<<(CField* field)
{
    add(field);
    return *this;
}

CDialog& CDialog::operator<<(CField& field)
{
    add(field);
    return *this;
}

bool CDialog::editable()const
{
    return _editable;
}

int CDialog::fieldNum()const
{
    return _fnum;
}

int CDialog::curIndex()const
{
    return _curidx;
}

CField& CDialog::operator[](unsigned int index)
{
    return *_fld[index];
}

CField& CDialog::curField()
{
    return *_fld[_curidx];
}

}