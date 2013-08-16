#include <fstream>
#include "cdialog.h"
#include "clabel.h"
#include "cmenu.h"
#include "clineedit.h"
#include "cchecklist.h"
#include "cveditline.h"
#include "ctext.h"

using namespace cui;

/* NON-MEMBER FUNCTION PROTOTYPES */
bool ValidDate(const char*, CDialog&);
void GetFilename(char*, bool);
/* END - PROTOTYPES */

struct Song
{
    char name[71];
    char artist[71];
    char album[71];
    char releaseDate[11];
    unsigned long genre;
    unsigned long rating;
    Song() : name(), artist(), album(), releaseDate(), genre(0), rating(0) {}
};

// Class than encapsulates the whole application
class MusicDB
{
private:
    bool insertMode;
    char filename[FILENAME_MAX];
    int index;
    int idxFileMenu;
    int idxEditMenu;
    int songSize;
    Song song;

    CDialog mainWindow;
    CDialog interface;
    CLabel  name;
    CLabel  artist;
    CLabel  album;
    CLabel  released;
    CLabel  genre;
    CLabel  rating;
    CButton topButton;
    CButton bottomButton;
    CButton nextButton;
    CButton previousButton;
    CButton jump;
    CMenu   fileMenu;
    CMenu   editMenu;
    CMenu   songMenu;
    CLineEdit jumpTo;
    CLabel message;


    /* START - PRIVATE MEMBER FUNCTION DEFINITIONS */

    // Sets the labels to the info contained in struct 'song'
    void RefreshInfo()
    {
        // variable to hold the decoded bit-pattern
        unsigned long decoding;
        name.set(song.name);
        artist.set(song.artist);
        album.set(song.album);
        released.set(song.releaseDate);
        decoding = decodeLong(song.genre);

        switch(decoding)
        {
        case 0:
            genre.set("");
            break;

        case 1:
            genre.set("Rock");
            break;

        case 2:
            genre.set("Pop");
            break;

        case 3:
            genre.set("Jazz");
            break;

        case 4:
            genre.set("Electronic");
            break;

        case 5:
            genre.set("Classic");
            break;

        case 6:
            genre.set("Foreign");
            break;

        case 7:
            genre.set("Heavy Metal");
            break;
        }

        decoding = decodeLong(song.rating) + 48;
        // only display non-zero rating
        (decoding == 48) ? rating.set("") : rating.set(&decoding);
        name.draw();
        artist.draw();
        album.draw();
        released.draw();
        genre.draw();
        rating.draw();
    }

    // Clears data contained in 'song'
    void clearSong()
    {
        Song temp;
        song = temp;
    }

    // Returns the number of songs present in the file
    std::streamoff NumberSongs()
    {
        std::ifstream temp(filename);
        temp.seekg(0, temp.end);
        return temp.tellg() / songSize;
    }

    // Displays a confirmation dialog with the message received in the parameter
    // Returns true if 'Yes' was pressed, false otherwise
    bool Yes(const char* message)
    {
        int key;
        bool res = false;
        bool done = false;
        CButton bt_yes("Yes", 4, 10, true, "     _  ");
        CButton bt_no("No", 4, 22, true, "     _  ");
        CDialog YesNo(&interface, 8, 24, 40, 10, true);
        YesNo << new CLabel(2, 3, 36) << bt_yes << bt_no;
        YesNo[0].set(message);
        YesNo.draw(C_FULL_FRAME);

        while(!done)
        {
            key = YesNo.edit();

            if(key == C_BUTTON_HIT)
            {
                res =  &YesNo.curField() == &bt_yes;
                done = true;
            }
        }

        YesNo.hide();
        return res;
    }

    // Displays a dialog containing labels that describe the function keys
    int ShowHelp()
    {
        CDialog help(&interface, 5, 20, 50, 11, true);
        help << new CLabel("F1: Display help", 2, 3)
             << new CLabel("F2: File Menu", 4, 3, 36)
             << new CLabel("F3: Edit Menu", 6, 3, 36)
             << new CLabel("F10 / Esc: Exit the test program.", 8, 3);
        help.edit(C_FULL_FRAME);
        help.hide();
        return interface.curIndex() + 1;
    }

    // Displays a dialog with editable fields holding information for a record
    bool ShowEdit()
    {
        int  key       = 0;
        bool insert    = true;
        bool done      = false;
        bool rv        = false; // return true if save was pressed
        Song song_orig = song;  // save original info to restore if cancel is pressed
        
        CDialog    edit(&interface, 2, 2, 60, 20, true);
        CButton    save("Save", 5, 48);
        CButton    cancel("Cancel", 10, 47);
        CLabel     editMessage("", 17, 2);
        CLineEdit  name(song.name, 3,  2, 20, 70, &insert, true);
        CLineEdit  artist(song.artist, 8,  2, 20, 70, &insert, true);
        CLineEdit  album(song.album, 13,  2, 20, 70, &insert, true);
        CValEdit   released(song.releaseDate, 3, 24, 20, 10, &insert, ValidDate, NO_HELPFUNC, true);
        CCheckList genre("(o)", 8, 23, 15, true, false);
        CCheckList rating("(o)", 8, 38, 15, true, false);
        
        genre.add("Rock");
        genre.add("Pop");
        genre.add("Jazz");
        genre.add("Electronic");
        genre.add("Classic");
        genre.add("Foreign");
        genre.add("Heavy Metal");
        rating.add("1");
        rating.add("2");
        rating.add("3");
        rating.add("4");
        rating.add("5");
        genre.set(&song.genre);
        rating.set(&song.rating);
        edit << new CLabel("Name:", 2, 3)
             << new CLabel("Artist:", 7, 3)
             << new CLabel("Album:", 12, 3)
             << new CLabel("Released:", 2, 25)
             << new CLabel("Genre:", 7, 25)
             << new CLabel("Rating:", 7, 38)
             << editMessage
             << name
             << artist
             << album
             << released
             << genre
             << rating
             << save
             << cancel;

        while(!done)
        {
            key = edit.edit();

            switch(key)
            {
            case C_BUTTON_HIT:
            case ESCAPE:
            case F(10):
                // copy the data from the fields to the struct 'song'
                if(&edit.curField() == &save || Yes("Save record before quitting?"))
                {
                    bio::strcpy(song.name, (char*)name.data());
                    bio::strcpy(song.artist, (char*)artist.data());
                    bio::strcpy(song.album, (char*)album.data());
                    bio::strcpy(song.releaseDate, (char*)released.data());
                    song.genre = 0;  // reset the genre and rating
                    song.rating = 0;
                    song.genre = (unsigned long)genre.data();
                    song.rating = (unsigned long)rating.data();
                    rv = true;
                }
                // return false and restore original data if cancel is hit
                // or if the user declines to save information
                else
                {
                    rv = false;
                    song = song_orig;
                }

                done = true;
                break;
            }   // switch
        }       // while

        edit.hide();
        return rv;
    }

    // Loads a single record from the file at index i into the struct song
    // Returns true on a successful read
    bool ReadFromFile(int i)
    {
        bool rv = false;
        std::ifstream temp(filename, std::ios::binary);

        if(i < NumberSongs() && temp.good())
        {
            // position the HEAD at the starting of the record requested
            temp.seekg(i * songSize, temp.beg);
            temp.read((char*)&song, songSize);
            rv = true;
        }

        return rv;
    }

    // Appends a record to the file and the menu with information from struct song
    void AddRecord()
    {
        // FILE
        std::ofstream temp(filename, std::ios::app | std::ios::binary);
        temp.write((char*)&song, songSize);
        temp.close();
        
        // MENU
        songMenu.add(song.name, true);
        songMenu.draw();
        ReadFromFile(songMenu.selectedIndex());
        RefreshInfo();
    }

    // Overwrites the record at index i with the information from struct song
    void EditRecord(int i)
    {
        // FILE
        std::fstream temp(filename, std::ios::out | std::ios::in | std::ios::binary);

        if(i < NumberSongs() && temp.good())
        {
            // position the HEAD at the starting of the record to be overwritten
            temp.seekp(i * songSize, temp.beg);
            temp.write((char*)&song, songSize);
        }

        temp.close();
        
        // MENU
        // update the label of the menu item at index i with the name of the edited song
        songMenu.editNode(i, song.name);
        songMenu.draw();
        RefreshInfo();
    }

    // Removes the record located at index i from the file and the menu
    void DeleteRecord(int i)
    {
        // FILE
        std::ifstream readData(filename, std::ios::binary);
        readData.seekg(0, readData.end);
        int numSongs = (int)(((readData.tellg()) / songSize) - 1);
        int filesize = numSongs * songSize;
        int firsthalf = i * songSize;
        char* buffer = new char[filesize];
        // load the records present before the index to be deleted into the buffer
        readData.seekg(0, readData.beg);  
        readData.read(buffer, firsthalf);
        // load the records present after the index to be deleted into the buffer
        readData.seekg((i + 1) * songSize, readData.beg);  
        readData.read(buffer + firsthalf, songSize * (numSongs - i));
        readData.close();
        // truncate and write the records from the buffer into the file
        std::ofstream writeData(filename, std::ios::trunc | std::ios::binary);
        writeData.write(buffer, filesize);
        writeData.close();
        delete[] buffer;

        // MENU
        songMenu.deleteNode(i);

        // update the selected record
        if(songMenu.selectedIndex() > 0)
        {
            songMenu.selectedIndex(i - 1);
            ReadFromFile(songMenu.selectedIndex());
            songMenu.draw();
        }
        else if(songMenu.selectedIndex() == 0)
        {
            songMenu.selectedIndex(numSongs ? 0 : -1);

            if(numSongs)
            {
                ReadFromFile(songMenu.selectedIndex());
            }
            else
            {
                clearSong();
            }

            songMenu.draw();
        }

        RefreshInfo();
    }

    // Displays a dialog containing credits for the application
    // (had to use CText somehow)
    void ShowAbout()
    {
        char about[] = "  Written by:              \n\n"
                       "  Team-K                     \n"
                       "         Amal Khandelwal     \n"
                       "          Artem Luzyanin     \n"
                       "             Elliot Kwan \n\n\n"
                       " With help from OOP344       \n"
                       " Summer-2013 teachers:     \n\n"
                       " Fardad Soleimanloo          \n"
                       " Joseph Hughes           \n\n\n"
                       " Press any key to exit.        ";

        CDialog aboutWindow(&interface, 2, 2, 36, 20, true);
        CLabel  editMessage(" About ", 0, 1);
        CText   aboutText(about, 1, 1, 30, 18, false, true, "        ");

        aboutWindow << editMessage
                    << aboutText;

        aboutWindow.draw();
        console.getKey();
        aboutWindow.hide();
    }

    // Returns the place value of 1 in the incoming parameter
    unsigned long decodeLong(unsigned long longNumber)
    {
        unsigned long temp = 1;

        if(longNumber == 0)
        {
            temp = 0;
        }
        else
        {
            while(longNumber % 2 != 1)
            {
                temp++;
                longNumber /= 2;
            }
        }

        return temp;
    }

    /* END - PRIVATE MEMBER FUNCTION DEFINITIONS */


public:
    // Constructor accepting the current filename as a char*
    // Initializes the dialogs and the fields in the application
    MusicDB(const char* fileN) :
        index(0),
        insertMode(true),
        songSize(sizeof(Song)),
        interface(&mainWindow, 2, 2, 80, 27, true),
        fileMenu ("File", "[]", 0, 1, 14, 5, true),
        editMenu ("Edit", "[]", 0, 7, 11, 5, true),
        songMenu ("Songs", "<>", 5, 2, 29, 12, false),
        name     (5,  45, 29),
        artist   (7,  45, 29),
        album    (9,  45, 29),
        released (11, 45, 20),
        genre    (13, 45, 15),
        rating   (15, 45,  5),
        topButton     ("Top", 2, 2),
        bottomButton  ("Bottom", 17, 2),
        nextButton    ("Next", 17, 23),
        previousButton("Previous", 2, 19),
        jump   ("Jump to:", 20, 2),
        jumpTo (20, 25, 6, 3, &insertMode, true),
        message(24, 2, 55)
    {
        bio::strcpy(filename, fileN);
        idxFileMenu = interface.add(fileMenu);    // reference for help
        idxEditMenu = interface.add(editMenu);
    }

    // Destructor that can contain clean-up code
    virtual ~MusicDB()
    {
    }

    // Adds the fields and records to the menus and dialogs
    void initialize()
    {
        fileMenu << "Help" << "About" << "Quit";
        editMenu << "Add"  << "Edit"  << "Delete";
        mainWindow << new CLabel("F1: HELP", 0, 2);
        mainWindow.draw();
        interface << new CLabel("Name:", 5, 39)
                  << new CLabel("Artist:", 7, 37)
                  << new CLabel("Album:", 9, 38)
                  << new CLabel("Released:", 11, 35)
                  << new CLabel("Genre:", 13, 38)
                  << new CLabel("Rating:", 15, 37)
                  << name
                  << artist
                  << album
                  << released
                  << genre
                  << rating
                  << message
                  << topButton
                  << previousButton
                  << songMenu
                  << bottomButton
                  << nextButton
                  << jump
                  << jumpTo;

        // load the menu with the name of all the songs present in the file
        while(ReadFromFile(index++))
        {
            songMenu.add(song.name);
        }

        // select the first record in the menu
        if(songMenu.editable()){
            ReadFromFile(0);
            songMenu.navUp(true);
            RefreshInfo();
        }

        clearSong();
    }

    // Edits the fields in the interface dialog until the user chooses to quit
    void edit()
    {
        bool done = false;
        int key = 0;
        int fieldNum = 0;
        interface.draw();

        while(!done)
        {
            key = interface.edit(fieldNum);
            message.draw();

            switch(key)
            {
            // Display help dialog
            case F(1):
                fieldNum = ShowHelp();
                break;

            // Focus on the file menu
            case F(2):
                fieldNum = idxFileMenu + 1;
                break;

            // Focus on the edit menu
            case F(3):
                fieldNum = idxEditMenu + 1;
                break;

            // Confirm exiting the application
            case ESCAPE:
            case F(10):
                if(Yes("Confirm quit."))
                {
                    done = true;
                }

                break;

            // Handles Top, Previous, Bottom, Next and Jump To button hits
            case C_BUTTON_HIT:
                fieldNum = interface.curIndex() + 1;

                // The buttons only function if there is at least one record in the menu
                if(NumberSongs() > 0)
                {
                    // Selects and changes current index to the first record in the menu
                    if(&interface.curField() == &topButton)
                    {
                        songMenu.navUp(true);
                        ReadFromFile(0);
                    }
                    // Selects and changes current index to the previous record in the menu
                    else if(&interface.curField() == &previousButton)
                    {
                        songMenu.navUp();
                        ReadFromFile(songMenu.selectedIndex());
                    }
                    // Selects and changes current index to the last record in the menu
                    else if(&interface.curField() == &bottomButton)
                    {
                        songMenu.navDown(true);
                        ReadFromFile(songMenu.selectedIndex());
                    }
                    // Selects and changes current index to the next record in the menu
                    else if(&interface.curField() == &nextButton)
                    {
                        songMenu.navDown();
                        ReadFromFile(songMenu.selectedIndex());
                    }
                    // Selects and changes current index to the entered value if possible
                    else if(&interface.curField() == &jump)
                    {
                        if(songMenu.navTo(atoi((const char*)jumpTo.data())))
                        {
                            ReadFromFile(songMenu.selectedIndex());
                        }
                        // Display an error if the requested record number is invalid
                        else
                        {
                            message.set("Cannot jump to that song!");
                            message.draw();
                        }
                    }

                    // Update record information displayed on the right
                    RefreshInfo();
                    songMenu.draw(C_NO_FRAME);
                    message.set("");
                }

                break;

            // Handles File, Edit and Song menu functionality
            case SPACE:
                fieldNum = interface.curIndex() + 1;

                // A record was selected in the Song menu
                if(&interface.curField() == &songMenu)
                {
                    ReadFromFile(songMenu.selectedIndex());
                    RefreshInfo();
                }
                // An option from the File menu was chosen
                else if(&interface.curField() == &fileMenu)
                {
                    switch(fileMenu.selectedIndex())
                    {
                    case 0: // help
                        ShowHelp();
                        break;

                    case 1: // about
                        ShowAbout();
                        break;

                    case 2: // quit
                        (Yes("Confirm quit.")) && (done = true);
                        break;
                    }
                }
                // An option was chosen from the Edit menu
                else if(&interface.curField() == &editMenu)
                {
                    switch(editMenu.selectedIndex())
                    {
                    case 0: // add
                        clearSong();

                        // if SAVE was pressed on the edit dialog, then add the record
                        if(ShowEdit())
                        {
                            AddRecord();
                            songMenu.firstToEnd();
                            songMenu.draw(C_NO_FRAME);
                        }

                        break;

                    case 1: // edit
                        index = songMenu.selectedIndex();

                        // if edited information was SAVED, then edit the record in the file
                        if(index != -1 && ReadFromFile(index) && ShowEdit())
                        {
                            EditRecord(index);
                        }

                        break;

                    case 2: // delete
                        index = songMenu.selectedIndex();

                        // Ask for deletion of a selected record; delete upon confirmation
                        if(index != -1 && Yes("Confirm deletion of the record."))
                        {
                            DeleteRecord(index);
                        }

                        break;
                    }   // switch()  -- editMenu's selectedIndex
                }       // else if() -- Edit Menu

                break;
            }           // switch()  -- key from fieldEdit
        }               // while()   -- not done browsing
    }                   // edit()

};

/* END - MUSICDB CLASS DEFINITION */


/* START - NON-MEMBER FUNCTION DEFINITIONS */

// Displays a dialog to request the user for a filename if prompt is true
// Creates the file if the entered filename does not exist
void GetFilename(char* fname, bool prompt = true)
{
    std::fstream tempFile;

    if(prompt)
    {
        bool insertMode = true;
        CDialog owner;
        CDialog nameInput(&owner, 5, 30, 35, 13, true);
        CLineEdit le_filename(4, 2, 31, FILENAME_MAX, &insertMode, true);
        CButton bt_okay("Okay", 9, 13);
        CLabel error(7, 3, 31);
        int key = 0;
        bool done = false;
        nameInput << new CLabel("Enter the filename:", 2, 8)
                  << le_filename
                  << bt_okay
                  << error;
        nameInput.draw(C_FULL_FRAME);

        while(!done)
        {
            key = nameInput.edit();

            // Okay button was hit
            if(key == C_BUTTON_HIT)
            {
                // Display an error if no filename was entered
                if(strlen((char*)le_filename.data()) < 1)
                {
                    error.set("The filename cannot be blank.");
                }
                // Create the file if it does not exist
                // And copy the filename into the parameter
                else
                {
                    tempFile.open((char*)le_filename.data());
                    bio::strcpy(fname, le_filename.data());
                    done = true;
                }
            }
        }
    }
    // prompt is false, filename was received as a command line argument
    // create the file if it does not exist
    else
    {
        tempFile.open(fname);
    }
}

// Validates the data entered in the "Released Date" field for a record
bool ValidDate(const char* date, CDialog& edit)
{
    bool check = true;
    char tempc[3];
    int tempm;
    int tempd;
    int i;
    // number of days in each month in a year
    int months[12] = {31, 28, 31, 30, 31, 30, 30, 31, 30, 31, 30, 31};

    // First four characters must be digits representing a year
    for(i = 0; check && i < 4; check = (bool)isdigit(date[i]), i++);

    // The fifth character must be a forward slash, separating the year from month
    check && (check = (date[i++] == '/'));

    // The sixth and seventh characters must be digits representing a month
    for(; check && i < 7; check = (bool)isdigit(date[i]), tempc[i - 5] = date[i], i++);

    // Check if the month entered falls within range of 1 - 12, followed by a forward slash
    check && (tempm = atoi(tempc));
    check && (check = (tempm <= 12 && tempm > 0));
    check && (check = (date[i++] == '/'));

    // The last two characters must be digits representing the date
    for(; check && i < 10; check = (bool)isdigit(date[i]), tempc[i - 8] = date[i], i++);

    // Check if the date entered is valid for a specific month
    check && (tempd = atoi(tempc));
    check && (check = (tempd <= months[tempm - 1] && tempd > 0));

    // If all checks were passed, clear the error message
    if(check)
    {
        edit[6].set("");
    }
    // Display an error message if invalid data was entered
    else
    {
        edit[6].set("Incorrect Date format. Use YYYY/MM/DD");
    }

    edit.draw(C_NO_FRAME);
    return check;
}
