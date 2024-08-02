#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace chess {

using Bitboard = uint64_t;
using Square = uint8_t;
using Piece = uint8_t;
using PieceType = uint8_t;
using Direction = int8_t;
using Color = uint8_t;
using CastlingRight = uint8_t;
using MoveFlag = uint8_t;

const int MAX_NUMBER_OF_MOVES = 256;            // is actually 218
const int GAME_STATE_HISTORY_LENGTH = 512;

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

constexpr char const* COLOR_NAMES[] {
    "WHITE",
    "BLACK"
};

enum PieceTypes {
    NO_PIECE_TYPE = 0,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};
const int N_PIECE_TYPES = 7;

constexpr char const* PIECE_TYPE_NAMES[] {
    "NO_PIECE_TYPE",
    "PAWN",
    "KNIGHT",
    "BISHOP",
    "ROOK",
    "QUEEN",
    "KING",
};

const int PIECE_ID_OFFSET = 8;
enum Pieces {
    NO_PIECE = 0,
    WHITE_PAWN = PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN = PAWN + PIECE_ID_OFFSET,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,
};
const int N_PIECES = 13;

constexpr char const* PIECE_SYMBOLS[] {
    "#",
    "P",
    "N",
    "B",
    "R",
    "Q",
    "K",
    "",     // unused
    "",     // unused
    "p",
    "n",
    "b",
    "r",
    "q",
    "k",
};

const inline Piece makePiece(Color c, PieceType pt) {
    return Piece((c << 3) + pt);
}
const inline Color colorOf(Piece pc) {
    return Color(pc >> 3);
}
const inline PieceType typeOf(Piece pc) {
    return PieceType(pc & 7);
}

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

enum CastlingRights {
    NO_CASTLING = 0,
    WHITE_KINGSIDE_CASTLING   = 0b0001,
    WHITE_QUEENSIDE_CASTLING  = 0b0010,
    BLACK_KINGSIDE_CASTLING   = 0b0100,
    BLACK_QUEENSIDE_CASTLING  = 0b1000,

    WHITE_CASTLING = WHITE_KINGSIDE_CASTLING | WHITE_QUEENSIDE_CASTLING,
    BLACK_CASTLING = BLACK_KINGSIDE_CASTLING | BLACK_QUEENSIDE_CASTLING,
    KINGSIDE_CASTLING = WHITE_KINGSIDE_CASTLING | BLACK_KINGSIDE_CASTLING,
    QUEENSIDE_CASTLING = WHITE_QUEENSIDE_CASTLING | BLACK_QUEENSIDE_CASTLING,
    ANY_CASTLING   = WHITE_CASTLING | BLACK_CASTLING
};
const int N_CASTLING_RIGHTS = 16;
constexpr Bitboard CASTLING_SQUARES[N_CASTLING_RIGHTS] = {                      // not too elegant to hardcode those
    0x0,
    0x0000000000000060,
    0x000000000000000E,
    0x0,
    0x6000000000000000,
    0x0,
    0x0,
    0x0,
    0x0E00000000000000
};
constexpr Square CASTLING_KING_GOAL_SQUARE[N_CASTLING_RIGHTS] = { 0, G1, C1, 0, G8, 0, 0, 0, C8 };

constexpr CastlingRights operator&(Color c, CastlingRights cr) {
    return CastlingRights((c == WHITE ? WHITE_CASTLING : BLACK_CASTLING) & (int)cr);
}

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