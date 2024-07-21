#ifndef ATTACKS_H
#define ATTACKS_H

#include <cstring>
#include "bitboard.h"
#include "utils.h"

namespace attacks {

    // attack lookup tables
    extern bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    extern bb::U64 knight_attack_table[bb::N_SQUARES];
    extern bb::U64 king_attack_table[bb::N_SQUARES];
    extern bb::U64 bishop_attack_table[bb::N_SQUARES][4096];
    extern bb::U64 rook_attack_table[bb::N_SQUARES][4096];

    // attack masks for rook and bishop (ignore blockers)
    extern bb::U64 bishop_attack_mask[bb::N_SQUARES];
    extern bb::U64 rook_attack_mask[bb::N_SQUARES];

    // magic numbers
    extern bb::U64 bishop_magics[bb::N_SQUARES];
    extern bb::U64 rook_magics[bb::N_SQUARES];

    // relevant bishop bits (i.e. number of attacked squares) for each square
    const int bishop_relevant_bits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        5, 5, 7, 7, 7, 7, 5, 5, 
        5, 5, 7, 9, 9, 7, 5, 5, 
        5, 5, 7, 9, 9, 7, 5, 5, 
        5, 5, 7, 7, 7, 7, 5, 5, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        6, 5, 5, 5, 5, 5, 5, 6
    };

    // relevant rook bits (i.e. number of attacked squares) for each square
    const int rook_relevant_bits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        12, 11, 11, 11, 11, 11, 11, 12
    };

    /* --- ATTACK GENERATION --- */

    // initialize all attack tables for rapid lookup during move generation
    void InitializeAttackTables(void);

    // get attacks for leaping pieces
    bb::U64 GetPawnAttacks(bb::Square square, bb::Color color);
    bb::U64 GetKnightAttacks(bb::Square square);
    bb::U64 GetKingAttacks(bb::Square square);

    // get attacks for sliding pieces
    bb::U64 GetBishopAttacks(bb::Square square, bb::U64 blockers = 0ULL, bool mask_mode = false);   // mask mode: used for magics initialization which require to ignore board edges
    bb::U64 GetRookAttacks(bb::Square square, bb::U64 blockers = 0ULL, bool mask_mode = false);

    // lookup the sliding attacks for given blockers using the attack table     // <- these are the ones to actually use during move generation
    bb::U64 LookupBishopAttacks(bb::Square square, bb::U64 blockers = 0ULL);
    bb::U64 LookupRookAttacks(bb::Square square, bb::U64 blockers = 0ULL);

    /* --- MAGIC NUMBERS --- */

    /* @brief Get a possible blocker configuration for a given attack mask. Used
     *        during magic number generation in order to loop through all
     *        possible blocker configurations.
     * @param index Index of the blocker configuration to return. Must be in the
     *              range of possible indices (1 << n_bits).
     * @param attack_mask Bitboard representing all attacked squares by a piece.
     * @return Bitboard representing the blocker configuration for the given
     *         attack mask. */
    bb::U64 GetBlockerConfiguration(int index, bb::U64 attack_mask);

    /* @brief Perform brute-force random search to find a suitable magic number
     *        for sliding piece attack generation from a given square.
     * @param square The square for which to calculate the magic number.
     * @param is_bishop Boolean indicating whether to calculate the attacks for
     *        a bishop or for a rook.
     * @return A U64 magic number that surjectively maps all possible blocker
     *         configurations to an index in the attack table. */
    bb::U64 FindMagicNumber(bb::Square square, bool is_bishop);                 // TODO: add error handling
    
    /* @brief Perform magic transform, i.e. multiply the masked blockers with a
     *        given magic number and then bitshift to discard non-relevant bits.
     * @param masked_blockers Bitboard containing the relevant blockers for the
     *                        attack generation.
     * @param magic Magic number that maps the blocker configuration to a unique
     *              index.
     * @param bits Number of relevant bits, i.e. maximum number of attacked
     *               squares for the given piece and square.
     * @return A transformed results that is used as an index for attack lookup. */
    bb::U64 MagicTransform(bb::U64 masked_blockers, bb::U64 magic, int bits);

    /* @brief Initialize the magic attack tables for a given square. Will search
     *        for a magic number, calculate the attack map for every possible
     *        blocker configuration, and then store the map in the attack table.
     * @param square The square for which to calculate and store the attacks.
     * @param is_bishop Boolean indicating whether to calculate the attacks for
     *        a bishop or for a rook. */
    void InitializeMagicAttack(bb::Square square, bool is_bishop);
}

#endif