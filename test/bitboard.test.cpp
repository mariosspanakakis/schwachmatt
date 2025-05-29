#include <gtest/gtest.h>
#include "bitboard.hpp"
#include "types.hpp"

TEST(BitboardTest, SetAndGetBits) {
    for (int square = 0; square < N_SQUARES; square++) {
        Bitboard b = 0ULL;
        bb::set(b, square);
        for (int p = 0; p < N_SQUARES; p++) {
            if (p == square) {
                EXPECT_TRUE(bb::get(b, p)) << "Bit not set for square: " << p;
            } else {
                EXPECT_FALSE(bb::get(b, p)) << "Bit unexpectedly set for square: " << p;
            }
        }
    }
}

TEST(BitboardTest, ClearBits) {
    for (int square = 0; square < N_SQUARES; square++) {
        Bitboard b = ~0ULL;
        bb::clear(b, square);
        for (int p = 0; p < N_SQUARES; p++) {
            if (p == square) {
                EXPECT_FALSE(bb::get(b, p)) << "Bit not cleared for square: " << p;
            } else {
                EXPECT_TRUE(bb::get(b, p)) << "Bit unexpectedly cleared for square: " << p;
            }
        }
    }
}

TEST(BitboardTest, ShiftBitboard) {
    Bitboard b;

    b = RANK_1_BB;
    EXPECT_EQ(bb::shift<NORTH>(b), RANK_2_BB) << "Shifted bitboard does not match expected value.";
    EXPECT_EQ(bb::shift<NORTH + NORTH>(b), RANK_3_BB) << "Shifted bitboard does not match expected value.";
    EXPECT_EQ(bb::shift<SOUTH>(b), 0ULL) << "Shifted bitboard does not match expected value.";

    b = RANK_8_BB;
    EXPECT_EQ(bb::shift<SOUTH>(b), RANK_7_BB) << "Shifted bitboard does not match expected value.";
    EXPECT_EQ(bb::shift<SOUTH + SOUTH>(b), RANK_6_BB) << "Shifted bitboard does not match expected value.";
    EXPECT_EQ(bb::shift<NORTH>(b), 0ULL) << "Shifted bitboard does not match expected value.";

    b = FILE_A_BB;
    EXPECT_EQ(bb::shift<EAST>(b), FILE_B_BB) << "Shifted bitboard does not match expected value.";
    EXPECT_EQ(bb::shift<WEST>(b), 0ULL) << "Shifted bitboard does not match expected value.";

    b = FILE_H_BB;
    EXPECT_EQ(bb::shift<EAST>(b), 0ULL) << "Shifted bitboard does not match expected value.";
    EXPECT_EQ(bb::shift<WEST>(b), FILE_G_BB) << "Shifted bitboard does not match expected value.";

    for (Square square = 0; square < N_SQUARES; square++) {
        b = bb::getBitboard(square);

        if (b & RANK_8_BB) {
            EXPECT_EQ(bb::shift<NORTH>(b), 0ULL) << "North shift failed for square: " << square;
        } else {
            EXPECT_EQ(bb::shift<NORTH>(b), bb::getBitboard(square + 8))
                << "North shift failed for square: " << square;
        }

        if (b & RANK_1_BB) {
            EXPECT_EQ(bb::shift<SOUTH>(b), 0ULL) << "South shift failed for square: " << square;
        } else {
            EXPECT_EQ(bb::shift<SOUTH>(b), bb::getBitboard(square - 8))
                << "South shift failed for square: " << square;
        }

        if (b & FILE_H_BB) {
            EXPECT_EQ(bb::shift<EAST>(b), 0ULL) << "East shift failed for square: " << square;
        } else {
            EXPECT_EQ(bb::shift<EAST>(b), bb::getBitboard(square + 1))
                << "East shift failed for square: " << square;
        }

        if (b & FILE_A_BB) {
            EXPECT_EQ(bb::shift<WEST>(b), 0ULL) << "West shift failed for square: " << square;
        } else {
            EXPECT_EQ(bb::shift<WEST>(b), bb::getBitboard(square - 1))
                << "West shift failed for square: " << square;
        }

        if (b & (RANK_8_BB | FILE_H_BB)) {
            EXPECT_EQ(bb::shift<NORTHEAST>(b), 0ULL) << "Northeast shift failed for square: " << square;
        } else {
            EXPECT_EQ(bb::shift<NORTHEAST>(b), bb::getBitboard(square + 9))
                << "Northeast shift failed for square: " << square;
        }

        if (b & (RANK_8_BB | FILE_A_BB)) {
            EXPECT_EQ(bb::shift<NORTHWEST>(b), 0ULL) << "Northwest shift failed for square: " << square;
        } else {
            EXPECT_EQ(bb::shift<NORTHWEST>(b), bb::getBitboard(square + 7))
                << "Northwest shift failed for square: " << square;
        }

        if (b & (RANK_1_BB | FILE_H_BB)) {
            EXPECT_EQ(bb::shift<SOUTHEAST>(b), 0ULL) << "Southeast shift failed for square: " << square;
        } else {
            EXPECT_EQ(bb::shift<SOUTHEAST>(b), bb::getBitboard(square - 7))
                << "Southeast shift failed for square: " << square;
        }

        if (b & (RANK_1_BB | FILE_A_BB)) {
            EXPECT_EQ(bb::shift<SOUTHWEST>(b), 0ULL) << "Southwest shift failed for square: " << square;
        } else {
            EXPECT_EQ(bb::shift<SOUTHWEST>(b), bb::getBitboard(square - 9))
                << "Southwest shift failed for square: " << square;
        }
    }
}

TEST(BitboardTest, CountBits) {
    Bitboard b = 0ULL;
    EXPECT_EQ(bb::count(b), 0) << "Count of empty bitboard should be 0.";
    for (Square square = 0; square < N_SQUARES; square++) {
        bb::set(b, square);
        EXPECT_EQ(bb::count(b), square + 1) << "Count of bitboard should be " << (square + 1) << " after setting square: " << square;
    }
    for (Square square = 0; square < N_SQUARES; square++) {
        bb::clear(b, square);
        EXPECT_EQ(bb::count(b), N_SQUARES - square - 1) << "Count of bitboard should be " << (N_SQUARES - square - 1) << " after clearing square: " << square;
    }
}

TEST(BitboardTest, GetLSB) {
    Bitboard b = 0ULL;
    for (Square square = 0; square < N_SQUARES; square++) {
        bb::set(b, square);
        EXPECT_EQ(bb::getLSB(b), square) << "Failed to get LSB for square: " << square;
        bb::clear(b, square);
    }
}

TEST(BitboardTest, PopLSB) {
    Bitboard b = ~0ULL;
    for (Square square = 0; square < N_SQUARES; square++) {
        EXPECT_EQ(bb::popLSB(b), square) << "Failed to pop LSB for square: " << square;
        EXPECT_FALSE(bb::get(b, square)) << "Bit not cleared after popping LSB for square: " << square;
    }

    EXPECT_EQ(b, 0ULL) << "Bitboard should be empty after all bits are popped.";
}