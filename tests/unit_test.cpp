#include <iostream>
#include <gtest/gtest.h>
#include "bitboard.h"
#include "attacks.h"


/* --- BITBOARD UNIT TESTS --- */
/* Set a bit on an empty bitboard and test for the result. */
TEST(BitboardTest, SetBit) {
    bb::U64 bitboard = 0ULL;
    bb::SetBit(bitboard, bb::E4);
    bb::U64 expected = 268435456;
    EXPECT_EQ(bitboard, expected);
}

/* Clear a bit on a bitboard and test for the result. */
TEST(BitboardTest, ClearBit) {
    bb::U64 bitboard = 268435456;
    bb::U64 expected = 0ULL;
    bb::ClearBit(bitboard, bb::E4);
    EXPECT_EQ(bitboard, expected);
}

/* --- ATTACK TABLE GENERATION UNIT TESTS --- */
/* Generate sliding piece attack tables and check for errors. */
TEST(AttackTest, MagicNumberGeneration) {
    attacks::InitializeAttackTables();

    for (int is_bishop = 0; is_bishop < 2; is_bishop++) {
        for (int square = 0; square < bb::N_SQUARES; square++) {

            // get attack mask and number of relevant bits
            bb::U64 attack_mask = is_bishop ? attacks::GetBishopAttacks(square) : attacks::GetRookAttacks(square);
            int bits = is_bishop ? attacks::bishop_relevant_bits[square] : attacks::rook_relevant_bits[square];

            // manually generate the attack mask for each blocker configuration
            for (int i = 0; i < (1 << bits); i++) {
                bb::U64 blockers = attacks::GetBlockerConfiguration(i, attack_mask);
                bb::U64 manual_attacks = is_bishop ? attacks::GetBishopAttacks(square, blockers) : attacks::GetRookAttacks(square, blockers);
                bb::U64 lookup_attacks = is_bishop ? attacks::LookupBishopAttacks(square, blockers) : attacks::LookupRookAttacks(square, blockers);

                ASSERT_EQ(manual_attacks, lookup_attacks);
            }
        }
    }
}

/* --- MOVE GENERATION UNIT TESTS --- */

/* --- MAIN TEST FUNCTION --- */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}