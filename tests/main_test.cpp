#include <gtest/gtest.h>
#include "bitboard.h"


TEST(BitboardTest, SetBit) {
    bb::U64 bitboard = 0ULL;
    bb::SetBit(bitboard, bb::E4);
    bb::U64 expected = 268435456;
    EXPECT_EQ(bitboard, expected);
}

TEST(BitboardTest, ClearBit) {
    bb::U64 bitboard = 268435456;
    bb::U64 expected = 0ULL;
    bb::ClearBit(bitboard, bb::E4);
    EXPECT_EQ(bitboard, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}