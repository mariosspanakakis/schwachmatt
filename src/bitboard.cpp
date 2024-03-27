#include "bitboard.h"

namespace bb {

    void PrintBitboard(U64 bitboard) {
        std::cout << "\n";
        for (int rank = 7; rank >= 0; rank--) {
            std::cout << " " << rank + 1 << "  ";
            for (int file = 0; file < 8; file++) {
                if ((bitboard >> (rank * 8 + file)) & (U64) 1) {
                    std::cout << " 1";
                } else {
                    std::cout << " 0";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout << "     A B C D E F G H\n";
        std::cout << "\n";
        std::cout << " < Value: " << bitboard << " >";
        std::cout << "\n";
    }
}