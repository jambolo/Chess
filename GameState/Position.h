#pragma once

#if !defined(Position_h__)
#define Position_h__

#include <vector>

struct Position
{
    int row;
    int column;

    static int const INVALID = -1;

    Position() {}

    Position(int r, int c)
        : row(r)
        , column(c)
    {
    }

    bool initializeFromFen(char const * start, char const * end);
};

inline bool operator ==(Position const & a, Position const & b)
{
    return a.row == b.row && a.column == b.column;
}

#endif // !defined(Position_h__)
