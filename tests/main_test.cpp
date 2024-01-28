#include <gtest/gtest.h>
#include "bitboard.hpp"


TEST(BitboardTest, set_bit) {
    bb::U64 bitboard = 0ULL;
    bb::set_bit(bitboard, bb::E4);
    bb::U64 expected = 268435456;
    EXPECT_EQ(bitboard, expected);
}

TEST(BitboardTest, clear_bit) {
    bb::U64 bitboard = 268435456;
    bb::U64 expected = 0ULL;
    bb::clear_bit(bitboard, bb::E4);
    EXPECT_EQ(bitboard, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}