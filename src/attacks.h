#ifndef ATTACKS_H
#define ATTACKS_H

#include <cstring>
#include "bitboard.h"
#include "exceptions.h"
#include "utils.h"

namespace chess {
namespace attacks {

// attack lookup tables
extern Bitboard PAWN_ATTACKS[N_COLORS][N_SQUARES];
extern Bitboard KNIGHT_ATTACKS[N_SQUARES];
extern Bitboard BISHOP_ATTACKS[N_SQUARES][4096];
extern Bitboard ROOK_ATTACKS[N_SQUARES][4096];
extern Bitboard KING_ATTACKS[N_SQUARES];

// attack masks for rook and bishop
extern Bitboard BISHOP_MASK[N_SQUARES];
extern Bitboard ROOK_MASK[N_SQUARES];

// magic numbers
extern Bitboard BISHOP_MAGICS[N_SQUARES];
extern Bitboard ROOK_MAGICS[N_SQUARES];

// relevant bishop bits (i.e. number of attacked squares) for each square
const int BISHOP_BITS[64] = {
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
const int ROOK_BITS[64] = {
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
Bitboard calculatePawnAttacks(Square square, Color color);
Bitboard calculateKnightAttacks(Square square);
Bitboard calculateKingAttacks(Square square);
Bitboard calculateBishopAttacks(Square square, Bitboard blockers = 0ULL, bool mask_mode = false);   // mask mode: used for magics initialization which require to ignore board edges
Bitboard calculateRookAttacks(Square square, Bitboard blockers = 0ULL, bool mask_mode = false);

// lookup the sliding attacks for given blockers using the attack table         // <- these are the ones to actually use during move generation
Bitboard lookupBishopAttacks(Square square, Bitboard blockers = 0ULL);
Bitboard lookupRookAttacks(Square square, Bitboard blockers = 0ULL);

/* --- MAGIC NUMBERS --- */

/* @brief Get a possible blocker configuration for a given attack mask. Used
    *        during magic number generation in order to loop through all
    *        possible blocker configurations.
    * @param index Index of the blocker configuration to return. Must be in the
    *              range of possible indices (1 << n_bits).
    * @param attack_mask Bitboard representing all attacked squares by a piece.
    * @return Bitboard representing the blocker configuration for the given
    *         attack mask. */
Bitboard getBlockerConfiguration(int index, Bitboard attack_mask);

/* @brief Perform brute-force random search to find a suitable magic number
    *        for sliding piece attack generation from a given square.
    * @param square The square for which to calculate the magic number.
    * @param is_bishop Boolean indicating whether to calculate the attacks for
    *        a bishop or for a rook.
    * @return A Bitboard magic number that surjectively maps all possible blocker
    *         configurations to an index in the attack table. */
Bitboard findMagicNumber(Square square, bool is_bishop);

/* @brief Perform magic transform, i.e. multiply the masked blockers with a
    *        given magic number and then bitshift to discard non-relevant bits.
    * @param masked_blockers Bitboard containing the relevant blockers for the
    *                        attack generation.
    * @param magic Magic number that maps the blocker configuration to a unique
    *              index.
    * @param bits Number of relevant bits, i.e. maximum number of attacked
    *               squares for the given piece and square.
    * @return A transformed results that is used as an index for attack lookup. */
Bitboard magicTransform(Bitboard masked_blockers, Bitboard magic, int bits);

/* @brief Initialize the magic attack tables for a given square. Will search
    *        for a magic number, calculate the attack map for every possible
    *        blocker configuration, and then store the map in the attack table.
    * @param square The square for which to calculate and store the attacks.
    * @param is_bishop Boolean indicating whether to calculate the attacks for
    *        a bishop or for a rook. */
void initializeMagicAttack(Square square, bool is_bishop);

}   // namespace attacks
}   // namespace chess

#endif