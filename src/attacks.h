#ifndef ATTACKS_H
#define ATTACKS_H

#include <cstring>
#include "bitboard.h"
#include "utils.h"

namespace attacks {

    // initialize all attack tables for rapid lookup during move generation
    void InitializeAttackTables(void);

    // get attacks for leaping pieces
    bb::U64 GetPawnAttacks(bb::Square square, bb::Color color);
    bb::U64 GetKnightAttacks(bb::Square square);
    bb::U64 GetKingAttacks(bb::Square square);

    // get attacks for sliding piece attack generation
    bb::U64 GetBishopAttacks(bb::Square square, bb::U64 blockers = 0ULL);
    bb::U64 GetRookAttacks(bb::Square square, bb::U64 blockers = 0ULL);

    /* @brief Perform brute-force random search to find a suitable magic number
     *        for sliding piece attack generation from a given square.
     * @param square The square for which to calculate the magic number.
     * @param is_bishop Boolean indicating whether to calculate the attacks for
     *        a bishop or for a rook.
     * @return A U64 magic number that surjectively maps all possible blocker
     *         configurations to an index in the attack table. */
    bb::U64 FindMagicNumber(bb::Square square, bool is_bishop);                 // TODO: add error handling

    // multiply the blockers configuration, then drop all but the relevant bits
    inline bb::U64 MagicTransform(bb::U64 blockers, bb::U64 magic, int n_bits) {
        return (blockers * magic) >> (64 - n_bits);
    }

    /* @brief Get a possible blocker configuration for a given attack mask. Used
     *        during magic number generation in order to loop through all
     *        possible blocker configurations.
     * @param index Index of the blocker configuration to return. Must be in the
     *              range of possible indices (1 << n_bits).
     * @param attack_mask Bitboard representing all attacked squares by a piece.
     * @return Bitboard representing the blocker configuration for the given
     *         attack mask.
     */
    bb::U64 GetBlockerConfiguration(int index, bb::U64 attack_mask);            // TODO: add validity checks

    // extern variables to hold attack tables
    extern bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    extern bb::U64 knight_attack_table[bb::N_SQUARES];
    extern bb::U64 king_attack_table[bb::N_SQUARES];
}

#endif