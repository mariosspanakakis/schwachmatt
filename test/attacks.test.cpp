#include <catch2/catch_test_macros.hpp>
#include "attacks.cpp"

TEST_CASE("Attacks: generate magic numbers") {
    attacks::initializeAttackTables();

    for (int is_bishop = 0; is_bishop < 2; is_bishop++) {
        for (int square = 0; square < N_SQUARES; square++) {
            // get attack mask and number of relevant bits
            Bitboard attack_mask = is_bishop ? attacks::BISHOP_MASK[square] : attacks::ROOK_MASK[square];
            int bits = is_bishop ? attacks::BISHOP_BITS[square] : attacks::ROOK_BITS[square];

            // manually generate the attack mask for each blocker configuration
            for (int i = 0; i < (1 << bits); i++) {
                Bitboard blockers = attacks::getBlockerConfiguration(i, attack_mask);
                Bitboard manual_attacks = is_bishop ? attacks::calculateBishopAttacks(square, blockers) : attacks::calculateRookAttacks(square, blockers);
                Bitboard lookup_attacks = is_bishop ? attacks::lookupBishopAttacks(square, blockers) : attacks::lookupRookAttacks(square, blockers);

                REQUIRE(manual_attacks == lookup_attacks);
            }
        }
    }
}