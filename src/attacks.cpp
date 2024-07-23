#include "attacks.h"

namespace chess {
namespace attacks {

Bitboard PAWN_ATTACKS[N_COLORS][N_SQUARES];
Bitboard KNIGHT_ATTACKS[N_SQUARES];
Bitboard KING_ATTACKS[N_SQUARES];
Bitboard BISHOP_ATTACKS[N_SQUARES][4096];
Bitboard ROOK_ATTACKS[N_SQUARES][4096];

Bitboard BISHOP_MASK[N_SQUARES];
Bitboard ROOK_MASK[N_SQUARES];

Bitboard BISHOP_MAGICS[N_SQUARES];
Bitboard ROOK_MAGICS[N_SQUARES];

void initializeAttackTables() {
    // loop through all board squares and precalculate attack maps
    for (int square = 0; square < N_SQUARES; square++) {
        
        // generate attack maps for all leaping pieces
        for (int color = WHITE; color < N_COLORS; ++color){
            PAWN_ATTACKS[color][square] = calculatePawnAttacks(square, color);
        }
        KNIGHT_ATTACKS[square] = calculateKnightAttacks(square);
        KING_ATTACKS[square] = calculateKingAttacks(square);

        // generate attack masks for bishop and rook
        BISHOP_MASK[square] = calculateBishopAttacks(square, 0ULL, true);
        ROOK_MASK[square] = calculateRookAttacks(square, 0ULL, true);

        // generate magic attack tables for the bishops and rooks
        initializeMagicAttack(square, true);
        initializeMagicAttack(square, false);
    }
}

Bitboard calculatePawnAttacks(Square square, Color color) {
    // initialize bitboards for pawn position and attacked fields
    Bitboard bitboard = 0ULL;
    bb::setBit(bitboard, square);
    Bitboard attacks = 0ULL;

    if (color == WHITE) {
        attacks |= (
            bb::shift<NORTHWEST>(bitboard) | bb::shift<NORTHEAST>(bitboard)
        );
    } else if (color == BLACK) {
        attacks |= (
            bb::shift<SOUTHWEST>(bitboard) | bb::shift<SOUTHEAST>(bitboard)
        );
    }
    return attacks;
}

Bitboard calculateKnightAttacks(Square square) {
    // initialize bitboards for knight position and attacked fields
    Bitboard bitboard = 0ULL;
    bb::setBit(bitboard, square);
    Bitboard attacks = 0ULL;
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

Bitboard calculateKingAttacks(Square square) {
    // initialize bitboards for king position and attacked fields
    Bitboard bitboard = 0ULL;
    bb::setBit(bitboard, square);
    Bitboard attacks = 0ULL;
    // generate attacks
    attacks |= (
          bb::shift<NORTH>(bitboard)
        | bb::shift<NORTHEAST>(bitboard)
        | bb::shift<EAST>(bitboard)
        | bb::shift<SOUTHEAST>(bitboard)
        | bb::shift<SOUTH>(bitboard)
        | bb::shift<SOUTHWEST>(bitboard)
        | bb::shift<WEST>(bitboard)
        | bb::shift<NORTHWEST>(bitboard)
    );
    return attacks;
}

Bitboard calculateBishopAttacks(Square square, Bitboard blockers, bool mask_mode) {
    // initialize bitboards for bishop position and attacked fields
    Bitboard bitboard = 0ULL;
    bb::setBit(bitboard, square);
    Bitboard attacks = 0ULL;
    Bitboard bb;
    // northeast diagonal
    for (bb = bitboard; (bb & ~bb::FILE_H_BB) != 0; bb <<= 9) {
        Bitboard attacked_square = bb << 9;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    // northwest diagonal
    for (bb = bitboard; (bb & ~bb::FILE_A_BB) != 0; bb <<= 7) {
        Bitboard attacked_square = bb << 7;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    // southeast diagonal
    for (bb = bitboard; (bb & ~bb::FILE_H_BB) != 0; bb >>= 7) {
        Bitboard attacked_square = bb >> 7;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    // southwest diagonal
    for (bb = bitboard; (bb & ~bb::FILE_A_BB) != 0; bb >>= 9) {
        Bitboard attacked_square = bb >> 9;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    if (mask_mode) {
        attacks &= ~bb::EDGE_BB;
    }
    return attacks;
}

Bitboard calculateRookAttacks(Square square, Bitboard blockers, bool mask_mode) {
    // initialize bitboards for rook position and attacked fields
    Bitboard bitboard = 0ULL;
    bb::setBit(bitboard, square);
    Bitboard attacks = 0ULL;
    Bitboard bb;
    if (mask_mode) {
        // north line
        for (Bitboard bb = bitboard; (bb & ~bb::RANK_7_BB) != 0; bb <<= 8) {
            Bitboard attacked_square = bb << 8;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // south line
        for (bb = bitboard; (bb & ~bb::RANK_2_BB) != 0; bb >>= 8) {
            Bitboard attacked_square = bb >> 8;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // east line
        for (bb = bitboard; (bb & ~bb::FILE_H_BB & ~bb::FILE_G_BB) != 0; bb <<= 1) {
            Bitboard attacked_square = bb << 1;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // west line
        for (bb = bitboard; (bb & ~bb::FILE_A_BB & ~bb::FILE_B_BB) != 0; bb >>= 1) {
            Bitboard attacked_square = bb >> 1;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
    } else {
        // north line
        for (Bitboard bb = bitboard; (bb & ~bb::RANK_8_BB) != 0; bb <<= 8) {
            Bitboard attacked_square = bb << 8;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // south line
        for (bb = bitboard; (bb & ~bb::RANK_1_BB) != 0; bb >>= 8) {
            Bitboard attacked_square = bb >> 8;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // east line
        for (bb = bitboard; (bb & ~bb::FILE_H_BB) != 0; bb <<= 1) {
            Bitboard attacked_square = bb << 1;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
        // west line
        for (bb = bitboard; (bb & ~bb::FILE_A_BB) != 0; bb >>= 1) {
            Bitboard attacked_square = bb >> 1;
            attacks |= attacked_square;
            if (blockers & attacked_square) break;
        }
    }
    return attacks;
}

Bitboard lookupBishopAttacks(Square square, Bitboard blockers) {
    // get attack mask and mask the given blockers
    Bitboard masked_blockers = blockers & attacks::BISHOP_MASK[square];

    // lookup magic number for the given square
    Bitboard magic = attacks::BISHOP_MAGICS[square];

    // lookup relevant bits
    int bits = attacks::BISHOP_BITS[square];

    // transform the blockers to obtain the index for the final lookup
    Bitboard blockers_index = attacks::magicTransform(masked_blockers, magic, bits);

    // lookup the actual attack pattern from the pregenerated attack table
    return attacks::BISHOP_ATTACKS[square][blockers_index];
}

Bitboard lookupRookAttacks(Square square, Bitboard blockers) {
    // get attack mask and mas kthe given blockers
    Bitboard masked_blockers = blockers & attacks::ROOK_MASK[square];

    // lookup magic number for the given square
    Bitboard magic = attacks::ROOK_MAGICS[square];

    // lookup relevant bits
    int bits = attacks::ROOK_BITS[square];

    // transform the blockers to obtain the index for the final lookup
    Bitboard blockers_index = attacks::magicTransform(masked_blockers, magic, bits);

    // lookup the actual attack pattern from the pregenerated attack table
    return attacks::ROOK_ATTACKS[square][blockers_index];
}

Bitboard getBlockerConfiguration(int index, Bitboard attack_mask) {
    // get number of relevant bits for the given attack mask
    int bits = bb::countBits(attack_mask);
    // initialize empty configuration
    Bitboard configuration = 0ULL;
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

Bitboard magicTransform(Bitboard masked_blockers, Bitboard magic, int bits) {
    return (masked_blockers * magic) >> (64 - bits);
}

Bitboard findMagicNumber(Square square, bool is_bishop) {
    // initialize arrays for blockers, attacks, and used indices
    Bitboard blockers[4096], attacks[4096], used[4096];

    // get attack mask and number of relevant bits
    Bitboard attack_mask = is_bishop ? BISHOP_MASK[square] : ROOK_MASK[square];
    int bits = is_bishop ? BISHOP_BITS[square] : ROOK_BITS[square];
    
    // generate all possible blocker configurations and corresponding attacks
    for (int i = 0; i < (1 << bits); i++) {
        blockers[i] = getBlockerConfiguration(i, attack_mask);
        attacks[i] = is_bishop ? calculateBishopAttacks(square, blockers[i]) : calculateRookAttacks(square, blockers[i]);
    }        
    // conduct brute-force random search for suitable magic numbers
    for (int iteration = 0; iteration < 1e7; iteration++) {
        // get magic number candidate
        Bitboard magic_number = utils::getRandom64Sparse();
        
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
    
    // throw an exception if no magic number has been found
    throw MagicNotFoundException(
        "No magic number found for square " + std::to_string(square) + "."
    );
}

void initializeMagicAttack(Square square, bool is_bishop) {
    // obtain number of relevant bits
    int bits = is_bishop ? BISHOP_BITS[square] : ROOK_BITS[square];

    // find magic number and store it in the table
    Bitboard magic;
    magic = attacks::findMagicNumber(square, is_bishop);

    // get all possible blocker configurations and attacks
    Bitboard blockers, attack;
    Bitboard attack_mask = is_bishop ? BISHOP_MASK[square] : ROOK_MASK[square];

    for (int i = 0; i < (1 << bits); i++) {
        // get blocker configuration and calculate attack
        blockers = getBlockerConfiguration(i, attack_mask);
        attack = is_bishop ? calculateBishopAttacks(square, blockers) : calculateRookAttacks(square, blockers);

        // store the attack map at the hashed magic index
        int magic_index = attacks::magicTransform(blockers, magic, bits);
        if (is_bishop) {
            BISHOP_MAGICS[square] = magic;
            BISHOP_ATTACKS[square][magic_index] = attack;
        } else {
            ROOK_MAGICS[square] = magic;
            ROOK_ATTACKS[square][magic_index] = attack;
        }
    }
}

}   // namespace attacks
}   // namespace chess