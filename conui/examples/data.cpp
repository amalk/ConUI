// Generates data.bn used for testing the app

#include <iostream>
#include <fstream>
#include <iomanip>  // setw()

struct Song {
    char name[71];
    char artist[71];
    char album[71];
    char releaseDate[11];
    unsigned long genre;
    unsigned long rating;
};

std::ostream& operator<<(std::ostream& os, const Song& temp) {
    std::cout << std::setw(25) << temp.name        << ' '
              << std::setw(15) << temp.artist      << ' '
              << std::setw(25) << temp.album       << ' '
                               << temp.releaseDate << ' '
                               << temp.genre       << ' '
                               << temp.rating;
    return os;
}

int main() {
    std::string fileN = "data.bn";
    unsigned int sz = sizeof(Song);
    std::ifstream in;
    Song temp;

    /* --- START WRITE --- */
    std::ofstream out(fileN, std::ios::binary);

    Song arr[10] = {
        {"Speak to Me",              "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"Breathe",                  "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"On the Run",               "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"Time",                     "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"The Great Gig in the Sky", "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"Money",                    "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"Us and Them",              "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"Any Colour You Like",      "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"Brain Damage",             "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
        {"Eclipse",                  "Pink Floyd", "Dark Side of the Moon", "1973/03/01", 1, 1},
    };

    out.write((const char*)arr, sizeof(arr));

    out.close();
    /* --- END WRITE --- */

    /* --- READ TEST --- */

    in.open(fileN, std::ios::binary);

    while(in.read((char*)&temp, sz).good())
        std::cout << temp << '\n';

    in.close();

    std::cin.get();

    return 0;
}
