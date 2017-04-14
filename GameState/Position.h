#pragma once

#if !defined(Position_h__)
#define Position_h__

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
    std::string fen() const;
};

inline bool operator ==(Position const & a, Position const & b)
{
    return a.row == b.row && a.column == b.column;
}

#endif // !defined(Position_h__)
