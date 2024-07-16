#include "move.h"

// null move constructor
Move::Move() {
    m_move = 0;
}

// general move constructor
Move::Move(bb::Square from, bb::Square to, mv::MoveFlag flag) {
    m_move = 0;
    SetFrom(from);
    SetTo(to);
    SetFlag(flag);
}

bb::Square Move::GetFrom() {
    return (m_move >> mv::FROM_SHIFT) & 0x3f;
}

void Move::SetFrom(bb::Square from) {
    m_move |= (from << mv::FROM_SHIFT);
}

bb::Square Move::GetTo() {
    return (m_move >> mv::TO_SHIFT) & 0x3f;
}

void Move::SetTo(bb::Square to) {
    m_move |= (to << mv::TO_SHIFT); 
}

mv::MoveFlag Move::GetFlag() {
    return (m_move >> mv::FLAG_SHIFT) & 0x3f;
}

void Move::SetFlag(mv::MoveFlag flag) {
    m_move |= (flag << mv::FLAG_SHIFT);
}

namespace mv {
    void PrintMoveDetails(Move move) {
        // print move bitsets
        std::cout << std::bitset<4>(move.GetFlag()) << " ";
        std::cout << std::bitset<6>(move.GetTo()) << " ";
        std::cout << std::bitset<6>(move.GetFrom()) << std::endl;

        // print readable move information
        std::cout << "FROM: " << bb::SQUARE_NAMES[move.GetFrom()] << std::endl;
        std::cout << "TO:   " << bb::SQUARE_NAMES[move.GetTo()] << std::endl;
    }
}