#include "board.h"

namespace chess {

Board::Board(const std::string& fen){
    // initialize empty bitboards for all pieces
    for (bool color : {WHITE, BLACK}){
        for (PieceType pieceType = 0; pieceType < N_PIECE_TYPES; pieceType++) {
            m_pieceBB[color][pieceType] = 0ULL;
        }
        m_occupancyBB[color] = 0ULL;
    }
    m_occupancyCombinedBB = 0ULL;

    // initialize the array of pieces
    for (Square square = 0; square < N_SQUARES; square++) {
        m_pieces[square] = NO_PIECE_TYPE;
    }
    
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
                    m_pieces[square] = is_white ? WHITE_PAWN : BLACK_PAWN;
                    break;
                case 'n':
                    bb::setBit(m_pieceBB[color][KNIGHT], square);
                    m_pieces[square] = is_white ? WHITE_KNIGHT : BLACK_KNIGHT;
                    break;
                case 'b':
                    bb::setBit(m_pieceBB[color][BISHOP], square);
                    m_pieces[square] = is_white ? WHITE_BISHOP : BLACK_BISHOP;
                    break;
                case 'r':
                    bb::setBit(m_pieceBB[color][ROOK], square);
                    m_pieces[square] = is_white ? WHITE_ROOK : BLACK_ROOK;
                    break;
                case 'q':
                    bb::setBit(m_pieceBB[color][QUEEN], square);
                    m_pieces[square] = is_white ? WHITE_QUEEN : BLACK_QUEEN;
                    break;
                case 'k':
                    bb::setBit(m_pieceBB[color][KING], square);
                    m_pieces[square] = is_white ? WHITE_KING : BLACK_KING;
                    break;
            }
            // proceed to next file
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
        for (PieceType pieceType = 0; pieceType < N_PIECE_TYPES; pieceType++) {
            Bitboard bb = m_pieceBB[color][pieceType];
            m_occupancyBB[color] |= bb;
        }
        m_occupancyCombinedBB |= m_occupancyBB[color];
    }

    // push the initial game state onto the game state history stack
    m_gameStateHistory.reserve(GAME_STATE_HISTORY_LENGTH);
    m_gameStateHistory.push_back(INITIAL_GAME_STATE);                           // TODO: take care of castling rights etc. as defined in FEN!
};

Bitboard Board::getPieceBitboard(Piece piece, Color color) const {
    return m_pieceBB[color][piece];
}

Bitboard Board::getOccupancyBitboard(Color color) const {
    return m_occupancyBB[color];
}

Bitboard Board::getCombinedOccupancyBitboard() const {
    return m_occupancyCombinedBB;
}

Piece Board::getPieceOnSquare(Square square) const {
    return m_pieces[square];
}

// NOTE: there are more efficient approaches than this
bool Board::isAttackedBy(Square square, Color color) const {
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

    Square fromSquare = move.getFrom();
    Square toSquare = move.getTo();
    Piece movingPiece = m_pieces[fromSquare];
}
    
Bitboard Board::getCurrentEnPassantTarget() const {
    return m_gameStateHistory.back().enPassantTarget;
}

bool Board::getCastlingRight(CastlingRight castling_right) const {
    return (m_gameStateHistory.back().castlingRights & castling_right);
}

void Board::print() {
    std::cout << std::endl;
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << " " << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            int square = (rank * 8 + file);
            Piece piece = m_pieces[square];
            std::cout << " " << PIECE_SYMBOLS[piece];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "     A B C D E F G H" << std::endl;
    std::cout << std::endl;
}

}   // namespace chess