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
    for (uint64_t i = 0; i < fen_pieces.length(); i++) {
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

    // instantiate game state object
    GameState gameState = GameState();

    // handle side to move
    std::string fen_side_to_move = fen_groups[1];
    gameState.sideToMove = fen_side_to_move.compare("w") == 0 ? WHITE : BLACK;

    // handle castling rights
    std::string fen_castling = fen_groups[2];
    CastlingRight castling_right = 0;
    for (char c : fen_castling) {
        switch (c) {
            case 'K':
                castling_right |= WHITE_KINGSIDE_CASTLING;
                break;
            case 'Q':
                castling_right |= WHITE_QUEENSIDE_CASTLING;
                break;
            case 'k':
                castling_right |= BLACK_KINGSIDE_CASTLING;
                break;
            case 'q':
                castling_right |= BLACK_QUEENSIDE_CASTLING;
                break;
            case '-':
                break;
            default:
                std::cerr << "Invalid castling character: " << c << std::endl;
        }
    }
    gameState.castlingRights = castling_right;

    // handle en passant square
    std::string fen_en_passant = fen_groups[3]; // like "e3"
    gameState.enPassantTarget = 0ULL;
    for (int i = 0; i < N_SQUARES; ++i) {
        if (SQUARE_NAMES[i] == fen_en_passant) {
            bb::setBit(gameState.enPassantTarget, i);
        }
    }

    // TODO: handle move counters

    // push the initial game state onto the game state history stack
    m_gameStateHistory.reserve(GAME_STATE_HISTORY_LENGTH);
    m_gameStateHistory.push_back(gameState);
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

CastlingRight Board::getCastlingRights() const {
    return m_gameStateHistory.back().castlingRights;
}

CastlingRight Board::getCastlingRights(Color color) const {
    return m_gameStateHistory.back().castlingRights & ((getSideToMove() == WHITE) ? WHITE_CASTLING : BLACK_CASTLING);
}

bool Board::canCastle(Color color, CastlingRight castling_right) const {
    return (m_gameStateHistory.back().castlingRights & castling_right);
}

Color Board::getSideToMove() const {
    return m_gameStateHistory.back().sideToMove;
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
    if (attacks::getPawnAttacks(square, us) & theirPawns) {
        return true;
    }
    Bitboard theirKnights = m_occupancies.pieces[them][KNIGHT];
    if (attacks::getPieceAttacks<KNIGHT>(square, 0ULL) & theirKnights) {
        return true;
    }
    Bitboard theirKing = m_occupancies.pieces[them][KING];
    if (attacks::getPieceAttacks<KING>(square, 0ULL) & theirKing) {
        return true;
    }
    Bitboard theirBishopsAndQueens = m_occupancies.pieces[them][BISHOP] | m_occupancies.pieces[them][QUEEN];
    if (attacks::getPieceAttacks<BISHOP>(square, m_occupancies.all) & theirBishopsAndQueens) {
        return true;
    }
    Bitboard theirRooksAndQueens = m_occupancies.pieces[them][BISHOP] | m_occupancies.pieces[them][QUEEN];
    if (attacks::getPieceAttacks<ROOK>(square, m_occupancies.all) & theirRooksAndQueens) {
        return true;
    }
    
    return false;
}

bool Board::isInCheck(Color color) const {
    Bitboard king = getPieceOccupancy(KING, color);
    Square kingSquare = bb::getLeastSignificantBitIndex(king);
    return isAttackedBy(kingSquare, color);
}

void Board::makeMove(Move move) {
    // get information on moving piece
    Square from = move.getFrom();
    Square to = move.getTo();
    Piece movingPiece = m_pieces[from];
    Piece capturedPiece = m_pieces[to];
    PieceType movingPieceType = pieceTypeFromPiece(movingPiece);
    Color movingPieceColor = colorFromPiece(movingPiece);                       // could be replaced by getSideToMove()

    GameState oldGameState = m_gameStateHistory.back();
    GameState newGameState = GameState();

    // remove the moving piece from its old location
    unsetPiece(from, movingPieceType, movingPieceColor);

    // set the piece on the target square
    if (move.isCapture()) {
        replacePiece(to, movingPieceType, movingPieceColor);
    } else {
        // handle promotions
        if (move.isPromotion()) {                                               // TODO: promotions don't work so far
            // get piece type that the pawn promotes to
            PieceType promotionPieceType = move.getPromotionPieceType();
            setPiece(to, promotionPieceType, movingPieceColor);
        } else {
            setPiece(to, movingPieceType, movingPieceColor);
        }
    }

    // store captured piece in the new game state
    newGameState.capturedPiece = capturedPiece;

    // store en passant target in the new game state
    if (move.isDoublePawnPush()) {
        newGameState.enPassantTarget = from + (movingPieceColor == WHITE) ? NORTH : SOUTH;
    } else {
        newGameState.enPassantTarget = 0ULL;
    }

    // TODO: withdraw castling rights if rook or king moves
    if (movingPieceType == ROOK || movingPiece == KING) {
        
    }
    // TODO: withdraw castling rights if rook is captured

    // TODO: handle castling in general!

    // flip side to move
    newGameState.sideToMove = !oldGameState.sideToMove;

    // push new game state to stack
    m_gameStateHistory.push_back(newGameState);
}

void Board::unmakeMove(Move move) {

    // store and pop the game state introduced by the move to be unmade
    GameState state = m_gameStateHistory.back();
    m_gameStateHistory.pop_back();

    // get relevant squares
    Square from = move.getFrom();
    Square to = move.getTo();
    // retrieve information on involved pieces
    Piece movingPiece = m_pieces[to];                                           // the moving piece is on the to square
    PieceType movingPieceType = pieceTypeFromPiece(movingPiece);
    Color movingPieceColor = colorFromPiece(movingPiece);
    Piece capturedPiece = state.capturedPiece;                                  // retrieve the captured piece from the game state
    PieceType capturedPieceType = pieceTypeFromPiece(capturedPiece);
    Color capturedPieceColor = colorFromPiece(capturedPiece);

    // place the piece on its original square
    setPiece(from, movingPieceType, movingPieceColor);

    if (move.isCapture()) {
        replacePiece(to, capturedPieceType, capturedPieceColor);                // TODO: should be overloaded to either except type and color or piece
    } else {
        // handle promotions
        if (move.isPromotion()) {                                               // TODO: promotions don't work so far
            // get piece type that the pawn promotes to
            PieceType promotionPieceType = move.getPromotionPieceType();
            unsetPiece(to, promotionPieceType, movingPieceColor);
        } else {
            unsetPiece(to, movingPieceType, movingPieceColor);
        }
    }

    // TODO: restore castling rights if rook or king moves
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

    // get written representation of castling rights
    std::string castling = std::string(canCastle(WHITE, WHITE_KINGSIDE_CASTLING) ? "K" : "")
                         + std::string(canCastle(WHITE, WHITE_QUEENSIDE_CASTLING) ? "Q" : "")
                         + std::string(canCastle(BLACK, BLACK_KINGSIDE_CASTLING) ? "k" : "")
                         + std::string(canCastle(BLACK, BLACK_QUEENSIDE_CASTLING) ? "q" : "");

    if (castling.compare("") == 0) {
        castling = "-";
    }

    // get written representation of en passant square
    int enPassantIndex = bb::getLeastSignificantBitIndex(getCurrentEnPassantTarget());
    std::string enPassantSquare = (enPassantIndex != -1) ? SQUARE_NAMES[enPassantIndex] : "NONE";

    // TODO: print further game information (side to move, castling rights, en passant square)
    std::cout << "State:" << std::endl;
    std::cout << std::setw(20) << "Side to Move:"
              << std::setw(8) << COLOR_NAMES[getSideToMove()] << std::endl;
    std::cout << std::setw(20) << "Castling Rights:"
              << std::setw(8) << castling << std::endl;
    std::cout << std::setw(20) << "En Passant Target:"
              << std::setw(8) << enPassantSquare << std::endl;

    std::cout << std::endl;
}

}   // namespace chess