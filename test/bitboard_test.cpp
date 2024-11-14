#include <gtest/gtest.h>
#include "bitboard.cpp"

TEST(BitboardTest, SetBit) {
    chess::Bitboard bitboard = 0ULL;
    chess::bb::setBit(bitboard, chess::E4);
    chess::Bitboard expected = 268435456;
    EXPECT_EQ(bitboard, expected);
}

TEST(BitboardTest, ClearBit) {
    chess::Bitboard bitboard = 268435456;
    chess::Bitboard expected = 0ULL;
    chess::bb::clearBit(bitboard, chess::E4);
    EXPECT_EQ(bitboard, expected);
}