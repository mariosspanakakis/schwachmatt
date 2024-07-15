#ifndef MOVE_H
#define MOVE_H

#include <bitset>
#include <cstdint>
#include <iostream>
#include "bitboard.h"

/*
The 16 bits of a move are assigned as follows:
    0000 | 000000 | 000000
    FLAG     TO      FROM
*/

namespace mv {
    // type aliases
    using MoveFlag = uint8_t;

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
}


class Move {
private:
    uint16_t m_move;

public:
    Move();
    Move(bb::Square from, bb::Square to, mv::MoveFlag flag);
    ~Move() = default;

    // move information
    bb::Square GetTo();
    void SetTo(bb::Square to);
    bb::Square GetFrom();
    void SetFrom(bb::Square from);
    mv::MoveFlag GetFlag();
    void SetFlag(mv::MoveFlag flag);

    // print move in standard algebraic notation, e.g. Bxe5
    void PrintMove();

    // print detailed move representation
    void PrintMoveDetails();
};

#endif