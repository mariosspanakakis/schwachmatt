#include <gtest/gtest.h>
#include "bitboard.cpp"

TEST(BitboardTest, SettingBits) {
    Bitboard bitboard = 0ULL;
    bb::setBit(bitboard, E4);
    Bitboard expected = 268435456;
    EXPECT_EQ(bitboard, expected);
}

TEST(BitboardTest, ClearingBits) {
    Bitboard bitboard = 268435456;
    Bitboard expected = 0ULL;
    bb::clearBit(bitboard, E4);
    EXPECT_EQ(bitboard, expected);
}
