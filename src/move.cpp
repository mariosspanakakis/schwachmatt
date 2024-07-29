#include "move.h"

namespace chess {

Move::Move() {
    m_move = 0;
}

Move::Move(Square from, Square to, MoveFlag flag) {
    m_move = 0;
    setFrom(from);
    setTo(to);
    setFlag(flag);
}

Square Move::getFrom() const {
    return (m_move >> FROM_SHIFT) & 0x3f;
}

void Move::setFrom(Square from) {
    m_move |= (from << FROM_SHIFT);
}

Square Move::getTo() const {
    return (m_move >> TO_SHIFT) & 0x3f;
}

void Move::setTo(Square to) {
    m_move |= (to << TO_SHIFT); 
}

MoveFlag Move::getFlag() const {
    return (m_move >> FLAG_SHIFT) & 0x3f;
}

void Move::setFlag(MoveFlag flag) {
    m_move |= (flag << FLAG_SHIFT);
}

bool Move::isCapture() {
    return (m_move >> FLAG_SHIFT) & 0b0100;
}

bool Move::isDoublePawnPush() {
    return (m_move >> FLAG_SHIFT) == 0b0001;
}

bool Move::isPromotion() {
    return (m_move >> FLAG_SHIFT) & 0b1000;
}

PieceType Move::getPromotionPieceType() {
    return PieceType(((m_move >> FLAG_SHIFT) & 0b0011) + 2);
}

void Move::printDetails() const {
    // print origin and target square
    std::cout << SQUARE_NAMES[getFrom()];
    std::cout << " -> ";
    std::cout << SQUARE_NAMES[getTo()];
    std::cout << " ";

    // print move bitsets
    std::cout << "[" << std::bitset<4>(getFlag()) << " ";
    std::cout << std::bitset<6>(getTo()) << " ";
    std::cout << std::bitset<6>(getFrom()) << "]";
    std::cout << " ";

    // print move flag
    std::cout << FLAG_NAMES[getFlag()];
    std::cout << std::endl;
}

std::string Move::toString() const {
    return std::string(SQUARE_NAMES[getFrom()]) + std::string(SQUARE_NAMES[getTo()]);
}

}   // namespace chess