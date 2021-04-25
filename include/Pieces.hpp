#ifndef PIECES_H
#define PIECES_H

#include <string>

/**
* The Color enumerator is used in place of values 1 and -1 
* in order to make the code easier to read. Note the values 
* of 1 and -1. This allows for alternating play by multiplying the current 
* by 1. 

*/
enum class Color
{
    RED = 1,
    BLACK = -1
};

/**
* The Pieces class represents all pieces for a particular player. Each object
* of the class contains a single instance of pieces. In addition to this,
* ANSII code strings were included here in order to make the output
* of the code easier to read. These codes serve no functional purpose
* aside from changing text color.
*/
class Pieces
{
public:
    Pieces();
    Pieces(Color color);

    // ANSII codes for colored text, to improve UI and readability
    static std::string ANSII_BLUE_START;
    static std::string ANSII_RED_START;
    static std::string ANSII_RED_HIGH;
    static std::string ANSII_END;
    static std::string ANSII_GREEN_START;
    static std::string ANSII_BLUE_COUT;
    static std::string ANSII_RED_COUT;
    static std::string ANSII_GREEN_COUT;
    static std::string ANSII_YELLOW_COUT;

    // Debug reporting level 3 == display all debug/status lines 2== important, 1 == basic, 0 == none
    static int ouputDebugData;

    bool isKing(int position) const;
    void setKing(int poisition, bool toKing);

    // This contains 64 bits to represent the entire piece set and king status for one side.
    long long pieces;
};

#endif // !PIECES_H
