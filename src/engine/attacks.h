#ifndef ATTACKS_H
#define ATTACKS_H

#include <cstring>
#include "bitboard.h"
#include "exceptions.h"

namespace attacks {

/* @brief Precalculate attacks for all pieces and all squares. This function
    is run once on the beginning of the program in order to speed up
    move generation by providing quick lookup tables. The initialization
    takes a moment which is due to the brute-force magic number
    generation. */
void initializeAttackTables(void);

/* @brief Get the attack bitboard representation for a given piece on a given
 *  square. The blocker configuration is only relevant for sliding pieces.
 * @tparam TPieceType The type of piece to generate attacks for.
 * @param square The square to generate the attacks from.
 * @returns A bitboard representation of all attacked squares. */
template<PieceType TPieceType>
Bitboard getPieceAttacks(Square square, Bitboard blockers);

/* @brief Get the attack bitboard representation for a given pawn on a given
 *  square.
 * @param square The square to generate the attacks from.
 * @param color The color/side for which to generate the attacks.
 * @returns A bitboard representation of all attacked squares. */
Bitboard getPawnAttacks(Square square, Color color);

}   // namespace attacks

#endif
