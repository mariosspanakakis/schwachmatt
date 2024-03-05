#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdint.h>
#include <iostream>
#include "bitboard.hpp"

namespace utils {

    // pseudo random number state
    extern bb::U64 random_state_64;

    std::string* split_fen(std::string fen);
    bb::U64 get_random_64_bit();
    bb::U64 get_random_sparse_64_bit();
}

#endif