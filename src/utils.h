#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include "bitboard.h"

namespace utils {

    // pseudo random number state
    extern uint32_t random_state;

    // split FEN string into subgroups
    std::vector<std::string> SplitFen(const std::string& fen);

    // pseudo random number generation
    uint32_t GetRandom32();
    bb::U64 GetRandom64();
    bb::U64 GetRandom64Sparse();
}

#endif