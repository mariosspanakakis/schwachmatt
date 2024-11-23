#ifndef BITBOARD_H
#define BITBOARD_H

#include <cassert>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "types.h"

namespace bb {

// print a readable bitboard representation to the console
void printBitboard(Bitboard bitboard);

// get a bitboard with the given squares set for testing purposes
Bitboard getPopulatedBitboard(const std::vector<Square>& populated_squares);

// get a bitboard with only the given square set to one
inline Bitboard square_bb(Square square) { return (1ULL << square); }

// bit manipulation
inline void setBit(Bitboard& bitboard, Square square){ bitboard |= square_bb(square); }
inline bool getBit(Bitboard& bitboard, Square square){ return ((bitboard >> square) & 1ULL); }
inline void clearBit(Bitboard& bitboard, Square square){ bitboard &= ~square_bb(square); }

/* Shifts a bitboard as specified by direction D. */
template <Direction D>
constexpr Bitboard shift(Bitboard b) {
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
         : 0;
}

// convert a rank-file coordinate to a square
inline Square get_square(int rank, int file) {
    return Square(8 * rank + file);
}

inline int popcount(Bitboard b) {
    // use built-in popcount instruction
    return __builtin_popcountll(b);

    // alternative, but much slower approach without using popcountll
    /*int r;
    for(r = 0; b; r++, b &= b - 1);
    return r;*/
}

/* Get the index of the least significant one bit of the given bitboard. */
inline Square lsb(Bitboard bitboard) {
    //assert(bitboard);
    return Square(__builtin_ctzll(bitboard));                                   // NOTE: calling __builtin_ctzll on an empty bitboard results in a segmentation fault
    //return popcount((bitboard & -bitboard) - 1);
}

/* Get and clear the least significant one bit of the given bitboard. */
inline Square pop_lsb(Bitboard& bitboard) {
    const Square square = lsb(bitboard);
    bitboard &= bitboard - 1;
    return square;
}

}   // namespace bb

#endif
