#include "movegen.h"

namespace movegen {

    MoveList generateMoves(Board& board, bb::Color color) {
        // initialize move list
        MoveList movelist;
        // add moves for all pieces
        for (bb::Piece piece : {bb::PAWN, bb::KNIGHT, bb::BISHOP, bb::ROOK, bb::QUEEN, bb::KING}) {
            generatePieceMoves(board, piece, color, movelist);
        }
        // return the list of all possible moves
        return movelist; 
    }

    void generatePieceMoves(Board& board, bb::Piece piece, bb::Color color, MoveList &movelist) {
        // for pawns, call the separate pawn move generation function
        if (piece == bb::PAWN) {
            generatePawnMoves(board, color, movelist);
            return;
        }

        // get piece locations
        bb::U64 our_pieces = board.getOccupancyBitboard(color);
        bb::U64 their_pieces = board.getOccupancyBitboard(!color);
        bb::U64 all_pieces = board.getCombinedOccupancyBitboard();
        bb::U64 pieces = board.getPieceBitboard(piece, color);
        
        // for each piece, get the attacked squares
        while (pieces) {
            // extract one of the pieces
            int from_square = bb::getLeastSignificantBitIndex(pieces);
            bb::clearBit(pieces, from_square);

            // lookup the squares which are attacked by this piece
            bb::U64 attacks;
            switch (piece) {
                case bb::KNIGHT:
                    attacks = attacks::knightAttackTable[from_square];
                    break;
                case bb::BISHOP:
                    attacks = attacks::lookupBishopAttacks(from_square, all_pieces);
                    break;
                case bb::ROOK:
                    attacks = attacks::lookupRookAttacks(from_square, all_pieces);
                    break;
                case bb::QUEEN:
                    attacks = (
                        attacks::lookupBishopAttacks(from_square, all_pieces)
                        | attacks::lookupRookAttacks(from_square, all_pieces)
                    );
                    break;
                case bb::KING:
                    attacks = attacks::kingAttackTable[from_square];
                    break;
            }

            // remove the attacked squares which are occupied by friendly pieces
            attacks &= ~our_pieces;

            // loop through all attacked squares
            while (attacks) {
                int to_square = bb::getLeastSignificantBitIndex(attacks);
                bb::clearBit(attacks, to_square);

                // test whether this move captures a piece and set the flag accordingly
                mv::MoveFlag flag = bb::getBit(their_pieces, to_square) ? mv::CAPTURE : mv::QUIET;

                // add the move to the list of moves
                movelist.add(Move(from_square, to_square, flag));
            }
        }

        // add castling moves for the king
        if (piece == bb::KING) {
            generateCastlingMoves(board, color, movelist);
        }
    }
    
    void generatePawnMoves(Board& board, bb::Color color, MoveList &movelist) {
        
        // get piece bitboards
        bb::U64 their_pieces = board.getOccupancyBitboard(!color);
        bb::U64 all_pieces = board.getCombinedOccupancyBitboard();

        // extract pawn locations
        bb::U64 pawns = board.getPieceBitboard(bb::PAWN, color);

        // rename color indicator for convenience and readability
        bool is_white = (color == bb::WHITE);

        // define the move directions
        bb::Direction forward_dir = is_white ? bb::NORTH : bb::SOUTH;
        bb::Direction right_capture_dir = is_white ? bb::NORTHEAST : bb::SOUTHWEST;
        bb::Direction left_capture_dir = is_white ? bb::NORTHWEST : bb::SOUTHEAST;
        // get all possible target squares
        bb::U64 single_pushes = ((is_white ? bb::shiftNorth(pawns) : bb::shiftSouth(pawns)) & ~all_pieces);
        bb::U64 double_pushes = ((is_white ? bb::shiftNorth(single_pushes & bb::RANK_3_BB) : bb::shiftSouth(single_pushes & bb::RANK_6_BB)) & ~all_pieces);
        bb::U64 right_captures = is_white ? bb::shiftNorthEast(pawns) : bb::shiftSouthWest(pawns);
        bb::U64 left_captures = is_white ? bb::shiftNorthWest(pawns) : bb::shiftSouthEast(pawns);

        // lambda function to generate the corresponding move for each target square
        auto GatherMoves {[&movelist] (bb::U64 targets, bb::Direction dir, mv::MoveFlag flag) {
            // separate the target bitboard into non-promotion and promotion targets
            bb::U64 promotions = targets & (bb::RANK_1_BB | bb::RANK_8_BB);
            bb::U64 non_promotions = targets - promotions;

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
                for (mv::MoveFlag promotion_flag : {mv::KNIGHT_PROMOTION, mv::BISHOP_PROMOTION, mv::ROOK_PROMOTION, mv::QUEEN_PROMOTION}) {
                    // combine the existing flag with the promotion flag to conserve the capture bit, and add the move to the moves list
                    movelist.add(Move(from_square, to_square, flag | promotion_flag));
                }
            }
        }
        };

        // gather all moves based on the identified target squares
        GatherMoves(single_pushes & ~all_pieces, forward_dir, mv::QUIET);
        GatherMoves(double_pushes & ~all_pieces, 2 * forward_dir, mv::DOUBLE_PAWN_PUSH);
        GatherMoves(right_captures & their_pieces, right_capture_dir, mv::CAPTURE);
        GatherMoves(left_captures & their_pieces, left_capture_dir, mv::CAPTURE);

        // check for possible en-passant captures
        bb::U64 en_passant_target = board.getCurrentEnPassantTarget();
        if (en_passant_target) {
            bb::printBitboard(en_passant_target);
            int to_square = bb::getLeastSignificantBitIndex(en_passant_target);
            mv::MoveFlag en_passant_flag = mv::EN_PASSANT_CAPTURE;
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

    void generateCastlingMoves(Board& board, bb::Color color, MoveList &movelist) {
        // prerequisites for castling:
        // - castling generally permitted (king and rook have not been moved)
        // - no pieces on the squares between king and rook
        // - the squares between king and rook are not under attack             -> this is tested during legality test
        bb::U64 all_pieces = board.getCombinedOccupancyBitboard();
        if (color == bb::WHITE) {
            if (board.getCastlingRight(WHITE_KINGSIDE_CASTLE)
                && ((all_pieces & bb::WHITE_KINGSIDE_CASTLE_SQUARES) == 0)) {
                    movelist.add(Move(bb::E1, bb::G1, mv::KINGSIDE_CASTLE));
            }
            if (board.getCastlingRight(WHITE_QUEENSIDE_CASTLE)
                && ((all_pieces & bb::WHITE_QUEENSIDE_CASTLE_SQUARES) == 0)) {
                    movelist.add(Move(bb::E1, bb::C1, mv::QUEENSIDE_CASTLE));
            }
        } else {
            if (board.getCastlingRight(BLACK_KINGSIDE_CASTLE)
                && ((all_pieces & bb::BLACK_KINGSIDE_CASTLE_SQUARES) == 0)) {
                    movelist.add(Move(bb::E8, bb::G8, mv::KINGSIDE_CASTLE));
            }
            if (board.getCastlingRight(BLACK_QUEENSIDE_CASTLE)
                && ((all_pieces & bb::BLACK_QUEENSIDE_CASTLE_SQUARES) == 0)) {
                    movelist.add(Move(bb::E8, bb::C8, mv::QUEENSIDE_CASTLE));
            }
        }
    }
}