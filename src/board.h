#ifndef BOARD_H
#define BOARD_H

#include <stdexcept>
#include <stdint.h>
#include <cctype>
#include <ctype.h>
#include "attacks.h"
#include "bitboard.h"
#include "utils.h"

// standard initial board configuration
const std::string initialFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// masks for the castling rights, each represented as a single bit
enum CastlingRights {
    WHITE_KINGSIDE_CASTLE   = 0b0001,
    WHITE_QUEENSIDE_CASTLE  = 0b0010,
    BLACK_KINGSIDE_CASTLE   = 0b0100,
    BLACK_QUEENSIDE_CASTLE  = 0b1000
};

// game state, containing all non-visible board information
struct GameState {
    public:
    bb::U64 enPassantTarget;
    uint8_t castlingRights;
};

const GameState initialGameState = GameState {
    0ULL,                   // no en passant square
    0b00001111              // full castling rights for both sides
};

class Board {
    private:
    /* Separate occupancy bitboards for all pieces. */
    bb::U64 m_pieceBB[bb::N_COLORS][bb::N_PIECES];
    /* Total occupancy bitboards for each side. */
    bb::U64 m_occupancyBB[bb::N_COLORS];
    /* Combined occupancy bitboard for both sides. */
    bb::U64 m_occupancyCombinedBB;
    /* Stack containing the game's state history. */
    std::vector<GameState> m_gameStateHistory;
    
    public:
    Board(const std::string& fen = initialFEN);
    ~Board() = default;

    bb::U64 getPieceBitboard(bb::Piece, bb::Color);
    bb::U64 getOccupancyBitboard(bb::Color);
    bb::U64 getCombinedOccupancyBitboard();

    /* @brief Test if a square is under attack by a piece of the given color.
     * @param square The square for which to test for attacks.
     * @param color The attacking color.
     * @return A boolean indicating whether the square is under attack. */
    bool isAttackedBy(bb::Square square, bb::Color color);

    bb::U64 getCurrentEnPassantTarget();
    bool getCastlingRight(uint8_t castling_right);
};

class InvalidFENException : public std::runtime_error {
public:
    InvalidFENException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif