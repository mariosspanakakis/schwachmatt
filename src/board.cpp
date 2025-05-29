#include "board.hpp"

Board::Board(const std::string& fen){
    // initialize empty bitboards
    for (bool color : {WHITE, BLACK}){
        for (PieceType pieceType = 0; pieceType < N_PIECE_TYPES; pieceType++) {
            occupancies_.pieces[color][pieceType] = 0ULL;
        }
        occupancies_.colors[color] = 0ULL;
    }
    occupancies_.all = 0ULL;

    // initialize the array of pieces
    for (Square square = 0; square < N_SQUARES; square++) {
        pieces_[square] = NO_PIECE;
    }
    
    // split the given FEN into groups that describe the board status
    std::vector<std::string> fen_groups = utils::tokenize(fen, ' ');

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
            Square square = get_square(rank, file);

            // set offset for piece referencing
            Color color = is_white ? WHITE : BLACK;

            // update the occupancy bitboard in accordance to the pieces
            switch (figure){
                case 'p':
                    bb::set(occupancies_.pieces[color][PAWN], square);
                    pieces_[square] = is_white ? WHITE_PAWN : BLACK_PAWN;
                    break;
                case 'n':
                    bb::set(occupancies_.pieces[color][KNIGHT], square);
                    pieces_[square] = is_white ? WHITE_KNIGHT : BLACK_KNIGHT;
                    break;
                case 'b':
                    bb::set(occupancies_.pieces[color][BISHOP], square);
                    pieces_[square] = is_white ? WHITE_BISHOP : BLACK_BISHOP;
                    break;
                case 'r':
                    bb::set(occupancies_.pieces[color][ROOK], square);
                    pieces_[square] = is_white ? WHITE_ROOK : BLACK_ROOK;
                    break;
                case 'q':
                    bb::set(occupancies_.pieces[color][QUEEN], square);
                    pieces_[square] = is_white ? WHITE_QUEEN : BLACK_QUEEN;
                    break;
                case 'k':
                    bb::set(occupancies_.pieces[color][KING], square);
                    pieces_[square] = is_white ? WHITE_KING : BLACK_KING;
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
            Bitboard bb = occupancies_.pieces[color][pieceType];
            occupancies_.colors[color] |= bb;
        }
        occupancies_.all |= occupancies_.colors[color];
    }

    // instantiate game state object
    GameState gameState = GameState();

    // handle side to move
    std::string fen_side_to_move = fen_groups[1];
    gameState.side_to_move = fen_side_to_move.compare("w") == 0 ? WHITE : BLACK;

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
    gameState.castling_rights = castling_right;

    // handle en passant square
    std::string fen_en_passant = fen_groups[3]; // like "e3"
    gameState.en_passant_target = 0ULL;
    for (int i = 0; i < N_SQUARES; ++i) {
        if (SQUARE_NAMES[i] == fen_en_passant) {
            bb::set(gameState.en_passant_target, i);
        }
    }

    // TODO: handle move counters

    // push the initial game state onto the game state history stack
    game_state_history_.reserve(GAME_STATE_HISTORY_LENGTH);
    game_state_history_.push_back(gameState);
};

Bitboard Board::getPieceOccupancy(PieceType pieceType, Color color) const {
    return occupancies_.pieces[color][pieceType];
}

Bitboard Board::getColorOccupancy(Color color) const {
    return occupancies_.colors[color];
}

Bitboard Board::getTotalOccupancy() const {
    return occupancies_.all;
}

Piece Board::getPieceOnSquare(Square square) const {
    return pieces_[square];
}

Square Board::getKingSquare(Color color) const {
    return bb::getLSB(occupancies_.pieces[color][KING]);
}

Bitboard Board::getCurrentEnPassantTarget() const {
    return game_state_history_.back().en_passant_target;
}

CastlingRight Board::getCastlingRights() const {
    return game_state_history_.back().castling_rights;
}

bool Board::canCastle(CastlingRight cr) const {
    return (game_state_history_.back().castling_rights & cr);
}

bool Board::isCastlingBlocked(CastlingRight cr) const {
    return occupancies_.all & CASTLING_SQUARES[cr];
}

Color Board::getSideToMove() const {
    return game_state_history_.back().side_to_move;
}

void Board::setPiece(Square square, Piece piece) {
    //assert(pieces_[square] == NO_PIECE);  // assert that square is empty
    bb::set(occupancies_.pieces[color_of(piece)][type_of(piece)], square);
    bb::set(occupancies_.colors[color_of(piece)], square);
    bb::set(occupancies_.all, square);
    pieces_[square] = piece;
}

void Board::unsetPiece(Square square) {
    Piece piece = pieces_[square];
    //assert((piece != NO_PIECE));  // assert that square is not empty
    bb::clear(occupancies_.pieces[color_of(piece)][type_of(piece)], square);
    bb::clear(occupancies_.colors[color_of(piece)], square);
    bb::clear(occupancies_.all, square);
    pieces_[square] = NO_PIECE;
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
    Bitboard theirPawns = occupancies_.pieces[them][PAWN];
    if (attacks::getPawnAttacks(square, us) & theirPawns) {
        return true;
    }
    Bitboard theirKnights = occupancies_.pieces[them][KNIGHT];
    if (attacks::getPieceAttacks<KNIGHT>(square, 0ULL) & theirKnights) {
        return true;
    }
    Bitboard theirKing = occupancies_.pieces[them][KING];
    if (attacks::getPieceAttacks<KING>(square, 0ULL) & theirKing) {
        return true;
    }
    Bitboard theirBishopsAndQueens = occupancies_.pieces[them][BISHOP] | occupancies_.pieces[them][QUEEN];
    if (attacks::getPieceAttacks<BISHOP>(square, occupancies_.all) & theirBishopsAndQueens) {
        return true;
    }
    Bitboard theirRooksAndQueens = occupancies_.pieces[them][ROOK] | occupancies_.pieces[them][QUEEN];
    if (attacks::getPieceAttacks<ROOK>(square, occupancies_.all) & theirRooksAndQueens) {
        return true;
    }
    
    return false;
}

bool Board::isInCheck(Color color) {
    Square kingSquare = getKingSquare(color);
    return isAttackedBy(kingSquare, !color);
}

void Board::makeMove(Move move) {                                               // TODO: add a movePiece function

    Color us = getSideToMove();
    //Color them = !us;
    Square from = move.getFrom();
    Square to = move.getTo();
    Piece piece = pieces_[from];
    Piece captured = move.isEnPassantCapture() ? ((us == WHITE) ? BLACK_PAWN : WHITE_PAWN) : pieces_[to];

    GameState prev_game_state = game_state_history_.back();
    GameState next_game_state = GameState();
    next_game_state.castling_rights = prev_game_state.castling_rights;                  // TODO: implement copy constructor for game state that copies the relevant attributes
    next_game_state.captured = captured;
    next_game_state.side_to_move = !prev_game_state.side_to_move;

    if (move.isCastling()) {
        // make sure the moving piece is the king
        //assert(type_of(piece) == KING);    // moving piece must be the king

        // determine rook target square
        Square rookFrom = get_square(us * RANK_8, (from < to) ? FILE_H : FILE_A);
        Square rookTo = (from < to) ? to + WEST : to + EAST;

        // move the king and the rook
        unsetPiece(from);
        setPiece(to, piece);
        unsetPiece(rookFrom);
        setPiece(rookTo, make_piece(us, ROOK));
    } else {
        // remove the moving piece from its old location
        unsetPiece(from);

        // handle promotions
        if (move.isPromotion()) {
            // handle captures
            if (move.isCapture()) {
                replacePiece(to, make_piece(us, move.getPromotionPieceType()));
            } else {
                setPiece(to, make_piece(us, move.getPromotionPieceType()));
            }
        } else {
            // handle en passant captures
            if (move.isEnPassantCapture()) {
                // remove the pawn that has passed us
                Bitboard enPassantTargetBB = getCurrentEnPassantTarget();
                Square enPassantTarget = bb::getLSB(enPassantTargetBB);
                unsetPiece(enPassantTarget + ((us == WHITE) ? SOUTH : NORTH));
                setPiece(to, piece);
            } else if (move.isCapture()) {
                replacePiece(to, piece);
            } else {
                setPiece(to, piece);
            }
        }
    }

    // store en passant target in the new game state
    if (move.isDoublePawnPush()) {
        next_game_state.en_passant_target = (us == WHITE) ? bb::getBitboard(from + NORTH) : bb::getBitboard(from + SOUTH);
    } else {
        next_game_state.en_passant_target = 0ULL;
    }

    // withdraw all castling rights if king moves (this is also done when actually castling, which is correct)
    if (type_of(piece) == KING) {
        next_game_state.castling_rights &= ~(us & ANY_CASTLING);
    }

    // withdraw castling rights if rook moves away from start square
    if (type_of(piece) == ROOK) {
        if (from == H1 || from == H8) {
            next_game_state.castling_rights &= ~(us & KINGSIDE_CASTLING);
        } else if (from == A1 || from == A8) {
            next_game_state.castling_rights &= ~(us & QUEENSIDE_CASTLING);
        }
    }

    // withdraw castling rights if rook is captured                             // NOTE: this seems to have a bug!
    if (type_of(captured) == ROOK) {

        // NOTE: This should be equivalent to what comes below. However, it fails
        //       for perft position 4 at depth 4.
        /*if (to == H1 || to == H8) {
            next_game_state.castlingRights &= ~(them & KINGSIDE_CASTLING);
        } else if (to == A1 || to == A8) {
            next_game_state.castlingRights &= ~(them & QUEENSIDE_CASTLING);
        }*/

        if (us == BLACK && to == H1) {
            next_game_state.castling_rights &= ~WHITE_KINGSIDE_CASTLING;
        } else if (us == BLACK && to == A1) {
            next_game_state.castling_rights &= ~WHITE_QUEENSIDE_CASTLING;
        } else if (us == WHITE && to == H8) {
            next_game_state.castling_rights &= ~BLACK_KINGSIDE_CASTLING;
        } else if (us == WHITE && to == A8) {
            next_game_state.castling_rights &= ~BLACK_QUEENSIDE_CASTLING;
        }
    }

    // push new game state to stack
    game_state_history_.push_back(next_game_state);
}

void Board::unmakeMove(Move move) {

    // store and pop the game state introduced by the move to be unmade
    GameState state = game_state_history_.back();
    game_state_history_.pop_back();

    Color us = getSideToMove();
    Color them = !us;
    Square from = move.getFrom();
    Square to = move.getTo();
    Piece piece = pieces_[to];
    Piece captured = state.captured;

    if (move.isCastling()) {
        assert(type_of(piece) == KING);    // moving piece must be the king

        // determine rook target square
        Square rookFrom = get_square(us * RANK_8, (from < to) ? FILE_H : FILE_A);
        Square rookTo = (from < to) ? to + WEST : to + EAST;

        // move king from to to from square
        unsetPiece(to);
        setPiece(from, piece);
        unsetPiece(rookTo);
        setPiece(rookFrom, make_piece(us, ROOK));
    } else {
        if (move.isPromotion()) {
            // handle captures
            if (move.isCapture()) {
                replacePiece(to, captured);
            } else {
                unsetPiece(to);
            }
            // place a pawn on the move's original square
            setPiece(from, make_piece(us, PAWN));
        } else {
            // restore en passant captures
            if (move.isEnPassantCapture()) {
                // put a pawn back on the field, one in front of the capture field
                setPiece((us == WHITE) ? (to + SOUTH) : (to + NORTH), make_piece(them, PAWN));
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

bool Board::isLegal(Move move) {
    // various situations to cover:
    // 1. if the king is moving: test whether the goal square is under attack
    // 2. if castling, ensure that none of the squares the king passes are under attack
    // 3. if en-passant capturing, ensure that the king does not end up in check
    // 4. for all other moves, test whether they unblock a sliding piece's attack to the king
    
    // what if the king is in check? could/should add a separate move GENERATION for this case 

    return true;
}

void Board::print() {
    std::cout << std::endl;
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << " " << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            int square = (rank * 8 + file);
            Piece piece = pieces_[square];
            std::cout << " " << PIECE_SYMBOLS[piece];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "     A B C D E F G H" << std::endl;
    std::cout << std::endl;

    // get written representation of castling rights
    std::string castling = std::string(canCastle(WHITE_KINGSIDE_CASTLING) ? "K" : "")
                         + std::string(canCastle(WHITE_QUEENSIDE_CASTLING) ? "Q" : "")
                         + std::string(canCastle(BLACK_KINGSIDE_CASTLING) ? "k" : "")
                         + std::string(canCastle(BLACK_QUEENSIDE_CASTLING) ? "q" : "");

    if (castling.compare("") == 0) {
        castling = "-";
    }

    // get written representation of en passant square
    Bitboard en_passant_target_bb = getCurrentEnPassantTarget();
    std::string enPassantSquare = en_passant_target_bb ? SQUARE_NAMES[bb::getLSB(en_passant_target_bb)] : "NONE";

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
