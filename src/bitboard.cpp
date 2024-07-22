#include "bitboard.h"

namespace bb {

    void printBitboard(U64 bitboard) {
        std::cout << std::endl;
        for (int rank = 7; rank >= 0; rank--) {
            std::cout << " " << rank + 1 << "  ";
            for (int file = 0; file < 8; file++) {
                if ((bitboard >> (rank * 8 + file)) & (U64) 1) {
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

    U64 getPopulatedBitboard(const std::vector<Square>& populatedSquares) {
        U64 bitboard = 0ULL;
        for (auto it = populatedSquares.begin(); it != populatedSquares.end(); ++it){
            bb::setBit(bitboard, *it);
        }
        return bitboard;
    }

    int countBits(bb::U64 bitboard) {
        int r;
        for(r = 0; bitboard; r++, bitboard &= bitboard - 1);
        return r;
    }

    int getLeastSignificantBitIndex(U64 bitboard) {
        if (bitboard) return countBits((bitboard & -bitboard) - 1);
        else return -1;
    }
}