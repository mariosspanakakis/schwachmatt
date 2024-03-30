#ifndef ATTACKS_H
#define ATTACKS_H

#include <cstring>
#include "bitboard.h"
#include "utils.h"

namespace attacks {

    // extern variables to hold attack tables
    extern bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    extern bb::U64 knight_attack_table[bb::N_SQUARES];
    extern bb::U64 king_attack_table[bb::N_SQUARES];
    extern bb::U64 bishop_attack_table[bb::N_SQUARES][4096];
    extern bb::U64 rook_attack_table[bb::N_SQUARES][4096];

    /* --- ATTACK GENERATION --- */

    // initialize all attack tables for rapid lookup during move generation
    void InitializeAttackTables(void);

    // get attacks for leaping pieces
    bb::U64 GetPawnAttacks(bb::Square square, bb::Color color);
    bb::U64 GetKnightAttacks(bb::Square square);
    bb::U64 GetKingAttacks(bb::Square square);

    // get attacks for sliding pieces
    bb::U64 GetBishopAttacks(bb::Square square, bb::U64 blockers = 0ULL);
    bb::U64 GetRookAttacks(bb::Square square, bb::U64 blockers = 0ULL);

    /* --- MAGIC NUMBERS --- */
    // TODO: it makes sense to precalculate the number of relevant bits, since it is also required for the lookup at runtime!

    /* @brief Get a possible blocker configuration for a given attack mask. Used
     *        during magic number generation in order to loop through all
     *        possible blocker configurations.
     * @param index Index of the blocker configuration to return. Must be in the
     *              range of possible indices (1 << n_bits).
     * @param attack_mask Bitboard representing all attacked squares by a piece.
     * @return Bitboard representing the blocker configuration for the given
     *         attack mask. */
    bb::U64 GetBlockerConfiguration(int index, bb::U64 attack_mask);            // TODO: add validity checks

    /* @brief Perform brute-force random search to find a suitable magic number
     *        for sliding piece attack generation from a given square.
     * @param square The square for which to calculate the magic number.
     * @param is_bishop Boolean indicating whether to calculate the attacks for
     *        a bishop or for a rook.
     * @return A U64 magic number that surjectively maps all possible blocker
     *         configurations to an index in the attack table. */
    bb::U64 FindMagicNumber(bb::Square square, bool is_bishop);                 // TODO: add error handling
    
    // multiply the blockers configuration, then drop all but the relevant bits
    /* @brief Perform magic transform, i.e. multiply the masked blockers with a
     *        given magic number and then bitshift to discard non-relevant bits.
     * @param masked_blockers Bitboard containing the relevant blockers for the
     *                        attack generation.
     * @param magic Magic number that maps the blocker configuration to a unique
     *              index.
     * @param n_bits Number of relevant bits, i.e. maximum number of attacked
     *               squares for the given piece and square.
     * @return A transformed results that is used as an index for attack lookup. */
    inline bb::U64 MagicTransform(bb::U64 masked_blockers, bb::U64 magic, int n_bits) {
        return (masked_blockers * magic) >> (64 - n_bits);
    }
}

#endif