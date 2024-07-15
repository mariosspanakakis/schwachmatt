#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "board.h"
#include "attacks.h"
#include "move.h"

namespace movegen {
    // generate all pseudo-legal moves
    void GenerateMoves(Board& board, bb::Color color);
    // generate pseudo-legal moves for separate pieces
    void GeneratePawnMoves(Board& board, bb::Color color);
    void GenerateKnightMoves(Board& board, bb::Color color);
}

#endif