#pragma once

#if !defined(TranspositionTable_h__)
#define TranspositionTable_h__

#include "ZHash/ZHash.h"

class GameState;

#define TRANSPOSITION_TABLE_ANALYSIS_ENABLED

// A "transposition table" is a hash table containing the values of all the board states calculated so far.
// Actually, not all the states are saved. The slot for each state is not unique and only one state is stored in
// each slot, so storing a state may overwrite a different state in the same slot. This a speed and memory
// optimization. A state is overwritten only if its "quality" is less than or equal to the "quality" of the state
// being stored.
//
// A game state can be the result of different sequences of the same set of moves, albeit "transposed". This
// technique used to cache the value of a game state regardless of the sequence of moves used to reach it, thus
// the name "transposition" table. The purpose of the "transposition" table has been extended to become simply a
// cache of the values of game states, so it is more aptly named "game state value cache" -- but the old name
// persists.

class TranspositionTable
{
public:

    static int const INDEX_SIZE = 19;
    static int const SIZE       = 1 << INDEX_SIZE; // Number of entries in the table
    static int const MAX_AGE    = 1;               // Entries not referenced in this many turns are removed

    TranspositionTable();

    // Returns true and value if the state is already in the table
    bool check(GameState const & state, int * pReturnedValue, int8_t * pReturnedQuality) const;

    // Returns true and value if the state is already in the table and the quality is high enough.
    bool check(GameState const & state, int minQ, int * pReturnedValue) const;

    // Puts the state into the table
    void forceUpdate(GameState const & state);

    // Puts the state into the table, if its quality is high enough
    void update(GameState const & state);

    // Bump the age of entries so that they are eventually replaced by newer entries.
    void age();

#if defined(TRANSPOSITION_TABLE_ANALYSIS_ENABLED)

    // Resets all analysis data
    void resetAnalysisData();

    // Analysis data

    struct AnalysisData
    {
        int checkCount;
        int updateCount;
        int hitCount;
        int usage;
        int collisionCount;
        int rejected;
        int overwritten;
        int refreshed;

        AnalysisData();
        void reset();
    };

    mutable AnalysisData analysisData_;

#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

private:

    class TableEntry
    {
public:

        static uint32_t const UNUSED_ENTRY = (uint32_t)(ZHash::INVALID >> 32);

        void clear() { hashCode_ = UNUSED_ENTRY; }

        uint32_t hashCode_;  // The state's hash code (actually just the upper half)
        int value_;          // The state's value
        int8_t q_;           // The quality of the value
        mutable int8_t age_; // The number of turns since the entry has been referenced
    };

    TableEntry table_[SIZE];
};

#endif // !defined(TranspositionTable_h__)
