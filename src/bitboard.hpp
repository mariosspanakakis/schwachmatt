#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <stdint.h>
#include <iostream>

namespace bb {

    // type aliases
    using U64 = uint64_t;
    using Square = uint8_t;
    using Side = uint8_t;

    // enumerations
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

    enum Colors {
        WHITE,
        BLACK
    };
    const int N_COLORS = 2;

    // indices to address pieces in bitboards
    enum PieceIndices {
        PAWN_IDX,
        KNIGHT_IDX,
        BISHOP_IDX,
        ROOK_IDX,
        QUEEN_IDX,
        KING_IDX
    };

    enum Pieces {
        WHITE_PAWN,
        WHITE_KNIGHT,
        WHITE_BISHOP,
        WHITE_ROOK,
        WHITE_QUEEN,
        WHITE_KING,
        BLACK_PAWN,
        BLACK_KNIGHT,
        BLACK_BISHOP,
        BLACK_ROOK,
        BLACK_QUEEN,
        BLACK_KING,
    };
    const int N_PIECES = 12;

    enum Ranks {
        RANK_1,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8
    };

    enum Files {
        FILE_A,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H
    };

    // rank definitions
    constexpr U64 RANK_1_BB = 0x00000000000000FFULL;
    constexpr U64 RANK_2_BB = RANK_1_BB << 8;
    constexpr U64 RANK_3_BB = RANK_1_BB << 16;
    constexpr U64 RANK_4_BB = RANK_1_BB << 24;
    constexpr U64 RANK_5_BB = RANK_1_BB << 32;
    constexpr U64 RANK_6_BB = RANK_1_BB << 40;
    constexpr U64 RANK_7_BB = RANK_1_BB << 48;
    constexpr U64 RANK_8_BB = RANK_1_BB << 56;

    // file definitions
    constexpr U64 FILE_H_BB = 0x8080808080808080ULL;
    constexpr U64 FILE_G_BB = FILE_H_BB >> 1;
    constexpr U64 FILE_F_BB = FILE_H_BB >> 2;
    constexpr U64 FILE_E_BB = FILE_H_BB >> 3;
    constexpr U64 FILE_D_BB = FILE_H_BB >> 4;
    constexpr U64 FILE_C_BB = FILE_H_BB >> 5;
    constexpr U64 FILE_B_BB = FILE_H_BB >> 6;
    constexpr U64 FILE_A_BB = FILE_H_BB >> 7;

    // print a readable bitboard representation to the console
    void print_bitboard(U64 bitboard);
    
    // bit manipulation
    inline void set_bit(U64& bitboard, Square square){
        bitboard |= (1ULL << square);
    }
    inline bool get_bit(U64& bitboard, Square square){
        return ((bitboard >> square) & 1ULL) == 1;
    }
    inline void clear_bit(U64& bitboard, Square square){
        bitboard &= ~(1ULL << square);
    }

    // bit shift operations with range checks
    inline U64 shift_north(U64 bitboard){
        return (bitboard << 8);
    }
    inline U64 shift_south(U64 bitboard){
        return (bitboard >> 8);
    }
    inline U64 shift_east(U64 bitboard){
        return (bitboard << 1) & ~FILE_A_BB;
    }
    inline U64 shift_west(U64 bitboard){
        return (bitboard >> 1) & ~FILE_H_BB;
    }
    inline U64 shift_northeast(U64 bitboard){
        return (bitboard << 9) & ~FILE_A_BB;
    }
    inline U64 shift_northwest(U64 bitboard){
        return (bitboard << 7) & ~FILE_H_BB;
    }
    inline U64 shift_southeast(U64 bitboard){
        return (bitboard >> 7) & ~FILE_A_BB;
    }
    inline U64 shift_southwest(U64 bitboard){
        return (bitboard >> 9) & ~FILE_H_BB;
    }

    // square conversion
    inline int coord_to_square(int rank, int file){
        return 8 * rank + file;
    }
}

#endif