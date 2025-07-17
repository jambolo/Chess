#include "gtest/gtest.h"
#include "ZHash/ZHash.h"
#include "Chess/Board.h"
#include "Chess/Piece.h"
#include "Chess/Position.h"
#include "Chess/Types.h"

TEST(ZHashTest, Constructor)
{
    ZHash hash_default;
    EXPECT_EQ(hash_default.value(), ZHash::EMPTY);
    EXPECT_FALSE(hash_default.isUndefined());

    ZHash hash_value(12345);
    EXPECT_EQ(hash_value.value(), 12345);
}

TEST(ZHashTest, Constructor_board)
{
    // I don't have a good way to test this because the values are random. Just test that it is not empty.
    Board board;
    board.initialize();
    ZHash hash(board, Color::WHITE);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
}

TEST(ZHashTest, Add_Remove)
{
    ZHash hash;
    Piece const * whiteQueen = Piece::get(PieceTypeId::QUEEN, Color::WHITE);

    // Add the queen to every position
    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            auto v1 = hash.value();
            hash.add(whiteQueen, Position(r, c));
            EXPECT_NE(hash.value(), v1);
            EXPECT_NE(hash.value(), ZHash::EMPTY);
        }
    }

    // Remove the queen from every position
    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            auto v1 = hash.value();
            hash.remove(whiteQueen, Position(r, c));
            EXPECT_NE(hash.value(), v1);
        }
    }
    EXPECT_EQ(hash.value(), ZHash::EMPTY);
}

TEST(ZHashTest, Move)
{
    ZHash hash;
    Piece const * blackRook = Piece::get(PieceTypeId::ROOK, Color::BLACK);
    Position from(0, 0);
    Position to(0, 7);
    hash.add(blackRook, from);
    auto v1 = hash.value();
    hash.move(blackRook, from, to);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
}

TEST(ZHashTest, Turn)
{
    Board board;
    board.initialize();
    ZHash hash(board, Color::WHITE);
    auto v1 = hash.value();
    hash.turn();
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    hash.turn();
    EXPECT_EQ(hash.value(), v1);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
}

TEST(ZHashTest, CastleAvailability)
{
    Board board;
    board.initialize();
    ZHash hash(board, Color::WHITE);
    auto v1 = hash.value();
    hash.castleAvailability(WHITE_QUEENSIDE_CASTLE_UNAVAILABLE);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    auto v2 = hash.value();
    hash.castleAvailability(WHITE_KINGSIDE_CASTLE_UNAVAILABLE);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), v2);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    auto v3 = hash.value();
    hash.castleAvailability(BLACK_QUEENSIDE_CASTLE_UNAVAILABLE);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), v3);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    auto v4 = hash.value();
    hash.castleAvailability(BLACK_KINGSIDE_CASTLE_UNAVAILABLE);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), v4);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    auto v5 = hash.value();
    hash.castleAvailability(WHITE_QUEENSIDE_CASTLE_UNAVAILABLE);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), v5);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    auto v6 = hash.value();
    hash.castleAvailability(WHITE_KINGSIDE_CASTLE_UNAVAILABLE);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), v6);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    auto v7 = hash.value();
    hash.castleAvailability(BLACK_QUEENSIDE_CASTLE_UNAVAILABLE);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), v7);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    hash.castleAvailability(BLACK_KINGSIDE_CASTLE_UNAVAILABLE);
    EXPECT_EQ(hash.value(), v1);
}

TEST(ZHashTest, EnPassant)
{
    Board board;
    board.initialize();
    ZHash hash(board, Color::WHITE);
    auto v1 = hash.value();
    hash.enPassant(Color::WHITE, 3);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    auto v2 = hash.value();
    hash.enPassant(Color::BLACK, 4);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), v2);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    auto v3 = hash.value();
    hash.enPassant(Color::WHITE, 3);
    EXPECT_NE(hash.value(), v1);
    EXPECT_NE(hash.value(), v3);
    EXPECT_NE(hash.value(), ZHash::EMPTY);
    hash.enPassant(Color::BLACK, 4);
    EXPECT_EQ(hash.value(), v1);
}

TEST(ZHashTest, Fifty)
{
    Board board;
    board.initialize();
    ZHash hash(board, Color::WHITE);
    auto v1 = hash.value();
    hash.fifty();
    EXPECT_NE(hash.value(), v1);
    auto v2 = hash.value();
    hash.fifty();
    EXPECT_EQ(hash.value(), v1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
