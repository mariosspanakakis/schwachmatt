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
with FLAG being
    PCSS (promotion - capture - special 1 - special 2)
    0000 - quiet
    0001 - double pawn push
    0010 - kingside castle
    0011 - queenside castle
    0100 - capture
    0101 - en-passant capture
    1000 - knight capture
    1001 - bishop capture
    1010 - rook capture
    1011 - queen capture
    1100 - knight capture with promotion
    1101 - bishop capture with promotion
    1110 - rook capture with promotion
    1111 - queen capture with promotion
*/

enum MoveShifts {
    FROM_SHIFT = 0,
    TO_SHIFT = 6,
    FLAG_SHIFT = 12
};

enum MoveMasks {
    TO_MASK = 0x3f,     // 0000 000000 111111
    FROM_MASK = 0xfc0   // 0000 111111 000000
};


class Move {
private:
    uint16_t m_move;

public:
    Move();
    Move(bb::Square from, bb::Square to, uint16_t flag);
    ~Move() = default;

    // move information
    bb::Square getTo();
    void setTo(bb::Square to);
    bb::Square getFrom();
    void setFrom(bb::Square from);
    uint8_t getFlag();
    void setFlag(uint8_t flag);

    // print move in standard algebraic notation, e.g. Bxe5
    void printMove();

    // print detailed move representation
    void printMoveDetails();
};

#endif