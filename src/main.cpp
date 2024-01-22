#include "main.hpp"

int main(int argc, char *argv[]) {

    bitboard::U64 bitboard = 0x000a000a10204000ULL;
    //bitboard::set_bit(bitboard, bitboard::E4);
    bitboard::print_bitboard(bitboard);
    
    return 0;
}