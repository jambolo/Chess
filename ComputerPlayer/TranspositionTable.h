#pragma once

#if !defined(TranspositionTable_h__)
#define TranspositionTable_h__

#include "ZHash/ZHash.h"

class GameState;

// A "transposition table" is a hash map containing the values of all the board states calculated so far.
// Actually, as a speed and memory optimization, not all the states are saved. The slot for each state is not
// unique and only one state is stored in each slot, so storing a state may overwrite a different state in the
// same slot. A state is overwritten only if its "quality" is less than or equal to the "quality" of the state
// being stored.
//
// A game state can be the result of different sequences of the same (or a different) set of moves. This
// technique is used to cache the value of a game state regardless of the sequence of moves used to reach it, thus
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
    bool check(GameState const & state, int * pReturnedValue, int * pReturnedQuality) const;

    // Returns true and value if the state is already in the table and the quality is high enough.
    bool check(GameState const & state, int minQ, int * pReturnedValue) const;

    // Puts the state into the table
    void forceUpdate(GameState const & state);

    // Puts the state into the table, if its quality is high enough
    void update(GameState const & state);

    // Bump the age of entries so that they are eventually replaced by newer entries.
    void age();

#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)

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

#endif // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

private:

    class TableEntry
    {
public:

        static ZHash::Z const UNUSED_ENTRY = ZHash::INVALID;

        void clear() { hashCode_ = UNUSED_ENTRY; }

        ZHash::Z hashCode_;  // The state's hash code
        int value_;          // The state's value
        int8_t q_;           // The quality of the value
        mutable int8_t age_; // The number of turns since the entry has been referenced
    };

    TableEntry const & find(ZHash::Z hash) const;
    TableEntry &       find(ZHash::Z hash);

    TableEntry table_[SIZE];
};

#endif // !defined(TranspositionTable_h__)
