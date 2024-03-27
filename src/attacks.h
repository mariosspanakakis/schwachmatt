#ifndef ATTACKS_H
#define ATTACKS_H

#include "bitboard.h"

namespace attacks {

    // functions for attack table initialization
    void InitializeAttackTables(void);
    bb::U64 GetPawnAttacks(bb::Square square, bb::Side side);
    bb::U64 GetKnightAttacks(bb::Square square);
    bb::U64 GetKingAttacks(bb::Square square);

    // attack masks for sliding piece attack generation
    bb::U64 GetBishopAttackMask(bb::Square square);
    bb::U64 GetRookAttackMask(bb::Square square);

    // extern variables to hold attack tables
    extern bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    extern bb::U64 knight_attack_table[bb::N_SQUARES];
    extern bb::U64 king_attack_table[bb::N_SQUARES];
    extern bb::U64 bishop_xray_attack_table[bb::N_SQUARES];
    extern bb::U64 rook_xray_attack_table[bb::N_SQUARES];
}

#endif