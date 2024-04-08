#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

/*
The 32 bits of a move are assigned as follows:
  00000000 |      0000      |     0000     | 0000 | 000000 | 000000
  (unused)   CAPTURED PIECE   MOVING PIECE   FLAG     TO      FROM   
*/

class Move {
private:
    uint32_t m_move;

public:
    Move();
    Move(uint8_t from, uint8_t to, uint8_t flag, uint8_t type);
    ~Move() = default;
};

#endif