#include "bitboard.hpp"

namespace bb {

Bitboard getBitboard(const std::vector<Square>& squares) {
    Bitboard b = 0ULL;
    for (auto it = squares.begin(); it != squares.end(); ++it) {
        b |= getBitboard(*it);
    }
    return b;
}

void print(Bitboard b) {
    std::cout << std::endl;
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << " " << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            if ((b >> (rank * 8 + file)) & (Bitboard) 1) {
                std::cout << " 1";
            } else {
                std::cout << " 0";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "     A B C D E F G H" << std::endl;
    std::cout << std::endl;
    std::cout << " < Value: " << b << " >";
    std::cout << std::endl;
}

}   // namespace bb
