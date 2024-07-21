#include "move.h"

Move::Move() {
    m_move = 0;
}

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


// move list to store moves during move generation
void MoveList::Add(Move move) {
    m_moves[m_size++] = move;
}

Move MoveList::GetMove(int index) {                                             // TODO: this is lacking range checks!
    return m_moves[index];
}

int MoveList::GetSize() {
    return m_size;
}


namespace mv {
    void PrintMoveDetails(Move move) {
        // print origin and target square
        std::cout << bb::SQUARE_NAMES[move.GetFrom()];
        std::cout << " -> ";
        std::cout << bb::SQUARE_NAMES[move.GetTo()];
        std::cout << " ";

        // print move bitsets
        std::cout << "[" << std::bitset<4>(move.GetFlag()) << " ";
        std::cout << std::bitset<6>(move.GetTo()) << " ";
        std::cout << std::bitset<6>(move.GetFrom()) << "]";
        std::cout << " ";

        // print move flag
        std::cout << mv::FLAG_NAMES[move.GetFlag()];
        std::cout << std::endl;
    }
}