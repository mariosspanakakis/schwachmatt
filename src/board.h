#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <cctype>
#include <ctype.h>
#include "attacks.h"
#include "bitboard.h"
#include "exceptions.h"
#include "move.h"
#include "types.h"
#include "utils.h"

namespace chess {

// standard initial board configuration
const std::string INITIAL_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// game state, containing all non-visible board information
struct GameState {
    public:
    Bitboard enPassantTarget;
    uint8_t castlingRights;
};

const GameState INITIAL_GAME_STATE = GameState {
    0ULL,                   // no en passant square
    0b00001111              // full castling rights for both sides
};

class Board {
    private:
    /* Separate occupancy bitboards for all pieces. */
    Bitboard m_pieceBB[N_COLORS][N_PIECES];
    /* Total occupancy bitboards for each side. */
    Bitboard m_occupancyBB[N_COLORS];
    /* Combined occupancy bitboard for both sides. */
    Bitboard m_occupancyCombinedBB;
    /* Stack containing the game's state history. */
    std::vector<GameState> m_gameStateHistory;
    
    public:
    Board(const std::string& fen = INITIAL_FEN);
    ~Board() = default;

    Bitboard getPieceBitboard(Piece, Color) const;
    Bitboard getOccupancyBitboard(Color) const;
    Bitboard getCombinedOccupancyBitboard() const;

    /* @brief Test if a square is under attack by a piece of the given color.
     * @param square The square for which to test for attacks.
     * @param color The attacking color.
     * @return A boolean indicating whether the square is under attack. */
    bool isAttackedBy(Square square, Color color) const;

    void makeMove(Move move);

    Bitboard getCurrentEnPassantTarget() const;
    bool getCastlingRight(CastlingRight castling_right) const;
};

}   // namespace chess

#endif