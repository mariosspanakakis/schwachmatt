#include "board.h"

namespace chess {

Board::Board(const std::string& fen){
    // initialize empty bitboards
    for (bool color : {WHITE, BLACK}){
        for (PieceType pieceType = 0; pieceType < N_PIECE_TYPES; pieceType++) {
            m_occupancies.pieces[color][pieceType] = 0ULL;
        }
        m_occupancies.colors[color] = 0ULL;
    }
    m_occupancies.all = 0ULL;

    // initialize the array of pieces
    for (Square square = 0; square < N_SQUARES; square++) {
        m_pieces[square] = NO_PIECE;
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
            int square = bb::coordinateToSquare(rank, file);

            // set offset for piece referencing
            Color color = is_white ? WHITE : BLACK;

            // update the occupancy bitboard in accordance to the pieces
            switch (figure){
                case 'p':
                    bb::setBit(m_occupancies.pieces[color][PAWN], square);
                    m_pieces[square] = is_white ? WHITE_PAWN : BLACK_PAWN;
                    break;
                case 'n':
                    bb::setBit(m_occupancies.pieces[color][KNIGHT], square);
                    m_pieces[square] = is_white ? WHITE_KNIGHT : BLACK_KNIGHT;
                    break;
                case 'b':
                    bb::setBit(m_occupancies.pieces[color][BISHOP], square);
                    m_pieces[square] = is_white ? WHITE_BISHOP : BLACK_BISHOP;
                    break;
                case 'r':
                    bb::setBit(m_occupancies.pieces[color][ROOK], square);
                    m_pieces[square] = is_white ? WHITE_ROOK : BLACK_ROOK;
                    break;
                case 'q':
                    bb::setBit(m_occupancies.pieces[color][QUEEN], square);
                    m_pieces[square] = is_white ? WHITE_QUEEN : BLACK_QUEEN;
                    break;
                case 'k':
                    bb::setBit(m_occupancies.pieces[color][KING], square);
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
            Bitboard bb = m_occupancies.pieces[color][pieceType];
            m_occupancies.colors[color] |= bb;
        }
        m_occupancies.all |= m_occupancies.colors[color];
    }

    // push the initial game state onto the game state history stack
    m_gameStateHistory.reserve(GAME_STATE_HISTORY_LENGTH);
    m_gameStateHistory.push_back(INITIAL_GAME_STATE);                           // TODO: take care of castling rights etc. as defined in FEN!
};

Bitboard Board::getPieceOccupancy(PieceType pieceType, Color color) const {
    return m_occupancies.pieces[color][pieceType];
}

Bitboard Board::getColorOccupancy(Color color) const {
    return m_occupancies.colors[color];
}

Bitboard Board::getTotalOccupancy() const {
    return m_occupancies.all;
}

Piece Board::getPieceOnSquare(Square square) const {
    return m_pieces[square];
}

Bitboard Board::getCurrentEnPassantTarget() const {
    return m_gameStateHistory.back().enPassantTarget;
}

bool Board::getCastlingRight(CastlingRight castling_right) const {
    return (m_gameStateHistory.back().castlingRights & castling_right);
}

void Board::setPiece(Square square, PieceType pieceType, Color color) {
    assert(m_pieces[square] == NO_PIECE);  // assert that square is empty
    bb::setBit(m_occupancies.pieces[color][pieceType], square);
    bb::setBit(m_occupancies.colors[color], square);
    bb::setBit(m_occupancies.all, square);
    m_pieces[square] = pieceFromColorAndType(color, pieceType);
}

void Board::unsetPiece(Square square, PieceType pieceType, Color color) {
    assert((m_pieces[square] != NO_PIECE));  // assert that square is not empty
    bb::clearBit(m_occupancies.pieces[color][pieceType], square);
    bb::clearBit(m_occupancies.colors[color], square);
    bb::clearBit(m_occupancies.all, square);
    m_pieces[square] = NO_PIECE;
}

void Board::replacePiece(Square square, PieceType pieceType, Color color) {
    assert((m_pieces[square] != NO_PIECE));  // assert that square is not empty
    // get the piece that is on the square
    Piece prevPiece = m_pieces[square];
    Color prevColor = colorFromPiece(prevPiece);
    PieceType prevPieceType = pieceTypeFromPiece(prevPiece);
    // replace the piece
    unsetPiece(square, prevPieceType, prevColor);
    setPiece(square, pieceType, color);
}

// NOTE: there are more efficient approaches than this
bool Board::isAttackedBy(Square square, Color color) const {
    bool us = color;
    bool them = !color;

    // following the I-see-you-you-see-me approach, calculate piece attacks from
    // the initial square and check if the corresponding pieces can be attacked
    Bitboard theirPawns = m_occupancies.pieces[them][PAWN];
    if (attacks::PAWN_ATTACKS[us][square] & theirPawns) {
        return true;
    }
    Bitboard theirKnights = m_occupancies.pieces[them][KNIGHT];
    if (attacks::KNIGHT_ATTACKS[square] & theirKnights) {
        return true;
    }
    Bitboard theirKing = m_occupancies.pieces[them][KING];
    if (attacks::KING_ATTACKS[square] & theirKing) {
        return true;
    }
    Bitboard theirBishopsAndQueens = m_occupancies.pieces[them][BISHOP] | m_occupancies.pieces[them][QUEEN];
    if (attacks::lookupBishopAttacks(square, m_occupancies.all) & theirBishopsAndQueens) {
        return true;
    }
    Bitboard theirRooksAndQueens = m_occupancies.pieces[them][BISHOP] | m_occupancies.pieces[them][QUEEN];
    if (attacks::lookupRookAttacks(square, m_occupancies.all) & theirRooksAndQueens) {
        return true;
    }
    
    return false;
}

void Board::makeMove(Move move) {
    // get information on moving piece
    Square from = move.getFrom();
    Square to = move.getTo();
    Piece movingPiece = m_pieces[from];
    Piece capturedPiece = m_pieces[to];
    PieceType movingPieceType = pieceTypeFromPiece(movingPiece);
    Color movingPieceColor = colorFromPiece(movingPiece);

    GameState newState = GameState();

    // remove the moving piece from its old location
    unsetPiece(from, movingPieceType, movingPieceColor);

    // set the piece on the target square
    if (move.isCapture()) {
        replacePiece(to, movingPieceType, movingPieceColor);
    } else {
        // handle promotions
        if (move.isPromotion()) {
            // get piece type that the pawn promotes to
            PieceType promotionPieceType = move.getPromotionPieceType();
            setPiece(to, promotionPieceType, movingPieceColor);
        } else {
            setPiece(to, movingPieceType, movingPieceColor);
        }
    }

    // store captured piece in the new game state
    newState.capturedPiece = capturedPiece;

    // store en passant target in the new game state
    if (move.isDoublePawnPush()) {
        newState.enPassantTarget = from + (movingPieceColor == WHITE) ? NORTH : SOUTH;
    } else {
        newState.enPassantTarget = 0ULL;
    }

    // castling rights

    // push new game state to stack
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