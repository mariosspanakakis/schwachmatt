#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "board.h"
#include "attacks.h"
#include "move.h"

namespace chess {
namespace movegen {

/* @brief Find all possible pseudo-legal moves for all pieces on the board.
 * @tparam TColor The side to move.
 * @param board A given chess board.
 * @return A list of pseudo-legal moves that can be made on the given board. */
template <Color TColor>
MoveList generateMoves(Board& board);

/* @brief Find all possible pseudo-legal moves for a given piece type on the
        board.
 * @tparam TColor The side to move.
 * @param board A given chess board.
 * @param piece Piece to calculate moves for, can be any piece except for
 *              the pawn, since pawn moves are generated separately.
 * @param movelist Reference to move list that the moves are stored in. */
template <Color TColor>
void generatePieceMoves(Board& board, Piece piece, MoveList &movelist);

/* @brief Find all possible pseudo-legal pawn moves on the board.
 * @tparam TColor The side to move.
 * @param board A given chess board.
 * @param movelist Reference to move list that the moves are stored in. */
template <Color TColor>
void generatePawnMoves(Board& board, MoveList &movelist);

template <Color TColor>
void generateCastlingMoves(Board& board, MoveList &movelist);

}   // namespace movegen
}   // namespace chess

#endif