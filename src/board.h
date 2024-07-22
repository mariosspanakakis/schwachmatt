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

// castling rights, used as indices to set the corresponding bits in the game state
enum CastlingRights {
    WHITE_KINGSIDE_CASTLE   = 0b0001,
    WHITE_QUEENSIDE_CASTLE  = 0b0010,
    BLACK_KINGSIDE_CASTLE   = 0b0100,
    BLACK_QUEENSIDE_CASTLE  = 0b1000
};

// game state, containing all non-visible board information
struct GameState {
    public:
    bb::U64 en_passant_target;
    uint8_t castling_rights;
};

const GameState initial_game_state = GameState{
    .en_passant_target = 0ULL,          // no en passant square
    .castling_rights = 0b00001111       // full castling rights for both sides
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
    bb::U64 GetCurrentEnPassantTarget();
    bool GetCastlingRight(uint8_t castling_right);
};

#endif