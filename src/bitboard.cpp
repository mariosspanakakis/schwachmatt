#include "bitboard.h"

namespace chess {
namespace bb {

void printBitboard(Bitboard bitboard) {
    std::cout << std::endl;
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << " " << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            if ((bitboard >> (rank * 8 + file)) & (Bitboard) 1) {
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
    std::cout << " < Value: " << bitboard << " >";
    std::cout << std::endl;
}

Bitboard getPopulatedBitboard(const std::vector<Square>& populatedSquares) {
    Bitboard bitboard = 0ULL;
    for (auto it = populatedSquares.begin(); it != populatedSquares.end(); ++it) {
        bb::setBit(bitboard, *it);
    }
    return bitboard;
}

int countBits(Bitboard bitboard) {
    int r;
    for(r = 0; bitboard; r++, bitboard &= bitboard - 1);
    return r;
}

int getLeastSignificantBitIndex(Bitboard bitboard) {
    if (bitboard) return countBits((bitboard & -bitboard) - 1);
    else return -1;
}

}   // namespace bb
}   // namespace chess