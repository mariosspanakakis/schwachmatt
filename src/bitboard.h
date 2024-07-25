#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "types.h"

namespace chess {
namespace bb {

// rank definitions
constexpr Bitboard RANK_1_BB = 0x00000000000000FFULL;
constexpr Bitboard RANK_2_BB = RANK_1_BB << 8;
constexpr Bitboard RANK_3_BB = RANK_1_BB << 16;
constexpr Bitboard RANK_4_BB = RANK_1_BB << 24;
constexpr Bitboard RANK_5_BB = RANK_1_BB << 32;
constexpr Bitboard RANK_6_BB = RANK_1_BB << 40;
constexpr Bitboard RANK_7_BB = RANK_1_BB << 48;
constexpr Bitboard RANK_8_BB = RANK_1_BB << 56;

// file definitions
constexpr Bitboard FILE_H_BB = 0x8080808080808080ULL;
constexpr Bitboard FILE_G_BB = FILE_H_BB >> 1;
constexpr Bitboard FILE_F_BB = FILE_H_BB >> 2;
constexpr Bitboard FILE_E_BB = FILE_H_BB >> 3;
constexpr Bitboard FILE_D_BB = FILE_H_BB >> 4;
constexpr Bitboard FILE_C_BB = FILE_H_BB >> 5;
constexpr Bitboard FILE_B_BB = FILE_H_BB >> 6;
constexpr Bitboard FILE_A_BB = FILE_H_BB >> 7;

// other relevant bitboard representations
constexpr Bitboard EDGE_BB = 0xFF818181818181FF;
constexpr Bitboard WHITE_KINGSIDE_CASTLE_SQUARES = 0x0000000000000060;
constexpr Bitboard WHITE_QUEENSIDE_CASTLE_SQUARES = 0x000000000000000E;
constexpr Bitboard BLACK_KINGSIDE_CASTLE_SQUARES = 0x6000000000000000;
constexpr Bitboard BLACK_QUEENSIDE_CASTLE_SQUARES = 0x0E00000000000000;

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