#include "board.h"

namespace chess {

Board::Board(const std::string& fen){
    // initialize empty bitboards for all pieces
    for (bool color : {WHITE, BLACK}){
        for (Piece piece = 0; piece < N_PIECES; piece++) {
            m_pieceBB[color][piece] = 0ULL;
        }
        m_occupancyBB[color] = 0ULL;
    }
    m_occupancyCombinedBB = 0ULL;
    
    // split the given FEN into groups that describe the board status
    std::vector<std::string> fen_groups = utils::splitFen(fen);

    // loop through FEN and set up pieces as specified, note that FEN notation
    // counts ranks downwards but files upwards!
    std::string fen_pieces = fen_groups[0];
    int rank = RANK_8;
    int file = FILE_A;
    for (uint64_t i = 0; i < fen_pieces.length(); i++){
        char symbol = fen_pieces[i];

        // if the character is a number, advance the given number of squares
        if (isdigit(symbol)){
            int number = symbol - '0';  // convert to int
            file += number;
        }
        // if it is a letter, set the corresponding figure
        else if (isalpha(symbol)){
            // case determines color
            bool is_white = isupper(symbol);
            char figure = tolower(symbol);

            // obtain square from rank and file
            int square = bb::convertCoordToSquare(rank, file);

            // set offset for piece referencing
            Color color = is_white ? WHITE : BLACK;

            // update the occupancy bitboard in accordance to the pieces
            switch (figure){
                case 'p':
                    bb::setBit(m_pieceBB[color][PAWN], square);
                    break;
                case 'n':
                    bb::setBit(m_pieceBB[color][KNIGHT], square);
                    break;
                case 'b':
                    bb::setBit(m_pieceBB[color][BISHOP], square);
                    break;
                case 'r':
                    bb::setBit(m_pieceBB[color][ROOK], square);
                    break;
                case 'q':
                    bb::setBit(m_pieceBB[color][QUEEN], square);
                    break;
                case 'k':
                    bb::setBit(m_pieceBB[color][KING], square);
                    break;
            }
            // procesed to next file
            file += 1;
        }

        // reset file and rank after having looped through all files of a rank
        if (file > 7) {
            file -= 8;
            rank -= 1;
        }
    }

    // throw an exception for invalid FEN
    if (rank != -1 || file != 0){
        throw InvalidFENException(std::string("The specified FEN is invalid."));
    }

    // update the occupancy bitboards
    for (bool color : {WHITE, BLACK}) {
        for (Piece piece = 0; piece < N_PIECES; piece++) {
            Bitboard bb = m_pieceBB[color][piece];
            m_occupancyBB[color] |= bb;
        }
        m_occupancyCombinedBB |= m_occupancyBB[color];
    }

    // push the initial game state onto the game state history stack
    m_gameStateHistory.reserve(512);
    m_gameStateHistory.push_back(INITIAL_GAME_STATE);
};

Bitboard Board::getPieceBitboard(Piece piece, Color color) {
    return m_pieceBB[color][piece];
}

Bitboard Board::getOccupancyBitboard(Color color) {
    return m_occupancyBB[color];
}

Bitboard Board::getCombinedOccupancyBitboard() {
    return m_occupancyCombinedBB;
}

// NOTE: there are more efficient approaches than this
bool Board::isAttackedBy(Square square, Color color) {
    bool us = color;
    bool them = !color;

    // following the I-see-you-you-see-me approach, calculate piece attacks from
    // the initial square and check if the corresponding pieces can be attacked
    Bitboard theirPawns = m_pieceBB[them][PAWN];
    if (attacks::PAWN_ATTACKS[us][square] & theirPawns) {
        return true;
    }
    Bitboard theirKnights = m_pieceBB[them][KNIGHT];
    if (attacks::KNIGHT_ATTACKS[square] & theirKnights) {
        return true;
    }
    Bitboard theirKing = m_pieceBB[them][KING];
    if (attacks::KING_ATTACKS[square] & theirKing) {
        return true;
    }
    Bitboard theirBishopsAndQueens = m_pieceBB[them][BISHOP] | m_pieceBB[them][QUEEN];
    if (attacks::lookupBishopAttacks(square, m_occupancyCombinedBB) & theirBishopsAndQueens) {
        return true;
    }
    Bitboard theirRooksAndQueens = m_pieceBB[them][BISHOP] | m_pieceBB[them][QUEEN];
    if (attacks::lookupRookAttacks(square, m_occupancyCombinedBB) & theirRooksAndQueens) {
        return true;
    }
    
    return false;
}

void Board::makeMove(Move move) {
    // things to take care of:
    // - remove the piece from its old location
    // - add the piece to the new location
    // - captures
    // - en passant
    // - promotions
    // - castling
}
    
Bitboard Board::getCurrentEnPassantTarget() {
    return m_gameStateHistory.back().enPassantTarget;
}

bool Board::getCastlingRight(uint8_t castling_right) {
    return (m_gameStateHistory.back().castlingRights & castling_right);
}

}   // namespace chess