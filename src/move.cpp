#include "move.h"

// null move constructor
Move::Move() {
    m_move = 0;
}

// general move constructor
Move::Move(uint8_t from, uint8_t to, uint8_t flag) {
    m_move = from | (to << 6) | (flag << 12);
}

uint8_t Move::getTo() {
    return m_move & TO_MASK;
}

uint8_t Move::getFrom() {
    return (m_move & FROM_MASK);
}

// print move in readable format
void Move::printMoveDetails() {
    std::cout << std::bitset<16>(m_move) << std::endl;
}