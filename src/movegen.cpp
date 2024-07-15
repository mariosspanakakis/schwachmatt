#include "movegen.h"

namespace movegen {

    void GenerateMoves(Board& board, bb::Color color) {

    }

    // each pawn has three possible move types: single push, double push, and capture
    void GeneratePawnMoves(Board& board, bb::Color color) {
        // TODO: implement pawn move generation
        //bb::U64 pieces = board.GetPieceBitboard(bb::PAWN, color);
    }

    void GenerateKnightMoves(Board& board, bb::Color color) {

        bb::U64 our_pieces = board.GetOccupancyBitboard(color);
        bb::U64 their_pieces = board.GetOccupancyBitboard(color ^ 1);
        
        // get knight locations
        bb::U64 pieces = board.GetPieceBitboard(bb::KNIGHT, color);
        
        // for each knight, get the attacked squares
        while (pieces) {
            // extract one of the knights
            int from_square = bb::GetLeastSignificantBitIndex(pieces);
            bb::ClearBit(pieces, from_square);

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

                // generate a move object
                Move move = Move(from_square, to_square, flag);
                move.PrintMoveDetails();
            }
        }
    }
}