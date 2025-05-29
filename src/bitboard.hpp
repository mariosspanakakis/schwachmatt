#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cassert>
#include <stdint.h>
#include <iostream>
#include <vector>
#include "types.hpp"

namespace bb {

inline Bitboard getBitboard(Square square) {
    return (1ULL << square);
}

Bitboard getBitboard(const std::vector<Square>& squares);

inline void set(Bitboard& b, Square square) {
    b |= getBitboard(square);
}

inline bool get(Bitboard& b, Square square) {
    return ((b >> square) & 1ULL);
}

inline void clear(Bitboard& b, Square square) {
    b &= ~getBitboard(square);
}

inline int count(Bitboard b) {
#ifdef __GNUC__
    return __builtin_popcountll(b);
#else
    // fallback implementation, much slower
    int r = 0;
    for (; b; r++, b &= b - 1);
    return r;
#endif
}

inline Square getLSB(Bitboard b) {
    #ifdef __GNUC__
    assert(b);
    return Square(__builtin_ctzll(b));
#else
    assert(b);
    return Square(count((b & -b) - 1));
#endif
}

inline Square popLSB(Bitboard& b) {
    const Square square = getLSB(b);
    b &= b - 1;
    return square;
}

template <Direction D>
inline constexpr Bitboard shift(Bitboard b) {
    return D == NORTH         ? b << 8
         : D == SOUTH         ? b >> 8
         : D == NORTH + NORTH ? b << 16
         : D == SOUTH + SOUTH ? b >> 16
         : D == EAST          ? (b & ~FILE_H_BB) << 1
         : D == WEST          ? (b & ~FILE_A_BB) >> 1
         : D == NORTHEAST     ? (b & ~FILE_H_BB) << 9
         : D == NORTHWEST     ? (b & ~FILE_A_BB) << 7
         : D == SOUTHEAST     ? (b & ~FILE_H_BB) >> 7
         : D == SOUTHWEST     ? (b & ~FILE_A_BB) >> 9
         : 0ULL;
}

void print(Bitboard b);

} // namespace bb

#endif
