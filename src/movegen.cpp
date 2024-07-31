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
        int from = bb::getLeastSignificantBitIndex(pieces);
        bb::clearBit(pieces, from);

        // lookup the squares which are attacked by this piece
        Bitboard attacks = attacks::getPieceAttacks<TPieceType>(from, all_pieces);

        // remove the attacked squares which are occupied by friendly pieces
        attacks &= ~our_pieces;

        // loop through all attacked squares
        while (attacks) {
            int to = bb::getLeastSignificantBitIndex(attacks);
            bb::clearBit(attacks, to);

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

    // define the move directions
    constexpr Direction forward_dir = (TColor == WHITE) ? NORTH : SOUTH;
    constexpr Direction right_capture_dir = (TColor == WHITE) ? NORTHEAST : SOUTHWEST;
    constexpr Direction left_capture_dir = (TColor == WHITE) ? NORTHWEST : SOUTHEAST;
    // define the double push rank
    Bitboard double_push_rank = (TColor == WHITE) ? bb::RANK_3_BB : bb::RANK_6_BB;
    // get all possible target squares
    Bitboard single_pushes = (bb::shift<forward_dir>(pawns) & ~allPieces);
    Bitboard double_pushes = (bb::shift<forward_dir>(single_pushes & double_push_rank) & ~allPieces);
    Bitboard right_captures = bb::shift<right_capture_dir>(pawns);
    Bitboard left_captures = bb::shift<left_capture_dir>(pawns);

    // lambda function to generate the corresponding move for each target square
    auto GatherMoves {[&movelist] (Bitboard targets, Direction dir, MoveFlag flag) {
        // separate the target bitboard into non-promotion and promotion targets
        Bitboard promotions = targets & (bb::RANK_1_BB | bb::RANK_8_BB);
        Bitboard non_promotions = targets - promotions;

        // generate all non-promotion moves
        while(non_promotions) {
            // extract a target square and the corresponding origin square
            int to_square = bb::getLeastSignificantBitIndex(non_promotions);
            bb::clearBit(non_promotions, to_square);
            int from_square = to_square - dir;

            // add the move to the moves list
            *movelist++ = Move(from_square, to_square, flag);
        }
        // generate all promotion moves
        while(promotions) {
            // extract a target square and the corresponding origin square
            int to_square = bb::getLeastSignificantBitIndex(promotions);
            bb::clearBit(promotions, to_square);
            int from_square = to_square - dir;

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
        int to_square = bb::getLeastSignificantBitIndex(en_passant_target);
        MoveFlag en_passant_flag = EN_PASSANT_CAPTURE;
        if (right_captures & en_passant_target) {
            int from_square = to_square - right_capture_dir;
            *movelist++ = Move(from_square, to_square, en_passant_flag);
        }
        if (left_captures & en_passant_target) {
            int from_square = to_square - left_capture_dir;
            *movelist++ = Move(from_square, to_square, en_passant_flag);
        }
    }

    return movelist;
}

template <Color TColor>
static Move* generateCastlingMoves(const Board& board, Move* movelist) {
    // prerequisites for castling:
    // - castling generally permitted (king and rook have not been moved or captured)
    // - no pieces on the squares between king and rook
    // - the squares between king and rook are not under attack
    Bitboard all_pieces = board.getTotalOccupancy();
    if (TColor == WHITE) {                                                      // TODO: castling is handled inefficiently both here and during move making/unmaking
        // test whether the relevant squares are under attack (king square + the squares the king passes over)
        // WHITE_KINGSIDE: E1, F1, G1
        // WHITE_QUEENSIDE: E1, D1, C1
        // BLACK_KINGSIDE: E8, F8, G8
        // BLACK_QUEENSIDE: E8, D8, C8
        if (board.canCastle(WHITE, WHITE_KINGSIDE_CASTLING)
            && ((all_pieces & bb::WHITE_KINGSIDE_CASTLE_SQUARES) == 0)) {
            bool allowed = true;
            for (Square square : {E1, F1, G1}) {
                if (board.isAttackedBy(square, BLACK)) {
                    allowed = false;
                }
            }
            if (allowed) {
                *movelist++ = Move(E1, G1, KINGSIDE_CASTLE);
            }
        }
        if (board.canCastle(WHITE, WHITE_QUEENSIDE_CASTLING)
            && ((all_pieces & bb::WHITE_QUEENSIDE_CASTLE_SQUARES) == 0)) {
            
            bool allowed = true;
            for (Square square : {E1, D1, C1}) {
                if (board.isAttackedBy(square, BLACK)) {
                    allowed = false;
                }
            }
            if (allowed) {
                *movelist++ = Move(E1, C1, QUEENSIDE_CASTLE);
            }
        }
    } else {
        if (board.canCastle(BLACK, BLACK_KINGSIDE_CASTLING)
            && ((all_pieces & bb::BLACK_KINGSIDE_CASTLE_SQUARES) == 0)) {
            
            bool allowed = true;
            for (Square square : {E8, F8, G8}) {
                if (board.isAttackedBy(square, WHITE)) {
                    allowed = false;
                }
            }
            if (allowed) {
                *movelist++ = Move(E8, G8, KINGSIDE_CASTLE);
            }
        }
        if (board.canCastle(BLACK, BLACK_QUEENSIDE_CASTLING)
            && ((all_pieces & bb::BLACK_QUEENSIDE_CASTLE_SQUARES) == 0)) {
            
            bool allowed = true;
            for (Square square : {E8, D8, C8}) {
                if (board.isAttackedBy(square, WHITE)) {
                    allowed = false;
                }
            }
            if (allowed) {
                *movelist++ = Move(E8, C8, QUEENSIDE_CASTLE);
            }
        }
    }

    return movelist;
}

/* Explicit template instantiations. */
template Move* generateAllMoves<WHITE>(const Board& board, Move* movelist);
template Move* generateAllMoves<BLACK>(const Board& board, Move* movelist);

}   // namespace chess