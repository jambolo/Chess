#include "TranspositionTable.h"

#include "GameState/GameState.h"

#include "Misc/Etc.h"
#include <cassert>

TranspositionTable::TranspositionTable()
{
    // Invalidate all entries in the table

    for (auto & e : table_)
    {
        e.clear();
    }
}

bool TranspositionTable::check(GameState const & state, int * pReturnedValue, int * pReturnedQuality) const
{
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
    ++analysisData_.checkCount;
#endif // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

    ZHash::Z           hash  = state.zhash_.value();
    TableEntry const & entry = table_[hash % SIZE];

    // A hit occurs if the states are the same.

    bool hit = false;

    assert(hash != TableEntry::UNUSED_ENTRY);
    if (entry.hashCode_ == hash)
    {
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
        ++analysisData_.hitCount;
#endif  // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

        entry.age_        = 0;  // Reset age
        hit               = true;
        *pReturnedValue   = entry.value_;
        *pReturnedQuality = entry.q_;
    }
    else
    {
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
        if (entry.hashCode_ != TableEntry::UNUSED_ENTRY)
            ++analysisData_.collisionCount;

#endif  // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )
    }

    return hit;
}

bool TranspositionTable::check(GameState const & state, int minQ, int * pReturnedValue) const
{
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
    ++analysisData_.checkCount;
#endif // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

    ZHash::Z           hash  = state.zhash_.value();
    TableEntry const & entry = table_[hash % SIZE];

    // A hit occurs if the states are the same, and the minimum quality is <= the quality of the stored state. The
    // reason for the quality check is that if the stored quality is less, then we aren't going to want the value
    // of the stored state anyway.

    bool hit = false;

    if (entry.hashCode_ == hash)
    {
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
        ++analysisData_.hitCount;
#endif  // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

        entry.age_ = 0;         // Reset age

        if (entry.q_ >= minQ)
        {
            hit = true;
            *pReturnedValue = entry.value_;
        }
    }
    else
    {
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
        if (entry.hashCode_ != TableEntry::UNUSED_ENTRY)
            ++analysisData_.collisionCount;

#endif  // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )
    }

    return hit;
}

void TranspositionTable::forceUpdate(GameState const & state)
{
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
    ++analysisData_.updateCount;
#endif // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

    ZHash::Z     hash  = state.zhash_.value();
    TableEntry & entry = table_[hash % SIZE];

#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)

    // For tracking the number of used entries

    if (entry.hashCode_ == TableEntry::UNUSED_ENTRY)
        ++analysisData_.usage;
    else if (entry.hashCode_ == hash)
        ++analysisData_.refreshed;
    else
        ++analysisData_.overwritten;

#endif // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

    // Store the state, value and quality

    assert(state.quality_ < 256);
    entry.hashCode_ = hash;
    entry.value_    = state.value_;
    entry.q_        = state.quality_;
    entry.age_      = 0;
}

void TranspositionTable::update(GameState const & state)
{
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
    ++analysisData_.updateCount;
#endif // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

    ZHash::Z     hash  = state.zhash_.value();
    TableEntry & entry = table_[hash % SIZE];
    assert(hash != TableEntry::UNUSED_ENTRY);

    bool isUnused = (entry.hashCode_ == TableEntry::UNUSED_ENTRY);

    // If the entry is unused or if the new quality >= the stored quality, then store the new state. It is better
    // to replace values of equal quality in order to dispose of old entries that may no longer be relevant.

    if (isUnused || (state.quality_ >= entry.q_))
    {
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)

        // For tracking the number of used entries

        if (isUnused)
            ++analysisData_.usage;
        else if (entry.hashCode_ == hash)
            ++analysisData_.refreshed;
        else
            ++analysisData_.overwritten;

#endif  // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )

        // Store the state, value and quality

        entry.hashCode_ = hash;
        entry.value_    = state.value_;
        entry.q_        = state.quality_;
        entry.age_      = 0;
    }
    else
    {
#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
        ++analysisData_.rejected;
#endif  // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )
    }
}

// The T-table is persistent. So in order to gradually dispose of entries that are no longer relevant, entries
// that have not been reference for a while are removed.

void TranspositionTable::age()
{
    for (auto & entry : table_)
    {
        if (entry.hashCode_ != TableEntry::UNUSED_ENTRY)
        {
            ++entry.age_;
            if (entry.age_ > MAX_AGE)
            {
                entry.hashCode_ = TableEntry::UNUSED_ENTRY;

#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
                --analysisData_.usage;
#endif          // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )
            }
        }
    }
}

#if defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)

void TranspositionTable::resetAnalysisData()
{
    analysisData_.reset();
}

void TranspositionTable::AnalysisData::reset()
{
    checkCount  = 0;
    updateCount = 0;
    hitCount    = 0;
//	usage			= 0;	// never reset
    collisionCount = 0;
    rejected       = 0;
    overwritten    = 0;
    refreshed      = 0;
}

TranspositionTable::AnalysisData::AnalysisData()
    : usage(0)
{
    reset();
}

#endif // defined( FEATURE_TRANSPOSITION_TABLE_ANALYSIS )
