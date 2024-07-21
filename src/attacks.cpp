#include "attacks.h"

namespace attacks {

    bb::U64 pawn_attack_table[bb::N_COLORS][bb::N_SQUARES];
    bb::U64 knight_attack_table[bb::N_SQUARES];
    bb::U64 king_attack_table[bb::N_SQUARES];
    bb::U64 bishop_attack_table[bb::N_SQUARES][4096];
    bb::U64 rook_attack_table[bb::N_SQUARES][4096];

    bb::U64 bishop_attack_mask[bb::N_SQUARES];
    bb::U64 rook_attack_mask[bb::N_SQUARES];

    bb::U64 bishop_magics[bb::N_SQUARES];
    bb::U64 rook_magics[bb::N_SQUARES];

    void InitializeAttackTables() {
        
        // loop through all board squares and precalculate attack maps
        for (int square = 0; square < bb::N_SQUARES; square++) {
            
            // generate attack maps for all leaping pieces
            for (int color = bb::WHITE; color < bb::N_COLORS; ++color){
                pawn_attack_table[color][square] = GetPawnAttacks(square, color);
            }
            knight_attack_table[square] = GetKnightAttacks(square);
            king_attack_table[square] = GetKingAttacks(square);

            // generate attack masks for bishop and rook
            bishop_attack_mask[square] = GetBishopAttacks(square) & ~bb::EDGE_BB;           // NOTE: treat this with care, what about the rook attack mask?
            rook_attack_mask[square] = GetRookAttacks(square);

            // generate magic attack tables for the bishops and rooks
            InitializeMagicAttack(square, true);
            InitializeMagicAttack(square, false);

            // TODO: generate queen attacks
        }
    }

    bb::U64 GetPawnAttacks(bb::Square square, bb::Color color) {
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::SetBit(bitboard, square);
        bb::U64 attacks = 0ULL;

        if (color == bb::WHITE) {
            attacks |= (
                bb::ShiftNorthWest(bitboard) | bb::ShiftNorthEast(bitboard)
            );
        } else if (color == bb::BLACK) {
            attacks |= (
                bb::ShiftSouthWest(bitboard) | bb::ShiftSouthEast(bitboard)
            );
        }
        return attacks;
    }

    bb::U64 GetKnightAttacks(bb::Square square) {
        // initialize bitboards for knight position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::SetBit(bitboard, square);
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

    bb::U64 GetKingAttacks(bb::Square square) {
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::SetBit(bitboard, square);
        bb::U64 attacks = 0ULL;
        // generate attacks
        attacks |= (
              bb::ShiftNorth(bitboard)
            | bb::ShiftNorthEast(bitboard)
            | bb::ShiftEast(bitboard)
            | bb::ShiftSouthEast(bitboard)
            | bb::ShiftSouth(bitboard)
            | bb::ShiftSouthWest(bitboard)
            | bb::ShiftWest(bitboard)
            | bb::ShiftNorthWest(bitboard)
        );
        return attacks;
    }

    bb::U64 GetBishopAttacks(bb::Square square, bb::U64 blockers) {
        // initialize bitboards for bishop position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::SetBit(bitboard, square);
        bb::U64 attacks = 0ULL;
        bb::U64 bb;
        // northeast diagonal
        for (bb = bitboard; (bb & ~bb::FILE_H_BB) != 0; bb <<= 9) {
            bb::U64 attacked_square = bb << 9;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // northwest diagonal
        for (bb = bitboard; (bb & ~bb::FILE_A_BB) != 0; bb <<= 7) {
            bb::U64 attacked_square = bb << 7;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // southeast diagonal
        for (bb = bitboard; (bb & ~bb::FILE_H_BB) != 0; bb >>= 7) {
            bb::U64 attacked_square = bb >> 7;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // southwest diagonal
        for (bb = bitboard; (bb & ~bb::FILE_A_BB) != 0; bb >>= 9) {
            bb::U64 attacked_square = bb >> 9;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        return attacks;
    }

    bb::U64 GetRookAttacks(bb::Square square, bb::U64 blockers) {
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::SetBit(bitboard, square);
        bb::U64 attacks = 0ULL;
        bb::U64 bb;
        // north line
        for (bb::U64 bb = bitboard; (bb & ~bb::RANK_7_BB) != 0; bb <<= 8) {
            bb::U64 attacked_square = bb << 8;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // south line
        for (bb = bitboard; (bb & ~bb::RANK_2_BB) != 0; bb >>= 8) {
            bb::U64 attacked_square = bb >> 8;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // east line
        for (bb = bitboard; (bb & ~bb::FILE_H_BB & ~bb::FILE_G_BB) != 0; bb <<= 1) {
            bb::U64 attacked_square = bb << 1;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // west line
        for (bb = bitboard; (bb & ~bb::FILE_A_BB & ~bb::FILE_B_BB) != 0; bb >>= 1) {
            bb::U64 attacked_square = bb >> 1;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        return attacks;
    }

    bb::U64 LookupBishopAttacks(bb::Square square, bb::U64 blockers) {
        // get attack mask and mask the given blockers
        bb::U64 masked_blockers = blockers & attacks::bishop_attack_mask[square];

        // lookup magic number for the given square
        bb::U64 magic = attacks::bishop_magics[square];

        // lookup relevant bits
        int bits = attacks::bishop_relevant_bits[square];

        // transform the blockers to obtain the index for the final lookup
        bb::U64 blockers_index = attacks::MagicTransform(masked_blockers, magic, bits);

        // lookup the actual attack pattern from the pregenerated attack table
        return attacks::bishop_attack_table[square][blockers_index];
    }

    bb::U64 LookupRookAttacks(bb::Square square, bb::U64 blockers) {
        // get attack mask and mas kthe given blockers
        bb::U64 masked_blockers = blockers & attacks::rook_attack_mask[square];

        // lookup magic number for the given square
        bb::U64 magic = attacks::rook_magics[square];

        // lookup relevant bits
        int bits = attacks::rook_relevant_bits[square];

        // transform the blockers to obtain the index for the final lookup
        bb::U64 blockers_index = attacks::MagicTransform(masked_blockers, magic, bits);

        // lookup the actual attack pattern from the pregenerated attack table
        return attacks::rook_attack_table[square][blockers_index];
    }

    bb::U64 GetBlockerConfiguration(int index, bb::U64 attack_mask) {
        // get number of relevant bits for the given attack mask
        int bits = bb::CountBits(attack_mask);
        // initialize empty configuration
        bb::U64 configuration = 0ULL;
        // loop through the attack mask and set the required bits
        for (int i = 0; i < bits; i++) {
            // pop least significant 1 bit in attack mask
            int square = bb::GetLeastSignificantBitIndex(attack_mask);
            bb::ClearBit(attack_mask, square);
            // populate the configuration bitboard at the given location
            if (index & (1 << i)) {
                bb::SetBit(configuration, square);
            }
        }
        return configuration;
    }

    bb::U64 MagicTransform(bb::U64 masked_blockers, bb::U64 magic, int bits) {
        return (masked_blockers * magic) >> (64 - bits);
    }

    bb::U64 FindMagicNumber(bb::Square square, bool is_bishop) {
        // initialize arrays for blockers, attacks, and used indices
        bb::U64 blockers[4096], attacks[4096], used[4096];

        // get attack mask and number of relevant bits
        bb::U64 attack_mask = is_bishop ? bishop_attack_mask[square] : rook_attack_mask[square];
        int bits = is_bishop ? bishop_relevant_bits[square] : rook_relevant_bits[square];
        
        // generate all possible blocker configurations and corresponding attacks
        for (int i = 0; i < (1 << bits); i++) {
            blockers[i] = GetBlockerConfiguration(i, attack_mask);
            attacks[i] = is_bishop ? GetBishopAttacks(square, blockers[i]) : GetRookAttacks(square, blockers[i]);
        }        
        // conduct brute-force random search for suitable magic numbers
        for (int iteration = 0; iteration < 1e6; iteration++) {
            // get magic number candidate
            bb::U64 magic_number = utils::GetRandom64Sparse();
            
            // discard numbers that do not contain enough bits
            if (bb::CountBits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;
            
            // reset array of used attacks
            memset(used, 0ULL, sizeof(used));
            
            // loop through all possible blocker configurations
            int index;
            bool collision;
            for (index = 0, collision = false; !collision && index < (1 << bits); index++) {
                // use the magic number to hash the masked attacks
                int magic_index = attacks::MagicTransform(blockers[index], magic_number, bits);

                // the number is invalid if the hashed key is already in use
                if (used[magic_index] == 0ULL) {
                    used[magic_index] = attacks[index];
                // constructive collisions are fine, actual collisions are forbidden
                } else if (used[magic_index] != attacks[index]) {
                    collision = true;
                }
            }
            // the magic number is valid if there are no collisions
            if (!collision) return magic_number;
        }
        
        // return zero if no suitable number has been found
        return 0ULL;
    }

    void InitializeMagicAttack(bb::Square square, bool is_bishop) {
        // obtain number of relevant bits
        int bits = is_bishop ? bishop_relevant_bits[square] : rook_relevant_bits[square];

        // find magic number and store it in the table
        bb::U64 magic = attacks::FindMagicNumber(square, is_bishop);

        // get all possible blocker configurations and attacks
        bb::U64 blockers, attack;
        bb::U64 attack_mask = is_bishop ? bishop_attack_mask[square] : rook_attack_mask[square];

        for (int i = 0; i < (1 << bits); i++) {
            // get blocker configuration and calculate attack
            blockers = GetBlockerConfiguration(i, attack_mask);
            attack = is_bishop? GetBishopAttacks(square, blockers) : GetRookAttacks(square, blockers);

            // store the attack map at the hashed magic index
            int magic_index = attacks::MagicTransform(blockers, magic, bits);
            if (is_bishop) {
                bishop_magics[square] = magic;
                bishop_attack_table[square][magic_index] = attack;
            } else {
                rook_magics[square] = magic;
                rook_attack_table[square][magic_index] = attack;
            }
        }
    }
}