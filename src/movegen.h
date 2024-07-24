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
 * @return Pointer to the last element of the move list. */
template <Color TColor>
Move* generateMoves(const Board& board, Move* movelist);

/* @brief Find all possible pseudo-legal moves for a given piece type on the
        board.
 * @tparam TColor The side to move.
 * @param board A given chess board.
 * @param piece Piece to calculate moves for, can be any piece except for
 *              the pawn, since pawn moves are generated separately.
 * @param movelist Reference to move list that the moves are stored in.
 * @return Pointer to the last element of the move list. */
template <Color TColor>
Move* generatePieceMoves(const Board& board, Piece piece, Move* movelist);

/* @brief Find all possible pseudo-legal pawn moves on the board.
 * @tparam TColor The side to move.
 * @param board A given chess board.
 * @param movelist Reference to move list that the moves are stored in.
 * @return Pointer to the last element of the move list. */
template <Color TColor>
Move* generatePawnMoves(const Board& board, Move* movelist);

template <Color TColor>
Move* generateCastlingMoves(const Board& board, Move* movelist);

}   // namespace movegen


template <Color TColor>
struct MoveList {

    private:
    Move m_moves[MAX_NUMBER_OF_MOVES];
    Move* m_last;

    public:
    /* Constructor for an empty move list. */
    MoveList() : m_last(m_moves) {}
    /* Constructor to conveniently generate a move list from a given board. */
    explicit MoveList(const Board& board) : m_last(movegen::generateMoves<TColor>(board, m_moves)) {}
    
    const Move* begin() const { return m_moves; }
    const Move* end() const { return m_last; }
    size_t size() const { return m_last - m_moves; }
    bool contains(Move move) const {
        return std::find(begin(), end(), move) != end();
    }
};

}   // namespace chess

#endif