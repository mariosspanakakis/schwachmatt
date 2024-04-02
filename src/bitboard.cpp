#include "bitboard.h"

namespace bb {

    void PrintBitboard(U64 bitboard) {
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

    U64 GetPopulatedBitboard(const std::vector<Square>& populated_squares) {
        U64 bitboard = 0ULL;
        for (auto it = populated_squares.begin(); it != populated_squares.end(); ++it){
            bb::SetBit(bitboard, *it);
        }
        return bitboard;
    }

    int CountBits(bb::U64 bitboard) {
        int r;
        for(r = 0; bitboard; r++, bitboard &= bitboard - 1);
        return r;
    }

    int GetLeastSignificantBitIndex(U64 bitboard) {
        if (bitboard) return CountBits((bitboard & -bitboard) - 1);
        else return -1;
    }
}