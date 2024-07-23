#include "movegen.h"

namespace chess {
namespace movegen {

template <Color TColor>
MoveList generateMoves(Board& board) {
    // initialize move list
    MoveList movelist;
    // add moves for all pieces
    for (Piece piece : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}) {
        generatePieceMoves<TColor>(board, piece, movelist);
    }
    // return the list of all possible moves
    return movelist;
}

template <Color TColor>
void generatePieceMoves(Board& board, Piece piece, MoveList &movelist) {
    // for pawns, call the separate pawn move generation function
    if (piece == PAWN) {
        generatePawnMoves<TColor>(board, movelist);
        return;
    }

    // get piece locations
    Bitboard our_pieces = board.getOccupancyBitboard(TColor);
    Bitboard their_pieces = board.getOccupancyBitboard(!TColor);
    Bitboard all_pieces = board.getCombinedOccupancyBitboard();
    Bitboard pieces = board.getPieceBitboard(piece, TColor);
    
    // for each piece, get the attacked squares
    while (pieces) {
        // extract one of the pieces
        int from_square = bb::getLeastSignificantBitIndex(pieces);
        bb::clearBit(pieces, from_square);

        // lookup the squares which are attacked by this piece
        Bitboard attacks;
        switch (piece) {
            case KNIGHT:
                attacks = attacks::KNIGHT_ATTACKS[from_square];
                break;
            case BISHOP:
                attacks = attacks::lookupBishopAttacks(from_square, all_pieces);
                break;
            case ROOK:
                attacks = attacks::lookupRookAttacks(from_square, all_pieces);
                break;
            case QUEEN:
                attacks = (
                    attacks::lookupBishopAttacks(from_square, all_pieces)
                    | attacks::lookupRookAttacks(from_square, all_pieces)
                );
                break;
            case KING:
                attacks = attacks::KING_ATTACKS[from_square];
                break;
        }

        // remove the attacked squares which are occupied by friendly pieces
        attacks &= ~our_pieces;

        // loop through all attacked squares
        while (attacks) {
            int to_square = bb::getLeastSignificantBitIndex(attacks);
            bb::clearBit(attacks, to_square);

            // test whether this move captures a piece and set the flag accordingly
            MoveFlag flag = bb::getBit(their_pieces, to_square) ? CAPTURE : QUIET;

            // add the move to the list of moves
            movelist.add(Move(from_square, to_square, flag));
        }
    }

    // add castling moves for the king
    if (piece == KING) {
        generateCastlingMoves<TColor>(board, movelist);
    }
}

template <Color TColor>
void generatePawnMoves(Board& board, MoveList &movelist) {
    
    // get piece bitboards
    Bitboard theirPieces = board.getOccupancyBitboard(!TColor);
    Bitboard allPieces = board.getCombinedOccupancyBitboard();
    Bitboard pawns = board.getPieceBitboard(PAWN, TColor);

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
            movelist.add(Move(from_square, to_square, flag));
        }
        // generate all promotion moves
        while(promotions) {
            // extract a target square and the corresponding origin square
            int to_square = bb::getLeastSignificantBitIndex(promotions);
            bb::clearBit(promotions, to_square);
            int from_square = to_square - dir;

            // generate a separate move for promoting to any possible piece
            for (MoveFlag promotion_flag : {KNIGHT_PROMOTION, BISHOP_PROMOTION, ROOK_PROMOTION, QUEEN_PROMOTION}) {
                // combine the existing flag with the promotion flag to conserve the capture bit, and add the move to the moves list
                movelist.add(Move(from_square, to_square, flag | promotion_flag));
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
        bb::printBitboard(en_passant_target);
        int to_square = bb::getLeastSignificantBitIndex(en_passant_target);
        MoveFlag en_passant_flag = EN_PASSANT_CAPTURE;
        if (right_captures & en_passant_target) {
            int from_square = to_square - right_capture_dir;
            movelist.add(Move(from_square, to_square, en_passant_flag));
        }
        if (left_captures & en_passant_target) {
            int from_square = to_square - left_capture_dir;
            movelist.add(Move(from_square, to_square, en_passant_flag));
        }
    }
}

template <Color TColor>
void generateCastlingMoves(Board& board, MoveList &movelist) {
    // prerequisites for castling:
    // - castling generally permitted (king and rook have not been moved)
    // - no pieces on the squares between king and rook
    // - the squares between king and rook are not under attack             -> this is tested during legality test
    Bitboard all_pieces = board.getCombinedOccupancyBitboard();
    if (TColor == WHITE) {
        if (board.getCastlingRight(WHITE_KINGSIDE_CASTLE)
            && ((all_pieces & bb::WHITE_KINGSIDE_CASTLE_SQUARES) == 0)) {
                movelist.add(Move(E1, G1, KINGSIDE_CASTLE));
        }
        if (board.getCastlingRight(WHITE_QUEENSIDE_CASTLE)
            && ((all_pieces & bb::WHITE_QUEENSIDE_CASTLE_SQUARES) == 0)) {
                movelist.add(Move(E1, C1, QUEENSIDE_CASTLE));
        }
    } else {
        if (board.getCastlingRight(BLACK_KINGSIDE_CASTLE)
            && ((all_pieces & bb::BLACK_KINGSIDE_CASTLE_SQUARES) == 0)) {
                movelist.add(Move(E8, G8, KINGSIDE_CASTLE));
        }
        if (board.getCastlingRight(BLACK_QUEENSIDE_CASTLE)
            && ((all_pieces & bb::BLACK_QUEENSIDE_CASTLE_SQUARES) == 0)) {
                movelist.add(Move(E8, C8, QUEENSIDE_CASTLE));
        }
    }
}

/* Explicit template instantiations. */
template MoveList generateMoves<WHITE>(Board& board);
template MoveList generateMoves<BLACK>(Board& board);

}   // namespace movegen
}   // namespace chess