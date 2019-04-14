#if !defined(CHESS_POSITION_H)
#define CHESS_POSITION_H

#pragma once

#include <string>

struct Position
{
    int row;
    int column;

    Position()
        : row(-1)
        , column(-1)
    {
    }

    Position(int r, int c)
        : row(r)
        , column(c)
    {
    }

    bool initializeFromFen(char const * start, char const * end);

    // Returns the notation for this position
    std::string notation() const;
};

inline bool operator ==(Position const & a, Position const & b)
{
    return a.row == b.row && a.column == b.column;
}

#endif // !defined(CHESS_POSITION_H)
