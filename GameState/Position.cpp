#include "Position.h"

bool Position::initializeFromFen(char const * start, char const * end)
{
    if (end != start + 2 || start[0] < 'a' || start[0] > 'h' || start[1] < '1' || start[1] > '8')
        return false;
    row    = 7 - (start[1] - '1');
    column = start[0] - 'a';
    return true;
}

std::string Position::notation() const
{
    std::string result;
    if (row < 0 || row > 7 || column < 0 || column > 7)
    {
        result = "-";
    }
    else
    {
        result += char(column + 'a');
        result += char(7 - row + '1');
    }
    return result;
}
