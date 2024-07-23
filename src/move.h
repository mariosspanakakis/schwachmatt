#ifndef MOVE_H
#define MOVE_H

#include <bitset>
#include <cstdint>
#include <iostream>
#include <map>
#include "bitboard.h"

/*
The 16 bits of a move are assigned as follows:
    0000 | 000000 | 000000
    FLAG     TO      FROM

Note:
This move representation does not specify the moving or captured piece. It is
therefore necessary to look at the board that the move has been made on in
order to obtain this information.
*/

namespace chess {

class Move {
    private:
    uint16_t m_move;

    public:
    Move();
    Move(Square from, Square to, MoveFlag flag);
    ~Move() = default;
    Square getTo();
    void setTo(Square to);
    Square getFrom();
    void setFrom(Square from);
    MoveFlag getFlag();
    void setFlag(MoveFlag flag);
};


class MoveList {
    private:
    Move m_moves[256];
    int m_size;

    public:
    void add(Move move);
    Move getMove(int index);
    int getSize();
};

}   // namespace chess

#endif