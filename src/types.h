#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace chess {

using Bitboard = uint64_t;
using Square = uint8_t;
using Piece = uint8_t;
using Direction = int8_t;
using Color = bool;
using CastlingRight = uint8_t;
using MoveFlag = uint8_t;

const int MAX_NUMBER_OF_MOVES = 256;            // would actually be 218

enum Squares {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
};
const int N_SQUARES = 64;

constexpr char const* SQUARE_NAMES[] {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

enum Colors {
    WHITE,
    BLACK
};
const int N_COLORS = 2;

enum Pieces {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};
const int N_PIECES = 6;

enum Directions {
    NORTH = 8,
    SOUTH = -8,
    EAST = 1,
    WEST = -1,
    NORTHEAST = 9,
    NORTHWEST = 7,
    SOUTHEAST = -7,
    SOUTHWEST = -9
};
const int N_DIRECTIONS = 8;

enum Ranks {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};
enum Files {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};

enum CastlingRights {
    WHITE_KINGSIDE_CASTLE   = 0b0001,
    WHITE_QUEENSIDE_CASTLE  = 0b0010,
    BLACK_KINGSIDE_CASTLE   = 0b0100,
    BLACK_QUEENSIDE_CASTLE  = 0b1000
};

enum MoveShifts {
    FROM_SHIFT = 0,
    TO_SHIFT = 6,
    FLAG_SHIFT = 12
};

enum MoveMasks {
    TO_MASK = 0x3f,     // 0000 000000 111111
    FROM_MASK = 0xfc0   // 0000 111111 000000
};

// move type flag encoding (promotion - capture - special 1 - special 2)
enum MoveFlags {
    QUIET                    = 0b0000,
    DOUBLE_PAWN_PUSH         = 0b0001,
    KINGSIDE_CASTLE          = 0b0010,
    QUEENSIDE_CASTLE         = 0b0011,
    CAPTURE                  = 0b0100,
    EN_PASSANT_CAPTURE       = 0b0101,
    KNIGHT_PROMOTION         = 0b1000,
    BISHOP_PROMOTION         = 0b1001,
    ROOK_PROMOTION           = 0b1010,
    QUEEN_PROMOTION          = 0b1011,
    KNIGHT_PROMOTION_CAPTURE = 0b1100,
    BISHOP_PROMOTION_CAPTURE = 0b1101,
    ROOK_PROMOTION_CAPTURE   = 0b1110,
    QUEEN_PROMOTION_CAPTURE  = 0b1111
};

constexpr char const* FLAG_NAMES[] = {
    "QUIET",
    "DOUBLE_PAWN_PUSH",
    "KINGSIDE_CASTLE",
    "QUEENSIDE_CASTLE",
    "CAPTURE",
    "EN_PASSANT_CAPTURE",
    "",     // unused
    "",     // unused
    "KNIGHT_PROMOTION",
    "BISHOP_PROMOTION",
    "ROOK_PROMOTION",
    "QUEEN_PROMOTION",
    "KNIGHT_PROMOTION_CAPTURE",
    "BISHOP_PROMOTION_CAPTURE",
    "ROOK_PROMOTION_CAPTURE",
    "QUEEN_PROMOTION_CAPTURE"
};

}   // namespace chess

#endif