#include "attacks.hpp"

namespace attacks {  

static Bitboard calculatePawnAttacks(Square square, Color color);
static Bitboard calculateKnightAttacks(Square square);
static Bitboard calculateKingAttacks(Square square);
static Bitboard calculateBishopAttacks(Square square, Bitboard blockers = 0ULL);
static Bitboard calculateRookAttacks(Square square, Bitboard blockers = 0ULL);

static Bitboard calculateBishopMask(Square square);
static Bitboard calculateRookMask(Square square);

static Bitboard getBlockerConfiguration(int index, Bitboard attack_mask);
static Bitboard findMagicNumber(Square square, bool is_bishop);
static Bitboard magicTransform(Bitboard masked_blockers, Bitboard magic, int bits);

static Bitboard lookupBishopAttacks(Square square, Bitboard blockers = 0ULL);
static Bitboard lookupRookAttacks(Square square, Bitboard blockers = 0ULL);

static void initializeMagicAttack(Square square, bool is_bishop);

static uint64_t getRandomU64();
static uint64_t getRandomU64Sparse();

struct Magic {
    Bitboard magic;
    Bitboard mask;
    int mask_bits;
};

static const int MAX_ATTACKS = 4096;

static Magic bishop_magics[N_SQUARES];
static Magic rook_magics[N_SQUARES];

static Bitboard pawn_attacks[N_COLORS][N_SQUARES];
static Bitboard knight_attacks[N_SQUARES];
static Bitboard king_attacks[N_SQUARES];
static Bitboard bishop_attacks[N_SQUARES][MAX_ATTACKS];
static Bitboard rook_attacks[N_SQUARES][MAX_ATTACKS];

static std::random_device rd;
static std::mt19937_64 rng(rd());
static std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

void precalculate(bool info) {
    if (info) {
        std::cout << "Initializing attack tables, please wait..." << std::endl;
    }
    for (int square = 0; square < N_SQUARES; square++) {
        // generate attack maps for all leaping pieces
        for (int color = WHITE; color < N_COLORS; ++color){
            pawn_attacks[color][square] = calculatePawnAttacks(square, color);
        }
        knight_attacks[square] = calculateKnightAttacks(square);
        king_attacks[square] = calculateKingAttacks(square);

        // generate attack masks for bishop and rook
        bishop_magics[square].mask = calculateBishopMask(square);
        bishop_magics[square].mask_bits = bb::count(bishop_magics[square].mask);
        rook_magics[square].mask = calculateRookMask(square);
        rook_magics[square].mask_bits = bb::count(rook_magics[square].mask);

        // generate magic attack tables for the bishops and rooks
        initializeMagicAttack(square, true);
        initializeMagicAttack(square, false);

        // print a progress bar if requested
        if (info) {
            std::cout << utils::getProgressBar(square + 1, N_SQUARES) << "\r";
        }
    }
    if (info) {
        std::cout << std::endl;
    }
    
}

template <PieceType TPieceType>
Bitboard getPieceAttacks(Square square, Bitboard blockers) {
    assert(TPieceType != PAWN);  // this function is not used for pawns
    
    switch (TPieceType) {
        case KNIGHT:
            return knight_attacks[square];
            break;
        case BISHOP:
            return lookupBishopAttacks(square, blockers);
            break;
        case ROOK:
            return lookupRookAttacks(square, blockers);
            break;
        case QUEEN:
            return lookupBishopAttacks(square, blockers) | lookupRookAttacks(square, blockers);
            break;
        case KING:
            return king_attacks[square];
            break;
        default:
            throw std::invalid_argument("Invalid piece type.");
    }
}

Bitboard getPawnAttacks(Square square, Color color) {
    return pawn_attacks[color][square];
}

static Bitboard calculatePawnAttacks(Square square, Color color) {
    Bitboard bitboard = bb::getBitboard(square);
    Bitboard attacks = 0ULL;

    switch (color) {
        case WHITE:
            attacks |= bb::shift<NORTHEAST>(bitboard) | bb::shift<NORTHWEST>(bitboard);
            break;
        case BLACK:
            attacks |= bb::shift<SOUTHEAST>(bitboard) | bb::shift<SOUTHWEST>(bitboard);
            break;
        default:
            throw std::invalid_argument("Invalid color.");
    }
    return attacks;
}

static Bitboard calculateKnightAttacks(Square square) {
    Bitboard bitboard = bb::getBitboard(square);
    Bitboard attacks = 0ULL;
    attacks |= (
          ((bitboard << 6)  & ~(FILE_G_BB | FILE_H_BB))  // WSW
        | ((bitboard << 10) & ~(FILE_A_BB | FILE_B_BB))  // ESE
        | ((bitboard << 15) & ~(FILE_H_BB))              // SSW
        | ((bitboard << 17) & ~(FILE_A_BB))              // SSE
        | ((bitboard >> 6)  & ~(FILE_A_BB | FILE_B_BB))  // ENE
        | ((bitboard >> 10) & ~(FILE_G_BB | FILE_H_BB))  // WNW
        | ((bitboard >> 15) & ~(FILE_A_BB))              // NNE
        | ((bitboard >> 17) & ~(FILE_H_BB))              // NNW
    );
    return attacks;
}

static Bitboard calculateKingAttacks(Square square) {
    Bitboard bitboard = bb::getBitboard(square);
    Bitboard attacks = 0ULL;
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

static Bitboard calculateBishopAttacks(Square square, Bitboard blockers) {
    Bitboard bitboard = bb::getBitboard(square);
    Bitboard attacks = 0ULL;
    Bitboard bb;
    for (bb = bitboard; (bb & ~FILE_H_BB) != 0; bb <<= 9) {  // northeast
        Bitboard attacked_square = bb << 9;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    for (bb = bitboard; (bb & ~FILE_A_BB) != 0; bb <<= 7) {  // northwest
        Bitboard attacked_square = bb << 7;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    for (bb = bitboard; (bb & ~FILE_H_BB) != 0; bb >>= 7) {  // southeast
        Bitboard attacked_square = bb >> 7;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    for (bb = bitboard; (bb & ~FILE_A_BB) != 0; bb >>= 9) {  // southwest
        Bitboard attacked_square = bb >> 9;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    return attacks;
}

Bitboard calculateRookAttacks(Square square, Bitboard blockers) {
    Bitboard bitboard = bb::getBitboard(square);
    Bitboard attacks = 0ULL;
    Bitboard bb;
    for (Bitboard bb = bitboard; (bb & ~RANK_8_BB) != 0; bb <<= 8) {  // north
        Bitboard attacked_square = bb << 8;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    for (bb = bitboard; (bb & ~RANK_1_BB) != 0; bb >>= 8) {  // south
        Bitboard attacked_square = bb >> 8;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    for (bb = bitboard; (bb & ~FILE_H_BB) != 0; bb <<= 1) {  // east
        Bitboard attacked_square = bb << 1;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    for (bb = bitboard; (bb & ~FILE_A_BB) != 0; bb >>= 1) {  // west
        Bitboard attacked_square = bb >> 1;
        attacks |= attacked_square;
        if (blockers & attacked_square) break;
    }
    return attacks;
}

static Bitboard calculateBishopMask(Square square) {
    Bitboard attacks = calculateBishopAttacks(square, 0ULL);
    attacks &= ~EDGE_BB;
    return attacks;
}

static Bitboard calculateRookMask(Square square) {
    Bitboard attacks = calculateRookAttacks(square, 0ULL);
    Bitboard bb = bb::getBitboard(square);
    
    if (!(bb & EDGE_BB)) {        // square not on the edge
        attacks &= ~EDGE_BB;
    } else if (bb & CORNER_BB) {  // square in the corner
        attacks &= ~CORNER_BB;
    } else {                      // square on the edge but not in the corner
        attacks &= ~CORNER_BB;
        if (bb & RANK_1_BB) {
            attacks &= ~(EDGE_BB & ~RANK_1_BB);
        }
        if (bb & RANK_8_BB) {
            attacks &= ~(EDGE_BB & ~RANK_8_BB);
        }
        if (bb & FILE_A_BB) {
            attacks &= ~(EDGE_BB & ~FILE_A_BB);
        }
        if (bb & FILE_H_BB) {
            attacks &= ~(EDGE_BB & ~FILE_H_BB);
        }
    }
    return attacks;
}

static Bitboard lookupBishopAttacks(Square square, Bitboard blockers) {
    // get attack mask and mask the given blockers
    Bitboard masked_blockers = blockers & bishop_magics[square].mask;

    // lookup magic number for the given square
    Bitboard magic = bishop_magics[square].magic;

    // lookup relevant bits
    int bits = bishop_magics[square].mask_bits;

    // transform the blockers to obtain the index for the final lookup
    Bitboard blockers_index = attacks::magicTransform(masked_blockers, magic, bits);

    // lookup the actual attack pattern from the pregenerated attack table
    return attacks::bishop_attacks[square][blockers_index];
}

static Bitboard lookupRookAttacks(Square square, Bitboard blockers) {
    // get attack mask and mask the given blockers
    Bitboard masked_blockers = blockers & rook_magics[square].mask;

    // lookup magic number for the given square
    Bitboard magic = rook_magics[square].magic;

    // lookup relevant bits
    int bits = rook_magics[square].mask_bits;

    // transform the blockers to obtain the index for the final lookup
    Bitboard blockers_index = attacks::magicTransform(masked_blockers, magic, bits);

    // lookup the actual attack pattern from the pregenerated attack table
    return attacks::rook_attacks[square][blockers_index];
}

static Bitboard getBlockerConfiguration(int index, Bitboard attack_mask) {
    // get number of relevant bits for the given attack mask
    int bits = bb::count(attack_mask);
    // initialize empty configuration
    Bitboard configuration = 0ULL;
    // loop through the attack mask and set the required bits
    for (int i = 0; i < bits; i++) {
        // pop least significant 1 bit in attack mask
        int square = bb::popLSB(attack_mask);
        // populate the configuration bitboard at the given location
        if (index & (1 << i)) {
            bb::set(configuration, square);
        }
    }
    return configuration;
}

static Bitboard magicTransform(Bitboard masked_blockers, Bitboard magic, int bits) {
    return (masked_blockers * magic) >> (64 - bits);
}

static Bitboard findMagicNumber(Square square, bool is_bishop) {
    // initialize arrays for blockers, attacks, and used indices
    Bitboard blockers[MAX_ATTACKS], attacks[MAX_ATTACKS], used[MAX_ATTACKS];

    // get attack mask and number of relevant bits
    Bitboard attack_mask = is_bishop ? bishop_magics[square].mask : rook_magics[square].mask;
    int bits = is_bishop ? bishop_magics[square].mask_bits : rook_magics[square].mask_bits;
    
    // generate all possible blocker configurations and corresponding attacks
    for (int i = 0; i < (1 << bits); i++) {
        blockers[i] = getBlockerConfiguration(i, attack_mask);
        attacks[i] = is_bishop ? calculateBishopAttacks(square, blockers[i]) : calculateRookAttacks(square, blockers[i]);
    }        
    // conduct brute-force random search for suitable magic numbers
    for (int iteration = 0; iteration < 1e7; iteration++) {
        // get magic number candidate
        Bitboard magic_number = getRandomU64Sparse();
        
        // discard numbers that do not contain enough bits
        if (bb::count((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;
        
        // reset array of used attacks
        for (int i = 0; i < 4096; ++i) used[i] = 0ULL;
        
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

static void initializeMagicAttack(Square square, bool is_bishop) {
    // obtain number of relevant bits
    int bits = is_bishop ? bishop_magics[square].mask_bits : rook_magics[square].mask_bits;

    // find magic number and store it in the table
    Bitboard magic;
    magic = attacks::findMagicNumber(square, is_bishop);

    // get all possible blocker configurations and attacks
    Bitboard blockers, attack;
    Bitboard attack_mask = is_bishop ? bishop_magics[square].mask : rook_magics[square].mask;

    for (int i = 0; i < (1 << bits); i++) {
        // get blocker configuration and calculate attack
        blockers = getBlockerConfiguration(i, attack_mask);
        attack = is_bishop ? calculateBishopAttacks(square, blockers) : calculateRookAttacks(square, blockers);

        // store the attack map at the hashed magic index
        int magic_index = attacks::magicTransform(blockers, magic, bits);
        if (is_bishop) {
            bishop_magics[square].magic = magic;
            bishop_attacks[square][magic_index] = attack;
        } else {
            rook_magics[square].magic = magic;
            rook_attacks[square][magic_index] = attack;
        }
    }
}

static uint64_t getRandomU64() {
    return dist(rng);
}

static uint64_t getRandomU64Sparse() {
    return getRandomU64() & getRandomU64() & getRandomU64();
}

template Bitboard getPieceAttacks<KNIGHT>(Square square, Bitboard blockers);
template Bitboard getPieceAttacks<BISHOP>(Square square, Bitboard blockers);
template Bitboard getPieceAttacks<ROOK>(Square square, Bitboard blockers);
template Bitboard getPieceAttacks<QUEEN>(Square square, Bitboard blockers);
template Bitboard getPieceAttacks<KING>(Square square, Bitboard blockers);

}  // namespace attacks
