#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "bitboard.h"

namespace utils {

    // pseudo random number state
    extern bb::U64 random_state_64;

    // split FEN string into subgroups
    std::vector<std::string> SplitFen(std::string fen);

    // pseudo random number generation
    bb::U64 GetRandom64();
    bb::U64 GetRandom64Sparse();
}

#endif