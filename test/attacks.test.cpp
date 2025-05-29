#include <gtest/gtest.h>
#include "attacks.cpp"

class AttacksTest : public ::testing::Test {
protected:
    void SetUp() override {
        attacks::precalculate();
    }
};

TEST_F(AttacksTest, GenerateMagicNumbers) {
    for (int is_bishop = 0; is_bishop < 2; is_bishop++) {
        for (int square = 0; square < N_SQUARES; square++) {
            // get attack mask and number of relevant bits
            Bitboard attack_mask = is_bishop ? attacks::bishop_magics[square].mask : attacks::rook_magics[square].mask;
            int bits = is_bishop ? attacks::bishop_magics[square].mask_bits : attacks::rook_magics[square].mask_bits;

            // manually generate the attack mask for each blocker configuration
            for (int i = 0; i < (1 << bits); i++) {
                Bitboard blockers = attacks::getBlockerConfiguration(i, attack_mask);
                Bitboard manual_attacks = is_bishop ? attacks::calculateBishopAttacks(square, blockers) : attacks::calculateRookAttacks(square, blockers);
                Bitboard lookup_attacks = is_bishop ? attacks::lookupBishopAttacks(square, blockers) : attacks::lookupRookAttacks(square, blockers);

                EXPECT_EQ(manual_attacks, lookup_attacks);
            }
        }
    }
}
