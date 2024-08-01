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

bool Board::canCastle(Color color, CastlingRight castling_right) const {
    return (m_gameStateHistory.back().castlingRights & castling_right);
}

Color Board::getSideToMove() const {
    return m_gameStateHistory.back().sideToMove;
}

void Board::setPiece(Square square, Piece piece) {
    assert(m_pieces[square] == NO_PIECE);  // assert that square is empty
    bb::setBit(m_occupancies.pieces[colorOf(piece)][typeOf(piece)], square);
    bb::setBit(m_occupancies.colors[colorOf(piece)], square);
    bb::setBit(m_occupancies.all, square);
    m_pieces[square] = piece;
}

void Board::unsetPiece(Square square) {
    Piece piece = m_pieces[square];
    assert((piece != NO_PIECE));  // assert that square is not empty
    bb::clearBit(m_occupancies.pieces[colorOf(piece)][typeOf(piece)], square);
    bb::clearBit(m_occupancies.colors[colorOf(piece)], square);
    bb::clearBit(m_occupancies.all, square);
    m_pieces[square] = NO_PIECE;
}

void Board::replacePiece(Square square, Piece piece) {
    unsetPiece(square);
    setPiece(square, piece);
}

// NOTE: there are more efficient approaches than this
bool Board::isAttackedBy(Square square, Color color) const {
    Color us = !color;
    Color them = color;

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
    Bitboard theirRooksAndQueens = m_occupancies.pieces[them][ROOK] | m_occupancies.pieces[them][QUEEN];
    if (attacks::getPieceAttacks<ROOK>(square, m_occupancies.all) & theirRooksAndQueens) {
        return true;
    }
    
    return false;
}

bool Board::isInCheck(Color color) const {
    Bitboard king = getPieceOccupancy(KING, color);
    Square kingSquare = bb::getLeastSignificantBitIndex(king);
    return isAttackedBy(kingSquare, !color);
}

/* TODO:
This is terribly inefficient. Make template for color dependency and maybe move
type, get rid of the cascaded if-else conditions and clean things up.
*/
void Board::makeMove(Move move) {                                               // TODO: add a movePiece function

    Color us = getSideToMove();
    Square from = move.getFrom();
    Square to = move.getTo();
    Piece piece = m_pieces[from];
    Piece captured = move.isEnPassantCapture() ? ((us == WHITE) ? BLACK_PAWN : WHITE_PAWN) : m_pieces[to];

    GameState oldGameState = m_gameStateHistory.back();
    GameState newGameState = GameState();
    newGameState.castlingRights = oldGameState.castlingRights;                  // TODO: implement copy constructor for game state that copies the relevant attributes

    if (move.isCastling()) {
        MoveFlag flag = move.getFlag();
        assert(typeOf(piece) == KING);    // moving piece must be the king
        // move king from from to to square
        unsetPiece(from);
        setPiece(to, piece);
        // move rook in the counter direction
        Square rookFrom;
        Square rookTo;
        if (flag == KINGSIDE_CASTLE) {
            if (us == WHITE) {
                rookFrom = H1;
                rookTo = F1;
            } else {
                rookFrom = H8;
                rookTo = F8;
            }
        } else {
            if (us == WHITE) {
                rookFrom = A1;
                rookTo = D1;
            } else {
                rookFrom = A8;
                rookTo = D8;
            }
        }
        unsetPiece(rookFrom);
        setPiece(rookTo, makePiece(us, ROOK));
    } else {
        // remove the moving piece from its old location
        unsetPiece(from);

        // handle promotions
        if (move.isPromotion()) {
            // handle captures
            if (move.isCapture()) {
                replacePiece(to, makePiece(us, move.getPromotionPieceType()));
            } else {
                setPiece(to, makePiece(us, move.getPromotionPieceType()));
            }
        } else {
            // handle en passant captures
            if (move.isEnPassantCapture()) {
                // remove the pawn that has passed us
                Bitboard enPassantTargetBB = getCurrentEnPassantTarget();
                Square enPassantTarget = bb::getLeastSignificantBitIndex(enPassantTargetBB);
                unsetPiece(enPassantTarget + ((us == WHITE) ? SOUTH : NORTH));
                setPiece(to, piece);
            } else if (move.isCapture()) {
                replacePiece(to, piece);
            } else {
                setPiece(to, piece);
            }
        }
    }

    // store captured piece in the new game state
    newGameState.capturedPiece = captured;

    // store en passant target in the new game state
    if (move.isDoublePawnPush()) {
        Bitboard enPassantTarget = 0ULL;
        bb::setBit(enPassantTarget, (us == WHITE) ? from + NORTH : from + SOUTH);
        newGameState.enPassantTarget = enPassantTarget;
    } else {
        newGameState.enPassantTarget = 0ULL;
    }

    // withdraw all castling rights if king moves (this is also done when actually castling, which is correct)
    if (typeOf(piece) == KING) {
        if (us == WHITE) {
            newGameState.castlingRights &= ~WHITE_CASTLING;
        } else {
            newGameState.castlingRights &= ~BLACK_CASTLING;
        }
    }

    // withdraw castling rights if rook moves
    if (typeOf(piece) == ROOK) {
        if (us == WHITE && from == H1) {
            newGameState.castlingRights &= ~WHITE_KINGSIDE_CASTLING;
        } else if (us == WHITE && from == A1) {
            newGameState.castlingRights &= ~WHITE_QUEENSIDE_CASTLING;
        } else if (us == BLACK && from == H8) {
            newGameState.castlingRights &= ~BLACK_KINGSIDE_CASTLING;
        } else if (us == BLACK && from == A8) {
            newGameState.castlingRights &= ~BLACK_QUEENSIDE_CASTLING;
        }
    }

    // withdraw castling rights if rook is captured
    if (typeOf(captured) == ROOK) {
        if (us == BLACK && to == H1) {
            newGameState.castlingRights &= ~WHITE_KINGSIDE_CASTLING;
        } else if (us == BLACK && to == A1) {
            newGameState.castlingRights &= ~WHITE_QUEENSIDE_CASTLING;
        } else if (us == WHITE && to == H8) {
            newGameState.castlingRights &= ~BLACK_KINGSIDE_CASTLING;
        } else if (us == WHITE && to == A8) {
            newGameState.castlingRights &= ~BLACK_QUEENSIDE_CASTLING;
        }
    }

    // flip side to move
    newGameState.sideToMove = !oldGameState.sideToMove;

    // push new game state to stack
    m_gameStateHistory.push_back(newGameState);
}

void Board::unmakeMove(Move move) {

    // store and pop the game state introduced by the move to be unmade
    GameState state = m_gameStateHistory.back();
    m_gameStateHistory.pop_back();

    Color us = getSideToMove();
    Color them = !us;
    Square from = move.getFrom();
    Square to = move.getTo();
    Piece piece = m_pieces[to];
    Piece captured = state.capturedPiece;

    if (move.isCastling()) {
        MoveFlag flag = move.getFlag();
        assert(typeOf(piece) == KING);    // moving piece must be the king
        // move king from to to from square
        unsetPiece(to);
        setPiece(from, piece);
        // move rook in the counter direction
        Square rookFrom;
        Square rookTo;
        if (flag == KINGSIDE_CASTLE) {
            if (us == WHITE) {
                rookFrom = H1;
                rookTo = F1;
            } else {
                rookFrom = H8;
                rookTo = F8;
            }
        } else {
            if (us == WHITE) {
                rookFrom = A1;
                rookTo = D1;
            } else {
                rookFrom = A8;
                rookTo = D8;
            }
        }
        unsetPiece(rookTo);
        setPiece(rookFrom, makePiece(us, ROOK));
    } else {
        if (move.isPromotion()) {
            // handle captures
            if (move.isCapture()) {
                replacePiece(to, captured);
            } else {
                unsetPiece(to);
            }
            // place a pawn on the move's original square
            setPiece(from, makePiece(us, PAWN));
        } else {
            // restore en passant captures
            if (move.isEnPassantCapture()) {
                // put a pawn back on the field, one in front of the capture field
                setPiece((us == WHITE) ? (to + SOUTH) : (to + NORTH), makePiece(them, PAWN));
                unsetPiece(to);
            } else if (move.isCapture()) {
                // handle captures
                replacePiece(to, captured);
            } else {
                unsetPiece(to);
            }
            // place the moving piece on the move's original square
            setPiece(from, piece);
        }
    }
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