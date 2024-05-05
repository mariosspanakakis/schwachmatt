#include "move.h"

// null move constructor
Move::Move() {
    m_move = 0;
}

// general move constructor
Move::Move(bb::Square from, bb::Square to, MoveFlag flag) {
    m_move = 0;
    SetFrom(from);
    SetTo(to);
    SetFlag(flag);
}

bb::Square Move::GetFrom() {
    return (m_move >> FROM_SHIFT) & 0x3f;
}

void Move::SetFrom(bb::Square from) {
    m_move |= (from << FROM_SHIFT);
}

bb::Square Move::GetTo() {
    return (m_move >> TO_SHIFT) & 0x3f;
}

void Move::SetTo(bb::Square to) {
    m_move |= (to << TO_SHIFT); 
}

MoveFlag Move::GetFlag() {
    return (m_move >> FLAG_SHIFT) & 0x3f;
}

void Move::SetFlag(MoveFlag flag) {
    m_move |= (flag << FLAG_SHIFT);
}

void Move::PrintMoveDetails() {
    // print move bitsets
    std::cout << std::bitset<4>(GetFlag()) << " ";
    std::cout << std::bitset<6>(GetTo()) << " ";
    std::cout << std::bitset<6>(GetFrom()) << std::endl;

    // print readable move information
    std::cout << "FROM: " << bb::SQUARE_NAMES[GetFrom()] << std::endl;
    std::cout << "TO:   " << bb::SQUARE_NAMES[GetTo()] << std::endl;
}