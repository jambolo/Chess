#include "gtest/gtest.h"
#include "BitBoard/BitBoard.h"

TEST(BitBoardTest, Constructor_default)
{
    BitBoard b;
    EXPECT_EQ(uint64_t(b), 0);
}

TEST(BitBoardTest, Constructor_value)
{
    BitBoard b(0x123456789ABCDEF0ULL);
    EXPECT_EQ(uint64_t(b), 0x123456789ABCDEF0ULL);
}

TEST(BitBoardTest, Set)
{
    BitBoard b;
    b.set(0, 0);
    EXPECT_EQ(uint64_t(b), 0x0000000000000001ULL);
    b.set(2, 5);
    EXPECT_EQ(uint64_t(b), 0x0000000000200001ULL);
    b.set(5, 2);
    EXPECT_EQ(uint64_t(b), 0x0000040000200001ULL);
    b.set(7, 7);
    EXPECT_EQ(uint64_t(b), 0x8000040000200001ULL);
}


TEST(BitBoardTest, Clear)
{
    BitBoard b(
        0xFFFFFFFFFFFFFFFFULL);
    b.clear(0, 0);
    EXPECT_EQ(uint64_t(b), 0xFFFFFFFFFFFFFFFEULL);
    b.clear(2, 5);
    EXPECT_EQ(uint64_t(b), 0xFFFFFFFFFFDFFFFEULL);
    b.clear(5, 2);
    EXPECT_EQ(uint64_t(b), 0xFFFFFBFFFFDFFFFEULL);
    b.clear(7, 7);
    EXPECT_EQ(uint64_t(b), 0x7FFFFBFFFFDFFFFEULL);
}
TEST(BitBoardTest, Flip)
{
    BitBoard b(0x123456789ABCDEF0ULL);
    b.flip();
    EXPECT_EQ(uint64_t(b), 0xF0DEBC9A78563412ULL);
}

TEST(BitBoardTest, Threatened)
{
    // Test threatened for a king at every square on the board
    for (int r = 0; r < BitBoard::SQUARES_PER_COLUMN; ++r)
    {
        for (int c = 0; c < BitBoard::SQUARES_PER_ROW; ++c)
        {
            BitBoard kingThreats = BitBoard::threatened(BitBoard::KING, r, c);
            for (int i = 0; i < BitBoard::SQUARES_PER_COLUMN; ++i)
            {
                for (int j = 0; j < BitBoard::SQUARES_PER_ROW; ++j)
                {
                    int expected = ((i == r - 1 || i == r + 1) && j >= c - 1 && j <= c + 1) ||
                        i == r && (j == c - 1 || j == c + 1)
                        ? 1 : 0;
                    EXPECT_EQ(kingThreats.test(i, j), expected);
                }
            }
        }
    }
}

TEST(BitBoardTest, Threatened_blockers)
{
    GTEST_SKIP() << "Skipping due to incomplete implmentation.";
#if 0
    BitBoard friends, foes;
    friends.set(4, 5); // Block E5
    foes.set(3, 4);    // Block D4
    BitBoard queenThreat = BitBoard::threatened(BitBoard::QUEEN, 4, 4, friends, foes);
    // Should not threaten blocked squares
    EXPECT_EQ(queenThreat.test(4, 5), 0);
    EXPECT_EQ(queenThreat.test(3, 4), 0);
#endif
}

TEST(BitBoardTest, Destinations)
{
    // Test destinations for a king at every square on the board
    for (int r = 0; r < BitBoard::SQUARES_PER_COLUMN; ++r)
    {
        for (int c = 0; c < BitBoard::SQUARES_PER_ROW; ++c)
        {
            BitBoard kingDestinations = BitBoard::destinations(BitBoard::KING, r, c);
            for (int i = 0; i < BitBoard::SQUARES_PER_COLUMN; ++i)
            {
                for (int j = 0; j < BitBoard::SQUARES_PER_ROW; ++j)
                {
                    int expected = ((i == r - 1 || i == r + 1) && j >= c - 1 && j <= c + 1) ||
                                   i == r && (j == c - 1 || j == c + 1)
                        ? 1 : 0;
                    EXPECT_EQ(kingDestinations.test(i, j), expected);
                }
            }
        }
    }
}

TEST(BitBoardTest, Destinations_blockers)
{
    GTEST_SKIP() << "Skipping due to incomplete implmentation.";
#if 0
    BitBoard friends, foes;
    friends.set(4, 5); // Block E5
    foes.set(3, 4);    // Block D4
    BitBoard queenDest = BitBoard::destinations(BitBoard::QUEEN, 4, 4, friends, foes);
    // Should not allow move to blocked squares
    EXPECT_EQ(queenDest.test(4, 5), 0);
    EXPECT_EQ(queenDest.test(3, 4), 0);
#endif
}
