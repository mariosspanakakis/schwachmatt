#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "board.h"
#include "attacks.h"
#include "move.h"

namespace chess {

/* @brief Generate all possible pseudo-legal moves for the given board. The
          moves are generated for the currently active side. This function can
          be called directly, but is intended to be called from the constructor
          of the move list itself.
 * @param board A given chess board.
 * @param movelist A pointer to an existing move list.
 * @return A pointer to the last generated move in the list. */
Move* generate(const Board& board, Move* movelist);

/* A list of moves, generated for a given board representation. The move
   generation is called from the constructor of this struct. */
struct MoveList {
    private:
    Move m_moves[MAX_NUMBER_OF_MOVES];
    Move* m_last;

    public:
    /* Constructor to conveniently generate a move list from a given board. */
    explicit MoveList(const Board& board) : m_last(generate(board, m_moves)) {}

    Move& operator[](size_t index) {
        return m_moves[index];
    }
    
    const Move* begin() const { return m_moves; }
    const Move* end() const { return m_last; }
    size_t size() const { return m_last - m_moves; }
};

}   // namespace chess

#endif