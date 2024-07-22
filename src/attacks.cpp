#include "attacks.h"

namespace attacks {

    bb::U64 pawnAttackTable[bb::N_COLORS][bb::N_SQUARES];
    bb::U64 knightAttackTable[bb::N_SQUARES];
    bb::U64 kingAttackTable[bb::N_SQUARES];
    bb::U64 bishopAttackTable[bb::N_SQUARES][4096];
    bb::U64 rookAttackTable[bb::N_SQUARES][4096];

    bb::U64 bishopAttackMask[bb::N_SQUARES];
    bb::U64 rookAttackMask[bb::N_SQUARES];

    bb::U64 bishopMagics[bb::N_SQUARES];
    bb::U64 rookMagics[bb::N_SQUARES];

    void initializeAttackTables() {
        
        // loop through all board squares and precalculate attack maps
        for (int square = 0; square < bb::N_SQUARES; square++) {
            
            // generate attack maps for all leaping pieces
            for (int color = bb::WHITE; color < bb::N_COLORS; ++color){
                pawnAttackTable[color][square] = calculatePawnAttacks(square, color);
            }
            knightAttackTable[square] = calculateKnightAttacks(square);
            kingAttackTable[square] = calculateKingAttacks(square);

            // generate attack masks for bishop and rook
            bishopAttackMask[square] = calculateBishopAttacks(square, 0ULL, true);
            rookAttackMask[square] = calculateRookAttacks(square, 0ULL, true);

            // generate magic attack tables for the bishops and rooks
            initializeMagicAttack(square, true);
            initializeMagicAttack(square, false);
        }
    }

    bb::U64 calculatePawnAttacks(bb::Square square, bb::Color color) {
        // initialize bitboards for pawn position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::setBit(bitboard, square);
        bb::U64 attacks = 0ULL;

        if (color == bb::WHITE) {
            attacks |= (
                bb::shiftNorthWest(bitboard) | bb::shiftNorthEast(bitboard)
            );
        } else if (color == bb::BLACK) {
            attacks |= (
                bb::shiftSouthWest(bitboard) | bb::shiftSouthEast(bitboard)
            );
        }
        return attacks;
    }

    bb::U64 calculateKnightAttacks(bb::Square square) {
        // initialize bitboards for knight position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::setBit(bitboard, square);
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

    bb::U64 calculateKingAttacks(bb::Square square) {
        // initialize bitboards for king position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::setBit(bitboard, square);
        bb::U64 attacks = 0ULL;
        // generate attacks
        attacks |= (
              bb::shiftNorth(bitboard)
            | bb::shiftNorthEast(bitboard)
            | bb::shiftEast(bitboard)
            | bb::shiftSouthEast(bitboard)
            | bb::shiftSouth(bitboard)
            | bb::shiftSouthWest(bitboard)
            | bb::shiftWest(bitboard)
            | bb::shiftNorthWest(bitboard)
        );
        return attacks;
    }

    bb::U64 calculateBishopAttacks(bb::Square square, bb::U64 blockers, bool mask_mode) {
        // initialize bitboards for bishop position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::setBit(bitboard, square);
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
        if (mask_mode) {
            attacks &= ~bb::EDGE_BB;
        }
        return attacks;
    }

    bb::U64 calculateRookAttacks(bb::Square square, bb::U64 blockers, bool mask_mode) {
        // initialize bitboards for rook position and attacked fields
        bb::U64 bitboard = 0ULL;
        bb::setBit(bitboard, square);
        bb::U64 attacks = 0ULL;
        bb::U64 bb;
        if (mask_mode) {
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
        } else {
            // north line
            for (bb::U64 bb = bitboard; (bb & ~bb::RANK_8_BB) != 0; bb <<= 8) {
                bb::U64 attacked_square = bb << 8;
                attacks |= attacked_square;
                if (blockers & attacked_square) break;
            }
            // south line
            for (bb = bitboard; (bb & ~bb::RANK_1_BB) != 0; bb >>= 8) {
                bb::U64 attacked_square = bb >> 8;
                attacks |= attacked_square;
                if (blockers & attacked_square) break;
            }
            // east line
            for (bb = bitboard; (bb & ~bb::FILE_H_BB) != 0; bb <<= 1) {
                bb::U64 attacked_square = bb << 1;
                attacks |= attacked_square;
                if (blockers & attacked_square) break;
            }
            // west line
            for (bb = bitboard; (bb & ~bb::FILE_A_BB) != 0; bb >>= 1) {
                bb::U64 attacked_square = bb >> 1;
                attacks |= attacked_square;
                if (blockers & attacked_square) break;
            }
        }
        return attacks;
    }

    bb::U64 lookupBishopAttacks(bb::Square square, bb::U64 blockers) {
        // get attack mask and mask the given blockers
        bb::U64 masked_blockers = blockers & attacks::bishopAttackMask[square];

        // lookup magic number for the given square
        bb::U64 magic = attacks::bishopMagics[square];

        // lookup relevant bits
        int bits = attacks::bishopRelevantBits[square];

        // transform the blockers to obtain the index for the final lookup
        bb::U64 blockers_index = attacks::magicTransform(masked_blockers, magic, bits);

        // lookup the actual attack pattern from the pregenerated attack table
        return attacks::bishopAttackTable[square][blockers_index];
    }

    bb::U64 lookupRookAttacks(bb::Square square, bb::U64 blockers) {
        // get attack mask and mas kthe given blockers
        bb::U64 masked_blockers = blockers & attacks::rookAttackMask[square];

        // lookup magic number for the given square
        bb::U64 magic = attacks::rookMagics[square];

        // lookup relevant bits
        int bits = attacks::rookRelevantBits[square];

        // transform the blockers to obtain the index for the final lookup
        bb::U64 blockers_index = attacks::magicTransform(masked_blockers, magic, bits);

        // lookup the actual attack pattern from the pregenerated attack table
        return attacks::rookAttackTable[square][blockers_index];
    }

    bb::U64 getBlockerConfiguration(int index, bb::U64 attack_mask) {
        // get number of relevant bits for the given attack mask
        int bits = bb::countBits(attack_mask);
        // initialize empty configuration
        bb::U64 configuration = 0ULL;
        // loop through the attack mask and set the required bits
        for (int i = 0; i < bits; i++) {
            // pop least significant 1 bit in attack mask
            int square = bb::getLeastSignificantBitIndex(attack_mask);
            bb::clearBit(attack_mask, square);
            // populate the configuration bitboard at the given location
            if (index & (1 << i)) {
                bb::setBit(configuration, square);
            }
        }
        return configuration;
    }

    bb::U64 magicTransform(bb::U64 masked_blockers, bb::U64 magic, int bits) {
        return (masked_blockers * magic) >> (64 - bits);
    }

    bb::U64 findMagicNumber(bb::Square square, bool is_bishop) {
        // initialize arrays for blockers, attacks, and used indices
        bb::U64 blockers[4096], attacks[4096], used[4096];

        // get attack mask and number of relevant bits
        bb::U64 attack_mask = is_bishop ? bishopAttackMask[square] : rookAttackMask[square];
        int bits = is_bishop ? bishopRelevantBits[square] : rookRelevantBits[square];
        
        // generate all possible blocker configurations and corresponding attacks
        for (int i = 0; i < (1 << bits); i++) {
            blockers[i] = getBlockerConfiguration(i, attack_mask);
            attacks[i] = is_bishop ? calculateBishopAttacks(square, blockers[i]) : calculateRookAttacks(square, blockers[i]);
        }        
        // conduct brute-force random search for suitable magic numbers
        for (int iteration = 0; iteration < 1e6; iteration++) {
            // get magic number candidate
            bb::U64 magic_number = utils::getRandom64Sparse();
            
            // discard numbers that do not contain enough bits
            if (bb::countBits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;
            
            // reset array of used attacks
            memset(used, 0ULL, sizeof(used));
            
            // loop through all possible blocker configurations
            int index;
            bool collision;
            for (index = 0, collision = false; !collision && index < (1 << bits); index++) {
                // use the magic number to hash the masked attacks
                int magic_index = attacks::magicTransform(blockers[index], magic_number, bits);

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

    void initializeMagicAttack(bb::Square square, bool is_bishop) {
        // obtain number of relevant bits
        int bits = is_bishop ? bishopRelevantBits[square] : rookRelevantBits[square];

        // find magic number and store it in the table
        bb::U64 magic = attacks::findMagicNumber(square, is_bishop);

        // get all possible blocker configurations and attacks
        bb::U64 blockers, attack;
        bb::U64 attack_mask = is_bishop ? bishopAttackMask[square] : rookAttackMask[square];

        for (int i = 0; i < (1 << bits); i++) {
            // get blocker configuration and calculate attack
            blockers = getBlockerConfiguration(i, attack_mask);
            attack = is_bishop ? calculateBishopAttacks(square, blockers) : calculateRookAttacks(square, blockers);

            // store the attack map at the hashed magic index
            int magic_index = attacks::magicTransform(blockers, magic, bits);
            if (is_bishop) {
                bishopMagics[square] = magic;
                bishopAttackTable[square][magic_index] = attack;
            } else {
                rookMagics[square] = magic;
                rookAttackTable[square][magic_index] = attack;
            }
        }
    }
}