#include "movegen.h"

namespace movegen {

    std::vector<Move> GenerateMoves(Board& board, bb::Color color) {
        // initialize vector to contain moves
        std::vector<Move> moves;

        return moves; 
    }

    // each pawn has three possible move types: single push, double push, and capture
    std::vector<Move> GeneratePawnMoves(Board& board, bb::Color color) {                    // TODO: implement en-passant captures
        
        // get piece bitboards
        bb::U64 their_pieces = board.GetOccupancyBitboard(!color);
        bb::U64 all_pieces = board.GetCombinedOccupancyBitboard();

        // extract pawn locations
        bb::U64 pawns = board.GetPieceBitboard(bb::PAWN, color);

        // define move directions and get the target squares for all possible pawn moves
        bb::U64 single_pushes, double_pushes, right_captures, left_captures;
        bb::Direction forward_dir, right_capture_dir, left_capture_dir;
        if (color == bb::WHITE) {
            // define the move directions for white
            forward_dir = bb::NORTH;
            right_capture_dir = bb::NORTHEAST;
            left_capture_dir = bb::NORTHWEST;
            // get all possible target squares
            single_pushes = bb::ShiftNorth(pawns) & ~all_pieces;
            double_pushes = bb::ShiftNorth(single_pushes & bb::RANK_3_BB) & ~all_pieces;
            right_captures = bb::ShiftNorthEast(pawns) & their_pieces;
            left_captures = bb::ShiftNorthWest(pawns) & their_pieces;
        } else {
            // define the move directions for black
            forward_dir = bb::SOUTH;
            right_capture_dir = bb::SOUTHWEST;
            left_capture_dir = bb::SOUTHEAST;
            // get all possible target squares
            single_pushes = bb::ShiftSouth(pawns) & ~all_pieces;
            double_pushes = bb::ShiftSouth(single_pushes & bb::RANK_6_BB) & ~all_pieces;
            right_captures = bb::ShiftSouthWest(pawns) & their_pieces;
            left_captures = bb::ShiftSouthEast(pawns) & their_pieces;
        }

        // initialize vector to contain moves
        std::vector<Move> moves;

        // lambda function to generate the corresponding move for each target square
        auto GatherMoves {[&moves] (bb::U64 targets, bb::Direction dir, mv::MoveFlag flag) {
            // separate the target bitboard into non-promotion and promotion targets
            bb::U64 promotions = targets & (bb::RANK_1_BB | bb::RANK_8_BB);
            bb::U64 non_promotions = targets - promotions;

            // generate all non-promotion moves
            while(non_promotions) {
                // extract a target square and the corresponding origin square
                int to_square = bb::GetLeastSignificantBitIndex(non_promotions);
                bb::ClearBit(non_promotions, to_square);
                int from_square = to_square - dir;

                // add the move to the moves list
                moves.push_back(Move(from_square, to_square, flag));
            }
            // generate all promotion moves
            while(promotions) {
                // extract a target square and the corresponding origin square
                int to_square = bb::GetLeastSignificantBitIndex(promotions);
                bb::ClearBit(promotions, to_square);
                int from_square = to_square - dir;

                for (mv::MoveFlag promotion_flag : {mv::KNIGHT_PROMOTION, mv::BISHOP_PROMOTION, mv::ROOK_PROMOTION, mv::QUEEN_PROMOTION}) {
                    // combine the existing flag with the promotion flag to conserve the capture bit, and add the move to the moves list
                    moves.push_back(Move(from_square, to_square, flag | promotion_flag));
                }
            }
        }
        };

        // gather all moves based on the identified target squares
        GatherMoves(single_pushes, forward_dir, mv::QUIET);
        GatherMoves(double_pushes, 2 * forward_dir, mv::DOUBLE_PAWN_PUSH);
        GatherMoves(right_captures, right_capture_dir, mv::CAPTURE);
        GatherMoves(left_captures, left_capture_dir, mv::CAPTURE);

        return moves;
    }

    std::vector<Move> GenerateKnightMoves(Board& board, bb::Color color) {

        bb::U64 our_pieces = board.GetOccupancyBitboard(color);
        bb::U64 their_pieces = board.GetOccupancyBitboard(!color);
        
        // get knight locations
        bb::U64 knights = board.GetPieceBitboard(bb::KNIGHT, color);

        // initialize vector to contain moves
        std::vector<Move> moves;
        
        // for each knight, get the attacked squares
        while (knights) {
            // extract one of the knights
            int from_square = bb::GetLeastSignificantBitIndex(knights);
            bb::ClearBit(knights, from_square);

            // lookup the squares which are attacked by this piece
            bb::U64 attacks = attacks::knight_attack_table[from_square];
            
            // remove the attacked squares which are occupied by friendly pieces
            attacks &= ~our_pieces;
            
            // loop through all attacked squares
            while (attacks) {
                int to_square = bb::GetLeastSignificantBitIndex(attacks);
                bb::ClearBit(attacks, to_square);

                // test whether this move captures a piece and set the flag accordingly
                mv::MoveFlag flag = bb::GetBit(their_pieces, to_square) ? mv::CAPTURE : mv::QUIET;

                // add the move to the collection of moves
                Move move = Move(from_square, to_square, flag);
                moves.push_back(move);
            }
        }
        return moves;
    }
}