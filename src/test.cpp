#include "attacks.hpp"
#include "bitboard.hpp"
#include "types.hpp"

int main(int argc, char* argv[]) {
    
    attacks::precalculate(true);

    //Bitboard bb = bb::getBitboard(Squares::A1);
    //bb::print(attacks::ROOK_MASK[Squares::B2]);

    for (Square square = 0; square < N_SQUARES; ++square) {
        
    }

    return 0;
}