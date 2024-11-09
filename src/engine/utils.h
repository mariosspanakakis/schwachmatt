#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include "bitboard.h"
#include "board.h"
#include "move.h"

class Board;

namespace chess {
namespace utils {

// pseudo random number state
extern uint32_t randomState;

// split FEN string into subgroups
std::vector<std::string> splitFen(const std::string& fen);

// pseudo random number generation
uint32_t getRandom32();
uint64_t getRandom64();
uint64_t getRandom64Sparse();

void printMoveSAN(Move move, Board board);
void printMove(Move move);

}   // namespace utils
}   // namespace chess

#endif