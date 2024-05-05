#include "move.h"

// null move constructor
Move::Move() {
    m_move = 0;
}

// general move constructor
Move::Move(bb::Square from, bb::Square to, uint16_t flag) {
    m_move = 0;
    setFrom(from);
    setTo(to);
    setFlag(flag);
}

bb::Square Move::getFrom() {
    return (m_move >> FROM_SHIFT) & 0x3f;
}

void Move::setFrom(bb::Square from) {
    m_move |= (from << FROM_SHIFT);
}

bb::Square Move::getTo() {
    return (m_move >> TO_SHIFT) & 0x3f;
}

void Move::setTo(bb::Square to) {
    m_move |= (to << TO_SHIFT); 
}

uint8_t Move::getFlag() {
    return (m_move >> FLAG_SHIFT) & 0x3f;
}

void Move::setFlag(uint8_t flag) {
    m_move |= (flag << FLAG_SHIFT);
}

void Move::printMoveDetails() {
    // print move bitsets
    std::cout << std::bitset<4>(getFlag()) << " ";
    std::cout << std::bitset<6>(getTo()) << " ";
    std::cout << std::bitset<6>(getFrom()) << std::endl;

    // print readable move information
    std::cout << "FROM: " << bb::SQUARE_NAMES[getFrom()] << std::endl;
    std::cout << "TO:   " << bb::SQUARE_NAMES[getTo()] << std::endl;
}