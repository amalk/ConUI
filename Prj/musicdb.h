#include <fstream>
#include "console.h"
#include "cdialog.h"
#include "clabel.h"
#include "cmenu.h"
#include "clineedit.h"
#include "cchecklist.h"
#include "cveditline.h"
#include "ctext.h"

using namespace cui;

/* START - PROTOTYPES */
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

class MusicDB
{
private:
    int index;
    bool insertMode;
    int idxFileMenu;
    int idxEditMenu;
    int songSize;
    char filename[FILENAME_MAX];
    CDialog mainWindow;
    CDialog interface;
    Song song;
    CLabel name;
    CLabel artist;
    CLabel album;
    CLabel released;
    CLabel genre;
    CLabel rating;
    CButton topButton;
    CButton bottomButton;
    CButton nextButton;
    CButton previousButton;
    CButton jump;
    CMenu fileMenu;
    CMenu editMenu;
    CMenu songMenu;
    CLineEdit jumpTo;
    CLabel message;    // output field for testing purposes


    /* START - PRIVATE MEMBER FUNCTION DEFINITIONS */

    void RefreshInfo()
    {
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
        (decoding == 48) ? rating.set("") : rating.set(&decoding);
        name.draw();
        artist.draw();
        album.draw();
        released.draw();
        genre.draw();
        rating.draw();
    }

    // clear data in 'song'
    void clearSong()
    {
        Song temp;
        song = temp;
    }

    std::streamoff NumberSongs()
    {
        std::ifstream temp(filename);
        temp.seekg(0, temp.end);
        return temp.tellg() / songSize;
    }

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

    bool ShowEdit()
    {
        int key = 0;
        bool insert = true;
        bool done = false;
        bool rv = false;    // returns true if save was pressed, false if cancel was pressed
        Song song_orig = song;
        CDialog   edit(&interface, 2, 2, 60, 20, true);
        CButton   save("Save", 5, 48);
        CButton   cancel("Cancel", 10, 47);
        CLabel editMessage("", 17, 2);
        CLineEdit name(song.name, 3,  2, 20, 70, &insert, true);
        CLineEdit artist(song.artist, 8,  2, 20, 70, &insert, true);
        CLineEdit album(song.album, 13,  2, 20, 70, &insert, true);
        CValEdit released(song.releaseDate, 3, 24, 20, 10, &insert, ValidDate, NO_HELPFUNC, true);
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

            if(!bio::strlen((char*)name.data()) && &edit.curField() != &cancel)
            {
                edit.draw();
                editMessage.set("The name field cannot be blank!");
                editMessage.draw();
                console.getKey();
                editMessage.set("");
                editMessage.draw();
            }
            else
            {
                switch(key)
                {
                case C_BUTTON_HIT:
                case ESCAPE:
                case F(10):
                    if(&edit.curField() == &save || (&edit.curField() != &cancel && Yes("Save record before quitting?")))
                    {
                        bio::strcpy(song.name, (char*)name.data());
                        bio::strcpy(song.artist, (char*)artist.data());
                        bio::strcpy(song.album, (char*)album.data());
                        bio::strcpy(song.releaseDate, (char*)released.data());
                        song.genre = 0;  //reinitialize the genre and rating
                        song.rating = 0;
                        song.genre = (unsigned long)genre.data();
                        song.rating = (unsigned long)rating.data();
                        rv = true;
                    }
                    else
                    {
                        rv = false;
                        song = song_orig;
                    }

                    done = true;
                    break;
                }   // switch
            }       // else
        }           // while

        edit.hide();
        return rv;
    }

    bool ReadFromFile(int i)
    {
        bool rv = false;
        std::ifstream temp(filename, std::ios::binary);

        if(i < NumberSongs() && temp.good())
        {
            temp.seekg(i * songSize, temp.beg);
            temp.read((char*)&song, songSize);
            rv = true;
        }

        return rv;
    }

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

    void EditRecord(int i)
    {
        // FILE
        bool rv = false;
        std::fstream temp(filename, std::ios::out | std::ios::in | std::ios::binary);

        if(i < NumberSongs() && temp.good())
        {
            temp.seekp(i * songSize, temp.beg);
            temp.write((char*)&song, songSize);
            rv = true;
        }

        temp.close();
        
        // MENU
        songMenu.editNode(i, song.name);
        songMenu.draw();
        RefreshInfo();
    }

    void DeleteRecord(int i)
    {
        // FILE
        std::ifstream readData(filename, std::ios::binary);
        readData.seekg(0, readData.end);
        int numSongs = (int)(((readData.tellg()) / songSize) - 1);
        int filesize = numSongs * songSize;
        int firsthalf = i * songSize;
        char* buffer = new char[filesize];
        readData.seekg(0, readData.beg);  //read the part of the file before the index
        readData.read(buffer, firsthalf);
        readData.seekg((i + 1) * songSize, readData.beg);  //read the part of the file after the index
        readData.read(buffer + firsthalf, songSize * (numSongs - i));
        readData.close();
        std::ofstream writeData(filename, std::ios::trunc | std::ios::binary);
        writeData.write(buffer, filesize);
        writeData.close();
        delete [] buffer;

        // MENU
        songMenu.deleteNode(i);

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
    MusicDB(const char* fileN) :
        index(0),
        insertMode(true),
        songSize(sizeof(Song)),
        interface(&mainWindow, 2, 2, 80, 27, true),
        fileMenu("File", "[]", 0, 1, 14, 5, true),
        editMenu("Edit", "[]", 0, 7, 11, 5, true),
        songMenu("Songs", "<>", 5, 2, 29, 12, false),
        name(5, 45, 29),
        artist(7, 45, 29),
        album(9, 45, 29),
        released(11, 45, 20),
        genre(13, 45, 15),
        rating(15, 45, 5),
        topButton("Top", 2, 2),
        bottomButton("Bottom", 17, 2),
        nextButton("Next", 17, 23),
        previousButton("Previous", 2, 19),
        jump("Jump to:", 20, 2),
        jumpTo(20, 25, 6, 3, &insertMode, true),
        message(24, 2, 55)
    {
        bio::strcpy(filename, fileN);
        idxFileMenu = interface.add(fileMenu);    // reference for help
        idxEditMenu = interface.add(editMenu);
    }

    virtual ~MusicDB()
    {
    }

    void initialize()
    {
        fileMenu << "Help" << "About" << "Quit";
        editMenu << "Add" << "Edit" << "Delete";
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

        while(ReadFromFile(index++))
        {
            songMenu.add(song.name);
        }

        clearSong();
    }

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
            case F(1):
                fieldNum = ShowHelp();
                break;

            case F(2):
                fieldNum = idxFileMenu + 1;
                break;

            case F(3):
                fieldNum = idxEditMenu + 1;
                break;

            case ESCAPE:
            case F(10):
                if(Yes("Confirm quit."))
                {
                    done = true;
                }

                break;

            case C_BUTTON_HIT:
                fieldNum = interface.curIndex() + 1;

                if(NumberSongs() > 0)
                {
                    if(&interface.curField() == &topButton)
                    {
                        songMenu.navUp(true);
                        ReadFromFile(0);
                    }
                    else if(&interface.curField() == &previousButton)
                    {
                        songMenu.navUp();
                        ReadFromFile(songMenu.selectedIndex());
                    }
                    else if(&interface.curField() == &bottomButton)
                    {
                        songMenu.navDown(true);
                        ReadFromFile(songMenu.selectedIndex());
                    }
                    else if(&interface.curField() == &nextButton)
                    {
                        songMenu.navDown();
                        ReadFromFile(songMenu.selectedIndex());
                    }
                    else if(&interface.curField() == &jump)
                    {
                        if(songMenu.navTo(atoi((const char*)jumpTo.data())))
                        {
                            ReadFromFile(songMenu.selectedIndex());
                        }
                        else
                        {
                            message.set("Cannot jump to that song!");
                            message.draw();
                        }
                    }

                    RefreshInfo();
                    songMenu.draw(C_NO_FRAME);
                    message.set("");
                }

                break;

            case SPACE:
                fieldNum = interface.curIndex() + 1;

                if(&interface.curField() == &songMenu)
                {
                    ReadFromFile(songMenu.selectedIndex());
                    RefreshInfo();
                }
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
                        }

                        break;

                    case 1: // edit
                        index = songMenu.selectedIndex();

                        // if edited information was SAVED, then edit the record in the file
                        if(index != -1 && ShowEdit())
                        {
                            EditRecord(index);
                        }

                        break;

                    case 2: // delete
                        if(Yes("Confirm deletion of the record."))
                        {
                            index = songMenu.selectedIndex();

                            if(index != -1)
                            {
                                DeleteRecord(index);
                            }
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

//
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

            if(key == C_BUTTON_HIT)
            {
                if(strlen((char*)le_filename.data()) < 1)
                {
                    error.set("The filename cannot be blank.");
                }
                else
                {
                    tempFile.open((char*)le_filename.data());
                    bio::strcpy(fname, le_filename.data());
                    done = true;
                }
            }
        }
    }
    else
    {
        tempFile.open(fname);
    }
}

bool ValidDate(const char* date, CDialog& edit)
{
    bool check = true;
    char tempc[3];
    int tempm;
    int tempd;
    int i;
    int months[12] = {31, 28, 31, 30, 31, 30, 30, 31, 30, 31, 30, 31};

    for(i = 0; check && i < 4; check = (bool)isdigit(date[i]), i++);

    check && (check = (date[i++] == '/'));

    for(; check && i < 7; check = (bool)isdigit(date[i]), tempc[i - 5] = date[i], i++);

    check && (tempm = atoi(tempc));
    check && (check = (tempm <= 12 && tempm > 0));
    check && (check = (date[i++] == '/'));

    for(; check && i < 10; check = (bool)isdigit(date[i]), tempc[i - 8] = date[i], i++);

    check && (tempd = atoi(tempc));
    check && (check = (tempd <= months[tempm - 1] && tempd > 0));

    if(check)
    {
        edit[6].set("");
    }
    else
    {
        edit[6].set("Incorrect Date format. Use YYYY/MM/DD");
    }

    edit.draw(C_NO_FRAME);
    return check;
}
