#ifndef ATTACKS_HPP
#define ATTACKS_HPP

#include "bitboard.hpp"

namespace attacks {

    void initialize_attack_tables(void);
    void initialize_pawn_attack_table(void);
    bb::U64 get_pawn_attacks(bb::Square square, bb::Side side);

    // extern variables to hold attack tables
    extern bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
}

#endif