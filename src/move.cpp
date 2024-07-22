#include "move.h"

Move::Move() {
    m_move = 0;
}

Move::Move(bb::Square from, bb::Square to, mv::MoveFlag flag) {
    m_move = 0;
    setFrom(from);
    setTo(to);
    setFlag(flag);
}

bb::Square Move::getFrom() {
    return (m_move >> mv::FROM_SHIFT) & 0x3f;
}

void Move::setFrom(bb::Square from) {
    m_move |= (from << mv::FROM_SHIFT);
}

bb::Square Move::getTo() {
    return (m_move >> mv::TO_SHIFT) & 0x3f;
}

void Move::setTo(bb::Square to) {
    m_move |= (to << mv::TO_SHIFT); 
}

mv::MoveFlag Move::getFlag() {
    return (m_move >> mv::FLAG_SHIFT) & 0x3f;
}

void Move::setFlag(mv::MoveFlag flag) {
    m_move |= (flag << mv::FLAG_SHIFT);
}


// move list to store moves during move generation
void MoveList::add(Move move) {
    m_moves[m_size++] = move;
}

Move MoveList::getMove(int index) {                                             // TODO: this is lacking range checks!
    return m_moves[index];
}

int MoveList::getSize() {
    return m_size;
}


namespace mv {
    void printMoveDetails(Move move) {
        // print origin and target square
        std::cout << bb::SQUARE_NAMES[move.getFrom()];
        std::cout << " -> ";
        std::cout << bb::SQUARE_NAMES[move.getTo()];
        std::cout << " ";

        // print move bitsets
        std::cout << "[" << std::bitset<4>(move.getFlag()) << " ";
        std::cout << std::bitset<6>(move.getTo()) << " ";
        std::cout << std::bitset<6>(move.getFrom()) << "]";
        std::cout << " ";

        // print move flag
        std::cout << mv::FLAG_NAMES[move.getFlag()];
        std::cout << std::endl;
    }
}