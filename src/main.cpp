#include "main.hpp"

int main(int argc, char *argv[]) {

    attacks::initialize_attack_tables();

    //bb::U64 bitboard = 0ULL;
    //bb::set_bit(bitboard, bb::E4);

    bb::print_bitboard(attacks::king_attack_table[bb::E4]);
    
    return 0;
}