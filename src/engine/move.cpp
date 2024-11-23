#include "move.h"

Move::Move() {
    move_ = 0;
}

Move::Move(Square from, Square to, MoveFlag flag) {
    move_ = 0;
    setFrom(from);
    setTo(to);
    setFlag(flag);
}

bool Move::isCastling() {
    MoveFlag flag = getFlag();
    return (flag == KINGSIDE_CASTLE || flag == QUEENSIDE_CASTLE);
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
