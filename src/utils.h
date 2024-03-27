#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <iostream>
#include "bitboard.h"

namespace utils {

    // pseudo random number state
    extern bb::U64 random_state_64;

    std::string* SplitFen(std::string fen);
    bb::U64 GetRandom64();
    bb::U64 GetRandom64Sparse();
}

#endif