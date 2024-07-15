#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>

namespace bb {

    // type aliases
    using U64 = uint64_t;
    using Square = uint8_t;
    using Piece = int8_t;
    using Color = uint8_t;

    // board squares
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

    // names of the board squares
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

    enum Colors {WHITE, BLACK};
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

    // chess piece indices
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

    // rank and file indices
    enum Ranks {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};
    enum Files {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};

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
    void PrintBitboard(U64 bitboard);

    // get a bitboard with the given squares set for testing purposes
    U64 GetPopulatedBitboard(const std::vector<Square>& populated_squares);
    
    // bit manipulation
    inline void SetBit(U64& bitboard, Square square){
        bitboard |= (1ULL << square);
    }
    inline bool GetBit(U64& bitboard, Square square){
        return ((bitboard >> square) & 1ULL) == 1;
    }
    inline void ClearBit(U64& bitboard, Square square){
        bitboard &= ~(1ULL << square);
    }

    // bit shift operations with range checks
    inline U64 ShiftNorth(U64 bitboard){
        return (bitboard << 8);
    }
    inline U64 ShiftSouth(U64 bitboard){
        return (bitboard >> 8);
    }
    inline U64 ShiftEast(U64 bitboard){
        return (bitboard << 1) & ~FILE_A_BB;
    }
    inline U64 ShiftWest(U64 bitboard){
        return (bitboard >> 1) & ~FILE_H_BB;
    }
    inline U64 ShiftNorthEast(U64 bitboard){
        return (bitboard << 9) & ~FILE_A_BB;
    }
    inline U64 ShiftNorthWest(U64 bitboard){
        return (bitboard << 7) & ~FILE_H_BB;
    }
    inline U64 ShiftSouthEast(U64 bitboard){
        return (bitboard >> 7) & ~FILE_A_BB;
    }
    inline U64 ShiftSouthWest(U64 bitboard){
        return (bitboard >> 9) & ~FILE_H_BB;
    }

    // convert a rank-file coordinate to a square
    inline int ConvertCoordToSquare(int rank, int file){
        return 8 * rank + file;
    }

    // other bit operations
    int CountBits(U64 bitboard);
    int GetLeastSignificantBitIndex(U64 bitboard);
}

#endif