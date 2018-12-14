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
    bool check(GameState const & state,
               int * pReturnedValue = NULL,
               int * pReturnedQuality = NULL) const;

    // Returns true and value if the state is already in the table and the quality is high enough.
    bool check(GameState const & state,
               int minQ,
               int * pReturnedValue = NULL,
               int * pReturnedQuality = NULL) const;

    // Puts the state into the table
    void set(GameState const & state, int value, int quality);

    // Puts the state into the table, if its quality is high enough
    void update(GameState const & state, int value, int quality);

    // Bump the age of entries so that they are eventually replaced by newer entries.
    void age();

#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)

    // Analysis data

    struct AnalysisData
    {
        int checkCount;     // The number of checks
        int updateCount;    // The number of updates
        int hitCount;       // The number of times a state was found
        int collisionCount; // The number of times a different state was found
        int rejected;       // The number of times an update was rejected
        int overwritten;    // The number of times a state was overwritten by a different state
        int refreshed;      // The number of times a state was updated with a newer value
        int usage;          // The number of slots in use

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

        void clear() { hash_ = UNUSED_ENTRY; }

        ZHash::Z       hash_;  // The state's hash
        int            value_; // The state's value
        int8_t         q_;     // The quality of the value
        mutable int8_t age_;   // The number of turns since the entry has been referenced
    };

    TableEntry const & find(ZHash::Z hash) const    { return table_[hash % SIZE]; }
    TableEntry &       find(ZHash::Z hash)          { return table_[hash % SIZE]; }

    TableEntry table_[SIZE];
};

#endif // !defined(TranspositionTable_h__)
