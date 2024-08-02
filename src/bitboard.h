#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "types.h"

namespace chess {
namespace bb {

// print a readable bitboard representation to the console
void printBitboard(Bitboard bitboard);

// get a bitboard with the given squares set for testing purposes
Bitboard getPopulatedBitboard(const std::vector<Square>& populatedSquares);

// bit manipulation
inline void setBit(Bitboard& bitboard, Square square){
    bitboard |= (1ULL << square);
}
inline bool getBit(Bitboard& bitboard, Square square){
    return ((bitboard >> square) & 1ULL) == 1;
}
inline void clearBit(Bitboard& bitboard, Square square){
    bitboard &= ~(1ULL << square);
}

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
inline int coordinateToSquare(int rank, int file) {
    return 8 * rank + file;
}

inline Bitboard squareToBitboard(Square square) {
    return 1ULL << square;
}

// other bit operations
int countBits(Bitboard bitboard);
int getLeastSignificantBitIndex(Bitboard bitboard);

}   // namespace bb
}   // namespace chess

#endif