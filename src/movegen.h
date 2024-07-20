#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "board.h"
#include "attacks.h"
#include "move.h"

namespace movegen {
    // generate all pseudo-legal moves
    std::vector<Move> GenerateMoves(Board& board, bb::Color color);
    // generate pseudo-legal moves for separate pieces
    std::vector<Move> GeneratePawnMoves(Board& board, bb::Color color);
    std::vector<Move> GenerateKnightMoves(Board& board, bb::Color color);
}

#endif