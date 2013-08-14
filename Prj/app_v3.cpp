#include "musicdb.h"

int main(int argc, char* argv[])
{
    char fileN[FILENAME_MAX];

    if(argc == 1)
    {
        GetFilename(fileN);
    }
    else if(argc == 2)
    {
        bio::strcpy(fileN, argv[1]);
        GetFilename(fileN, false);
    }

    MusicDB db(fileN);
    db.initialize();
    db.edit();
    return 0;
}
