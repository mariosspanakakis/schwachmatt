#include "attacks.hpp"

namespace attacks {

    bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    bb::U64 knight_attack_table[bb::N_SQUARES];
    bb::U64 king_attack_table[bb::N_SQUARES];
    bb::U64 bishop_xray_attack_table[bb::N_SQUARES];
    bb::U64 rook_xray_attack_table[bb::N_SQUARES];

    bb::U64 get_pawn_attacks(bb::Square square, bb::Side side){
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::set_bit(bitboard, square);
        bb::U64 attacks = 0ULL;

        // white to move
        if (!side){
            attacks |= (
                bb::shift_northwest(bitboard) | bb::shift_northeast(bitboard)
            );
        }
        // black to move
        else{
            attacks |= (
                bb::shift_southwest(bitboard) | bb::shift_southeast(bitboard)
            );
        }
        return attacks;
    }

    bb::U64 get_knight_attacks(bb::Square square){
        // initialize bitboards for knight position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::set_bit(bitboard, square);
        bb::U64 attacks = 0ULL;
        // generate attacks
        attacks |= (
              ((bitboard << 6)  & ~(bb::FILE_G_BB | bb::FILE_H_BB))     // WSW
            | ((bitboard << 10) & ~(bb::FILE_A_BB | bb::FILE_B_BB))     // ESE
            | ((bitboard << 15) & ~(bb::FILE_H_BB))                     // SSW
            | ((bitboard << 17) & ~(bb::FILE_A_BB))                     // SSE
            | ((bitboard >> 6)  & ~(bb::FILE_A_BB | bb::FILE_B_BB))     // ENE
            | ((bitboard >> 10) & ~(bb::FILE_G_BB | bb::FILE_H_BB))     // WNW
            | ((bitboard >> 15) & ~(bb::FILE_A_BB))                     // NNE
            | ((bitboard >> 17) & ~(bb::FILE_H_BB))                     // NNW
        );
        return attacks;
    }

    bb::U64 get_king_attacks(bb::Square square){
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::set_bit(bitboard, square);
        bb::U64 attacks = 0ULL;
        // generate attacks
        attacks |= (
              bb::shift_north(bitboard)
            | bb::shift_northeast(bitboard)
            | bb::shift_east(bitboard)
            | bb::shift_southeast(bitboard)
            | bb::shift_south(bitboard)
            | bb::shift_southwest(bitboard)
            | bb::shift_west(bitboard)
            | bb::shift_northwest(bitboard)
        );
        return attacks;
    }

    bb::U64 get_bishop_xray_attacks(bb::Square square){
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::set_bit(bitboard, square);
        bb::U64 attacks = 0ULL;
        // northeast diagonal
        for (bb::U64 bb = bitboard; (bb & ~bb::FILE_H_BB) != 0; bb <<= 9) {
            attacks |= bb << 9;
        }
        // northwest diagonal
        for (bb::U64 bb = bitboard; (bb & ~bb::FILE_A_BB) != 0; bb <<= 7) {
            attacks |= bb << 7;
        }
        // southeast diagonal
        for (bb::U64 bb = bitboard; (bb & ~bb::FILE_H_BB) != 0; bb >>= 7) {
            attacks |= bb >> 7;
        }
        // southwest diagonal
        for (bb::U64 bb = bitboard; (bb & ~bb::FILE_A_BB) != 0; bb >>= 9) {
            attacks |= bb >> 9;
        }
        // mask out the board's outer squares as these are not required
        attacks &= ~(bb::FILE_A_BB | bb::FILE_H_BB | bb::RANK_1_BB | bb::RANK_8_BB);
        return attacks;
    }

    bb::U64 get_rook_xray_attacks(bb::Square square){
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::set_bit(bitboard, square);
        bb::U64 attacks = 0ULL;
        // north line
        for (bb::U64 bb = bitboard; (bb & ~bb::RANK_7_BB) != 0; bb <<= 8) {
            attacks |= bb << 8;
        }
        // south line
        for (bb::U64 bb = bitboard; (bb & ~bb::RANK_2_BB) != 0; bb >>= 8) {
            attacks |= bb >> 8;
        }
        // east line
        for (bb::U64 bb = bitboard; (bb & ~bb::FILE_H_BB & ~bb::FILE_G_BB) != 0; bb <<= 1) {
            attacks |= bb << 1;
        }
        // west line
        for (bb::U64 bb = bitboard; (bb & ~bb::FILE_A_BB & ~bb::FILE_B_BB) != 0; bb >>= 1) {
            attacks |= bb >> 1;
        }
        return attacks;
    }

    void initialize_attack_tables(){
        // generate pawn attack table
        for (int side=0; side<bb::N_COLORS; ++side){
            for (int square = 0; square < bb::N_SQUARES; square++){
                pawn_attack_table[side][square] = get_pawn_attacks(square, side);
            }
        }
        // generate knight and king attack table
        for (int square = 0; square < bb::N_SQUARES; square++){
            knight_attack_table[square] = get_knight_attacks(square);
            king_attack_table[square] = get_king_attacks(square);
        }

        // generate X-ray attack tables for sliding pieces
        for (int square = 0; square < bb::N_SQUARES; square++){
            bishop_xray_attack_table[square] = get_bishop_xray_attacks(square);
            rook_xray_attack_table[square] = get_rook_xray_attacks(square);
        }
    }
}