#ifndef ATTACKS_HPP
#define ATTACKS_HPP

#include "bitboard.hpp"

namespace attacks {

    // functions for attack table initialization
    void initialize_attack_tables(void);
    bb::U64 get_pawn_attacks(bb::Square square, bb::Side side);
    bb::U64 get_knight_attacks(bb::Square square);
    bb::U64 get_king_attacks(bb::Square square);

    // get X-ray attack maps for sliding piece attack generation
    bb::U64 get_bishop_xray_attacks(bb::Square square);
    bb::U64 get_rook_xray_attacks(bb::Square square);

    // extern variables to hold attack tables
    extern bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    extern bb::U64 knight_attack_table[bb::N_SQUARES];
    extern bb::U64 king_attack_table[bb::N_SQUARES];
    extern bb::U64 bishop_xray_attack_table[bb::N_SQUARES];
    extern bb::U64 rook_xray_attack_table[bb::N_SQUARES];
}

#endif