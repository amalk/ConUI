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

// Depending on fn, draws all fields in dialog or draws a specific dialog
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

// Begins editing from the first field or from field fn
int CDialog::edit(int fn)
{
    int key = 0;
    draw(fn);

    // If no fields are editable, just get a key
    if(!_editable)
    {
        key = console.getKey();
    }
    else
    {
        bool done = false;

        // Set the current index to either: 0 if fn is less than 1, otherwise set it to the field number - 1
        if(fn < 1)
        {
            draw(fn);
            _curidx = 0;
        }
        else
        {
            _curidx = fn - 1;
        }

        // Find the first editable field starting from the current index
        while(!_fld[_curidx]->editable())
        {
            (_curidx >= _fnum) && (_curidx = 0);
            _curidx++;
        }

        while(!done)
        {
            // Call the fields edit()
            key = _fld[_curidx]->edit();

            switch(key)
            {
            case LEFT:
            case RIGHT:
                break;

            case UP:
                do
                {
                    _curidx--;  // Go to the previous index
                    (_curidx < 0) && (_curidx = _fnum - 1); // If the current index goes below 0, set the index to the index of the last field
                }
                while(!_fld[_curidx]->editable());  // Iterate through the loop until an editable field is reached

                break;

            case ENTER:
            case DOWN:
            case TAB:
                do
                {
                    _curidx++;  // Go to the next index
                    (_curidx >= _fnum) && (_curidx = 0);    //If the current index reaches the last index, set it to 0
                }
                while(!_fld[_curidx]->editable());  // Iterate through the loop until an editable field is reached

                break;

            default:
                done = true;
                break;
            }   // switch
        }   // while
    }   // else

    return key;
}

// Adds a field to the dialog
int CDialog::add(CField* field, bool dynamic)
{
    // Expand the pointer arrays to be able to hold more pointers if the number of fields reaches the max field size
    if(_fnum == _fldSize)
    {
        // Create temporary variables
        CField** tempf = new CField*[_fldSize + C_DIALOG_EXPANSION_SIZE];
        bool* tempd = new bool[_fldSize + C_DIALOG_EXPANSION_SIZE];
        unsigned int i;

        // Transferring the data from the old arrays
        for(i = 0; i < _fldSize; i++)
        {
            tempf[i] = _fld[i];
            tempd[i] = _dyn[i];
        }

        delete [] _dyn;
        delete [] _fld;

        // Pointing old pointers to new data
        _fld = tempf;
        _dyn = tempd;
        _fldSize += C_DIALOG_EXPANSION_SIZE;
    }

    // Add field pointer and whether it was dynamically allocated to their respective arrays
    _fld[_fnum] = field;
    _dyn[_fnum] = dynamic;
    (field->editable()) && (_editable = true);  // Set the dialogs _editable to true if the field is editable
    field->container(this); // Set the fields container
    return _fnum++; // Return the fields index, then increment it
}

// Call add with a reference to the field entered
int CDialog::add(CField& field, bool dynamic)
{
    return add(&field, dynamic);
}

// Call add with the fields address and return a pointer to this dialog
CDialog& CDialog::operator<<(CField* field)
{
    add(field);
    return *this;
}

// Call add with the referenced field and return a pointer to this dialog
CDialog& CDialog::operator<<(CField& field)
{
    add(field);
    return *this;
}

// Returns whether this dialog is editable
bool CDialog::editable()const
{
    return _editable;
}

// Returns the number of fields in the dialog
int CDialog::fieldNum()const
{
    return _fnum;
}

// Returns the current index
int CDialog::curIndex()const
{
    return _curidx;
}

// Returns a pointer to the field pointed to by index
CField& CDialog::operator[](unsigned int index)
{
    return *_fld[index];
}

// Returns a pointer to the field pointed to by _curidx
CField& CDialog::curField()
{
    return *_fld[_curidx];
}

}