#include "main.hpp"

int main(int argc, char *argv[]) {

    bb::U64 bitboard = 0x000a000a10204000ULL;
    //bitboard::set_bit(bitboard, bitboard::E4);
    bb::print_bitboard(bitboard);
    
    return 0;
}