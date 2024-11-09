#include "movegen.h"

namespace chess {

template <Color TColor>
static Move* generateAllMoves(const Board& board, Move* movelist);

template <Color TColor>
static Move* generatePawnMoves(const Board& board, Move* movelist);

template <Color TColor, PieceType TPieceType>
static Move* generatePieceMoves(const Board& board, Move* movelist);

template <Color TColor>
static Move* generateCastlingMoves(const Board& board, Move* movelist);


Move* generate(const Board& board, Move* movelist) {
    Color us = board.getSideToMove();
    return (us == WHITE) ? generateAllMoves<WHITE>(board, movelist)
                         : generateAllMoves<BLACK>(board, movelist);
}

template <Color TColor>
static Move* generateAllMoves(const Board& board, Move* movelist) {
    
    movelist = generatePawnMoves<TColor>(board, movelist);
    movelist = generatePieceMoves<TColor, KNIGHT>(board, movelist);
    movelist = generatePieceMoves<TColor, BISHOP>(board, movelist);
    movelist = generatePieceMoves<TColor, ROOK>(board, movelist);
    movelist = generatePieceMoves<TColor, QUEEN>(board, movelist);
    movelist = generatePieceMoves<TColor, KING>(board, movelist);

    return movelist;
}

template <Color TColor, PieceType TPieceType>
static Move* generatePieceMoves(const Board& board, Move* movelist) {

    assert(TPieceType != PAWN);  // pawn move generation is handled separately

    // get piece locations
    Bitboard our_pieces = board.getColorOccupancy(TColor);
    Bitboard their_pieces = board.getColorOccupancy(!TColor);
    Bitboard all_pieces = board.getTotalOccupancy();
    Bitboard pieces = board.getPieceOccupancy(TPieceType, TColor);
    
    // for each piece, get the attacked squares
    while (pieces) {
        // extract one of the pieces
        Square from = bb::pop_lsb(pieces);

        // lookup the squares which are attacked by this piece
        Bitboard attacks = attacks::getPieceAttacks<TPieceType>(from, all_pieces);

        // remove the attacked squares which are occupied by friendly pieces
        attacks &= ~our_pieces;

        // loop through all attacked squares
        while (attacks) {
            Square to = bb::pop_lsb(attacks);

            // test whether this move captures a piece and set the flag accordingly
            MoveFlag flag = bb::getBit(their_pieces, to) ? CAPTURE : QUIET;

            // add the move to the list of moves
            *movelist++ = Move(from, to, flag);
        }
    }

    // add castling moves for the king
    if (TPieceType == KING) {
        movelist = generateCastlingMoves<TColor>(board, movelist);
    }

    return movelist;
}

template <Color TColor>
static Move* generatePawnMoves(const Board& board, Move* movelist) {
    
    // get piece bitboards
    Bitboard theirPieces = board.getColorOccupancy(!TColor);
    Bitboard allPieces = board.getTotalOccupancy();
    Bitboard pawns = board.getPieceOccupancy(PAWN, TColor);

    // in dependence of the color, define the move directions and the double push rank
    constexpr Direction forward_dir = (TColor == WHITE) ? NORTH : SOUTH;
    constexpr Direction right_capture_dir = (TColor == WHITE) ? NORTHEAST : SOUTHWEST;
    constexpr Direction left_capture_dir = (TColor == WHITE) ? NORTHWEST : SOUTHEAST;
    constexpr Bitboard double_push_rank = (TColor == WHITE) ? RANK_3_BB : RANK_6_BB;
    // get all possible target squares
    Bitboard single_pushes = (bb::shift<forward_dir>(pawns) & ~allPieces);
    Bitboard double_pushes = (bb::shift<forward_dir>(single_pushes & double_push_rank) & ~allPieces);
    Bitboard right_captures = bb::shift<right_capture_dir>(pawns);
    Bitboard left_captures = bb::shift<left_capture_dir>(pawns);

    // lambda function to generate the corresponding move for each target square
    auto GatherMoves {[&movelist] (Bitboard targets, Direction dir, MoveFlag flag) {
        // separate the target bitboard into non-promotion and promotion targets
        Bitboard promotions = targets & (RANK_1_BB | RANK_8_BB);
        Bitboard non_promotions = targets - promotions;

        // generate all non-promotion moves
        while(non_promotions) {
            // extract a target square and the corresponding origin square
            Square to_square = bb::pop_lsb(non_promotions);
            Square from_square = to_square - dir;

            // add the move to the moves list
            *movelist++ = Move(from_square, to_square, flag);
        }
        // generate all promotion moves
        while(promotions) {
            // extract a target square and the corresponding origin square
            Square to_square = bb::pop_lsb(promotions);
            Square from_square = to_square - dir;

            // generate a separate move for promoting to any possible piece
            for (MoveFlag promotion_flag : {KNIGHT_PROMOTION, BISHOP_PROMOTION, ROOK_PROMOTION, QUEEN_PROMOTION}) {
                // combine the existing flag with the promotion flag to conserve the capture bit
                *movelist++ = Move(from_square, to_square, flag | promotion_flag);
            }
        }
    }
    };

    // gather all moves based on the identified target squares
    GatherMoves(single_pushes & ~allPieces, forward_dir, QUIET);
    GatherMoves(double_pushes & ~allPieces, 2 * forward_dir, DOUBLE_PAWN_PUSH);
    GatherMoves(right_captures & theirPieces, right_capture_dir, CAPTURE);
    GatherMoves(left_captures & theirPieces, left_capture_dir, CAPTURE);

    // check for possible en-passant captures
    Bitboard en_passant_target = board.getCurrentEnPassantTarget();
    if (en_passant_target) {
        int to_square = bb::lsb(en_passant_target);
        if (right_captures & en_passant_target) {
            int from_square = to_square - right_capture_dir;
            *movelist++ = Move(from_square, to_square, EN_PASSANT_CAPTURE);
        }
        if (left_captures & en_passant_target) {
            int from_square = to_square - left_capture_dir;
            *movelist++ = Move(from_square, to_square, EN_PASSANT_CAPTURE);
        }
    }

    return movelist;
}

template <Color TColor>
static Move* generateCastlingMoves(const Board& board, Move* movelist) {        // So far, will check for legality. This should however be moved to a board.isLegal(move) function

    if (board.canCastle(TColor & ANY_CASTLING)) {
        for (const CastlingRight cr : {TColor & KINGSIDE_CASTLING, TColor & QUEENSIDE_CASTLING}) {
            if (board.canCastle(cr) && !board.isCastlingBlocked(cr)) {
                Square from = board.getKingSquare(TColor);
                Square to = CASTLING_KING_GOAL_SQUARE[cr];
                // check if any square the king must pass is under attack
                Direction dir = to > from ? EAST : WEST;
                bool allowed = true;
                for (Square sq = from; sq != to + dir; sq += dir) {
                    if (board.isAttackedBy(sq, !TColor)) {
                        allowed = false;
                    }
                }
                if (allowed) {
                    if ((int)cr & (int)KINGSIDE_CASTLING) {
                        *movelist++ = Move(from, to, KINGSIDE_CASTLE);
                    } else {
                        *movelist++ = Move(from, to, QUEENSIDE_CASTLE);
                    }
                }
            }
        }
    }

    return movelist;
}

/* Explicit template instantiations. */
template Move* generateAllMoves<WHITE>(const Board& board, Move* movelist);
template Move* generateAllMoves<BLACK>(const Board& board, Move* movelist);

}   // namespace chess