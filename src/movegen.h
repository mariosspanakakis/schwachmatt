#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "board.h"
#include "attacks.h"
#include "move.h"

namespace movegen {
    /* @brief Find all possible pseudo-legal moves for all pieces on the board.
     * @param board A given chess board.
     * @param color Side to move.
     * @return A list of pseudo-legal moves that can be made on the given
     *         board. */
    MoveList GenerateMoves(Board& board, bb::Color color);
    
    /* @brief Find all possible pseudo-legal moves for a given piece type on the
              board.
     * @param board A given chess board.
     * @param piece Piece to calculate moves for, can be any piece except for
     *              the pawn, since pawn moves are generated separately.
     * @param color Side to move.
     * @param movelist Reference to move list that the moves are stored in. */
    void GeneratePieceMoves(Board& board, bb::Piece piece, bb::Color color, MoveList &movelist);
    
    /* @brief Find all possible pseudo-legal pawn moves on the board.
     * @param board A given chess board.
     * @param color Side to move.
     * @param movelist Reference to move list that the moves are stored in. */
    void GeneratePawnMoves(Board& board, bb::Color color, MoveList &movelist);

    void GenerateCastlingMoves(Board& board, bb::Color color, MoveList &movelist);
}

#endif