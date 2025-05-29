#ifndef ATTACKS_HPP
#define ATTACKS_HPP

#include <array>
#include <cassert>
#include <random>
#include "bitboard.hpp"
#include "exceptions.hpp"
#include "utils.hpp"

namespace attacks {

/* @brief Precalculate attack tables for all pieces. Must be called once before
 *  being able to use the attack module. */
void precalculate(bool info = false);

/* @brief Get the attack bitboard representation for a given piece on a given
 *  square. The blocker configuration is only relevant for sliding pieces. For
 *  other piece types, it is automatically set zero.
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
