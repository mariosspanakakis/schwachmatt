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

Square Move::getFrom() {
    return (m_move >> FROM_SHIFT) & 0x3f;
}

void Move::setFrom(Square from) {
    m_move |= (from << FROM_SHIFT);
}

Square Move::getTo() {
    return (m_move >> TO_SHIFT) & 0x3f;
}

void Move::setTo(Square to) {
    m_move |= (to << TO_SHIFT); 
}

MoveFlag Move::getFlag() {
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

}   // namespace chess