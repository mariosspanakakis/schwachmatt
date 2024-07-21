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

// game state, containing all non-visible board information
struct GameState {
    public:
    bb::U64 en_passant_target;
};

const GameState initial_game_state = GameState{
    .en_passant_target = 0ULL
};

class Board {
    private:
    // separate occupancy bitboards for all pieces
    bb::U64 m_pieces_BB[bb::N_COLORS][bb::N_PIECES];
    // total occupancy bitboards for each side
    bb::U64 m_occupancy_BB[bb::N_COLORS];
    // combined occupancy bitboard
    bb::U64 m_occupancy_combined_BB;
    // stack containing the game's state history
    std::vector<GameState> m_game_state_history;
    
    public:
    Board(const std::string& fen = initial_fen);
    ~Board() = default;

    bb::U64 GetPieceBitboard(bb::Piece, bb::Color);
    bb::U64 GetOccupancyBitboard(bb::Color);
    bb::U64 GetCombinedOccupancyBitboard();
    GameState GetCurrentGameState();
};

#endif