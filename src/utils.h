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
    extern uint32_t randomState;

    // split FEN string into subgroups
    std::vector<std::string> splitFen(const std::string& fen);

    // pseudo random number generation
    uint32_t getRandom32();
    bb::U64 getRandom64();
    bb::U64 getRandom64Sparse();
}

#endif