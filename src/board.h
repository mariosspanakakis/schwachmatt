#ifndef BOARD_H
#define BOARD_H

#include <stdexcept>
#include <stdint.h>
#include <cctype>
#include <ctype.h>
#include "bitboard.h"
#include "utils.h"

// standard initial board configuration
const std::string initial_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

class Board {
private:
    // bitboards for all pieces; white starts at ID 0, black starts at ID 6
    bb::U64 m_pieces_BB[bb::N_PIECES];
    // occupancy bitboards for each side
    bb::U64 m_occupancy_BB[bb::N_COLORS];
    // combined occupancy bitboard
    bb::U64 m_occupancy_combined_BB;
public:
    Board(const std::string& fen = initial_fen);
    ~Board() = default;
};

#endif