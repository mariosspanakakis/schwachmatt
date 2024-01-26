#include "attacks.hpp"

namespace attacks {

    bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    bb::U64 knight_attack_table[bb::N_SQUARES];

    bb::U64 get_pawn_attacks(bb::Square square, bb::Side side){
        
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::set_bit(bitboard, square);
        bb::U64 attacks = 0ULL;

        // white to move
        if (!side){
            attacks |= bb::shift_northwest(bitboard) | bb::shift_northeast(bitboard);
        }
        // black to move
        else{
            attacks |= bb::shift_southwest(bitboard) | bb::shift_southeast(bitboard);
        }

        return attacks;
    }

    bb::U64 get_knight_attacks(bb::Square square){
        // initialize bitboards for knight position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::set_bit(bitboard, square);
        bb::U64 attacks = 0ULL;
        // generate attacks
        attacks |= (
              ((bitboard << 6)  & ~(bb::FILE_G_BB | bb::FILE_H_BB))     // WSW
            | ((bitboard << 10) & ~(bb::FILE_A_BB | bb::FILE_B_BB))     // ESE
            | ((bitboard << 15) & ~(bb::FILE_H_BB))                     // SSW
            | ((bitboard << 17) & ~(bb::FILE_A_BB))                     // SSE
            | ((bitboard >> 6)  & ~(bb::FILE_A_BB | bb::FILE_B_BB))     // ENE
            | ((bitboard >> 10) & ~(bb::FILE_G_BB | bb::FILE_H_BB))     // WNW
            | ((bitboard >> 15) & ~(bb::FILE_A_BB))                     // NNE
            | ((bitboard >> 17) & ~(bb::FILE_H_BB))                     // NNW
        );

        return attacks;
    }

    void initialize_attack_tables(){
        // generate pawn attack table
        for (int side=0; side<bb::N_COLORS; ++side){
            for (int square = 0; square < bb::N_SQUARES; square++){
                pawn_attack_table[side][square] = get_pawn_attacks(square, side);
            }
        }
        // generate knight attack table
        for (int square = 0; square < bb::N_SQUARES; square++){
            knight_attack_table[square] = get_knight_attacks(square);
        }
    }
}