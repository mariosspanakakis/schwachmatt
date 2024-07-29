#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <cassert>
#include <cctype>
#include <ctype.h>
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
    Bitboard enPassantTarget;
    uint8_t castlingRights;
    Piece capturedPiece;
    Color sideToMove;
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
    OccupancyBitboards m_occupancies;

    /* Pieces by square. */
    Piece m_pieces[N_SQUARES];

    /* Stack containing the game state history. */
    std::vector<GameState> m_gameStateHistory;
    
    public:
    Board(const std::string& fen = INITIAL_FEN);
    ~Board() = default;

    Bitboard getPieceOccupancy(PieceType pieceType, Color color) const;
    Bitboard getColorOccupancy(Color color) const;
    Bitboard getTotalOccupancy() const;
    Piece getPieceOnSquare(Square square) const;

    Bitboard getCurrentEnPassantTarget() const;
    CastlingRight getCastlingRights() const;
    CastlingRight getCastlingRights(Color color) const;
    bool canCastle(Color color, CastlingRight castling_right) const;
    Color getSideToMove() const;

    void setPiece(Square square, PieceType pieceType, Color color);
    void unsetPiece(Square square, PieceType pieceType, Color color);           // we must compute which piece has been on that square anyway, so we just specify it (in case we already know)
    void replacePiece(Square square, PieceType pieceType, Color color);

    /* @brief Test if a square is under attack by a piece of the given color.
     * @param square The square for which to test for attacks.
     * @param color The attacking color.
     * @return A boolean indicating whether the square is under attack. */
    bool isAttackedBy(Square square, Color color) const;

    bool isInCheck(Color color) const;

    void makeMove(Move move);
    void unmakeMove(Move move);

    void print();
};

}   // namespace chess

#endif