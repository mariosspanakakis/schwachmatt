#ifndef BOARD_H
#define BOARD_H

#include <cassert>
#include <cctype>
#include <ctype.h>
#include <stdint.h>
#include <iomanip>
#include <string.h>
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
    Bitboard en_passant_target;
    uint8_t castling_rights;
    Piece captured;
    Color side_to_move;
};

const GameState INITIAL_GAME_STATE = GameState {
    0ULL,                   // no en passant square
    0b00001111,             // full castling rights for both sides
    NO_PIECE,               // no captured piece
    WHITE,                  // white begins
};

struct OccupancyBitboards {
    public:
    Bitboard pieces[N_COLORS][N_PIECE_TYPES];   // piece-wise occupancy         // NOTE: N_PIECE_TYPES is 7, one is not needed
    Bitboard colors[N_COLORS];                  // color-wise occupancy
    Bitboard all;                               // total occupancy
};

class Board {
    private:

    /* Bitboards for occupancy representation. */
    OccupancyBitboards occupancies_;

    /* Pieces by square. */
    Piece pieces_[N_SQUARES];

    /* Stack containing the game state history. */
    std::vector<GameState> game_state_history_;
    
    public:
    Board(const std::string& fen = INITIAL_FEN);
    ~Board() = default;

    Bitboard getPieceOccupancy(PieceType pieceType, Color color) const;
    Bitboard getColorOccupancy(Color color) const;
    Bitboard getTotalOccupancy() const;
    Piece getPieceOnSquare(Square square) const;
    Square getKingSquare(Color color) const;

    Bitboard getCurrentEnPassantTarget() const;
    CastlingRight getCastlingRights() const;
    bool canCastle(CastlingRight cr) const;
    bool isCastlingBlocked(CastlingRight cr) const;
    Color getSideToMove() const;

    void setPiece(Square square, Piece piece);
    void unsetPiece(Square square);
    void replacePiece(Square square, Piece piece);

    /* @brief Test if a square is under attack by a piece of the given color.
     * @param square The square for which to test for attacks.
     * @param color The attacking color.
     * @return A boolean indicating whether the square is under attack. */
    bool isAttackedBy(Square square, Color color) const;

    bool isInCheck(Color color);

    void makeMove(Move move);
    void unmakeMove(Move move);

    bool isLegal(Move move);

    void print();
};

}   // namespace chess

#endif