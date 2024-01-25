#include "attacks.hpp"

namespace attacks {

    bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];

    bb::U64 get_pawn_attacks(bb::Square square, bb::Side side){
        
        // setup bitboard for actual pawn position
        bb::U64 bitboard = 0ULL;
        bb::set_bit(bitboard, square);

        // setup empty bitboard for attacked positions
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

    void initialize_pawn_attack_table(){
        // loop over squares and fill table with precalculated attacks
        for (int side=0; side<bb::N_COLORS; ++side){
            for (int square = 0; square < bb::N_SQUARES; square++){
                pawn_attack_table[side][square] = get_pawn_attacks(square, side);
            }
        }
    }

    void initialize_attack_tables(){
        initialize_pawn_attack_table();
    }
}