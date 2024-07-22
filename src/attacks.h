#ifndef ATTACKS_H
#define ATTACKS_H

#include <cstring>
#include "bitboard.h"
#include "utils.h"

namespace attacks {

    // attack lookup tables
    extern bb::U64 pawnAttackTable[bb::N_COLORS][bb::N_SQUARES];
    extern bb::U64 knightAttackTable[bb::N_SQUARES];
    extern bb::U64 bishopAttackTable[bb::N_SQUARES][4096];
    extern bb::U64 rookAttackTable[bb::N_SQUARES][4096];
    extern bb::U64 kingAttackTable[bb::N_SQUARES];

    // attack masks for rook and bishop (ignore blockers)
    extern bb::U64 bishopAttackMask[bb::N_SQUARES];
    extern bb::U64 rookAttackMask[bb::N_SQUARES];

    // magic numbers
    extern bb::U64 bishopMagics[bb::N_SQUARES];
    extern bb::U64 rookMagics[bb::N_SQUARES];

    // relevant bishop bits (i.e. number of attacked squares) for each square
    const int bishopRelevantBits[64] = {
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
    const int rookRelevantBits[64] = {
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
    void initializeAttackTables(void);

    // calculate attacks - slow and only meant to be used during initialization
    bb::U64 calculatePawnAttacks(bb::Square square, bb::Color color);
    bb::U64 calculateKnightAttacks(bb::Square square);
    bb::U64 calculateKingAttacks(bb::Square square);
    bb::U64 calculateBishopAttacks(bb::Square square, bb::U64 blockers = 0ULL, bool mask_mode = false);   // mask mode: used for magics initialization which require to ignore board edges
    bb::U64 calculateRookAttacks(bb::Square square, bb::U64 blockers = 0ULL, bool mask_mode = false);

    // lookup the sliding attacks for given blockers using the attack table     // <- these are the ones to actually use during move generation
    bb::U64 lookupBishopAttacks(bb::Square square, bb::U64 blockers = 0ULL);
    bb::U64 lookupRookAttacks(bb::Square square, bb::U64 blockers = 0ULL);

    /* --- MAGIC NUMBERS --- */

    /* @brief Get a possible blocker configuration for a given attack mask. Used
     *        during magic number generation in order to loop through all
     *        possible blocker configurations.
     * @param index Index of the blocker configuration to return. Must be in the
     *              range of possible indices (1 << n_bits).
     * @param attack_mask Bitboard representing all attacked squares by a piece.
     * @return Bitboard representing the blocker configuration for the given
     *         attack mask. */
    bb::U64 getBlockerConfiguration(int index, bb::U64 attack_mask);

    /* @brief Perform brute-force random search to find a suitable magic number
     *        for sliding piece attack generation from a given square.
     * @param square The square for which to calculate the magic number.
     * @param is_bishop Boolean indicating whether to calculate the attacks for
     *        a bishop or for a rook.
     * @return A U64 magic number that surjectively maps all possible blocker
     *         configurations to an index in the attack table. */
    bb::U64 findMagicNumber(bb::Square square, bool is_bishop);                 // TODO: add error handling
    
    /* @brief Perform magic transform, i.e. multiply the masked blockers with a
     *        given magic number and then bitshift to discard non-relevant bits.
     * @param masked_blockers Bitboard containing the relevant blockers for the
     *                        attack generation.
     * @param magic Magic number that maps the blocker configuration to a unique
     *              index.
     * @param bits Number of relevant bits, i.e. maximum number of attacked
     *               squares for the given piece and square.
     * @return A transformed results that is used as an index for attack lookup. */
    bb::U64 magicTransform(bb::U64 masked_blockers, bb::U64 magic, int bits);

    /* @brief Initialize the magic attack tables for a given square. Will search
     *        for a magic number, calculate the attack map for every possible
     *        blocker configuration, and then store the map in the attack table.
     * @param square The square for which to calculate and store the attacks.
     * @param is_bishop Boolean indicating whether to calculate the attacks for
     *        a bishop or for a rook. */
    void initializeMagicAttack(bb::Square square, bool is_bishop);
}

#endif