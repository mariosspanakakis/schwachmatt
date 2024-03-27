#ifndef ATTACKS_H
#define ATTACKS_H

#include "bitboard.h"
#include "utils.h"

namespace attacks {

    // initialize all attack tables for rapid lookup during move generation
    void InitializeAttackTables(void);

    // get attack tables for leaping pieces
    bb::U64 GetPawnAttacks(bb::Square square, bb::Side side);
    bb::U64 GetKnightAttacks(bb::Square square);
    bb::U64 GetKingAttacks(bb::Square square);

    // get attack masks for sliding piece attack generation
    bb::U64 GetBishopAttacks(bb::Square square, bb::U64 blockers = 0ULL);
    bb::U64 GetRookAttacks(bb::Square square, bb::U64 blockers = 0ULL);

    // find a suitable magic number for a given square
    void FindMagicNumber(bb::Square square, int n_bits, bool is_bishop);

    // extern variables to hold attack tables
    extern bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    extern bb::U64 knight_attack_table[bb::N_SQUARES];
    extern bb::U64 king_attack_table[bb::N_SQUARES];
}

#endif