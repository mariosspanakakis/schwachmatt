#include "board.h"

Board::Board(const std::string& fen){
    // initialize empty bitboards for all pieces
    for (bool color : {bb::WHITE, bb::BLACK}){
        for (bb::Piece piece = 0; piece < bb::N_PIECES; piece++) {
            m_pieces_BB[color][piece] = 0ULL;
        }
        m_occupancy_BB[color] = 0ULL;
    }
    m_occupancy_combined_BB = 0ULL;
    
    // split the given FEN into groups that describe the board status
    std::vector<std::string> fen_groups = utils::splitFen(fen);

    // loop through FEN and set up pieces as specified, note that FEN notation
    // counts ranks downwards but files upwards!
    std::string fen_pieces = fen_groups[0];
    int rank = bb::RANK_8;
    int file = bb::FILE_A;
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
            bb::Color color = is_white ? bb::WHITE : bb::BLACK;

            // update the occupancy bitboard in accordance to the pieces
            switch (figure){
                case 'p':
                    bb::setBit(m_pieces_BB[color][bb::PAWN], square);
                    break;
                case 'n':
                    bb::setBit(m_pieces_BB[color][bb::KNIGHT], square);
                    break;
                case 'b':
                    bb::setBit(m_pieces_BB[color][bb::BISHOP], square);
                    break;
                case 'r':
                    bb::setBit(m_pieces_BB[color][bb::ROOK], square);
                    break;
                case 'q':
                    bb::setBit(m_pieces_BB[color][bb::QUEEN], square);
                    break;
                case 'k':
                    bb::setBit(m_pieces_BB[color][bb::KING], square);
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
        throw std::runtime_error(std::string("Invalid FEN"));
    }

    // update the occupancy bitboards
    for (bool color : {bb::WHITE, bb::BLACK}) {
        for (bb::Piece piece = 0; piece < bb::N_PIECES; piece++) {
            bb::U64 bb = m_pieces_BB[color][piece];
            m_occupancy_BB[color] |= bb;
        }
        m_occupancy_combined_BB |= m_occupancy_BB[color];
    }

    // push the initial game state onto the game state history stack
    m_game_state_history.reserve(512);
    m_game_state_history.push_back(initial_game_state);
};

bb::U64 Board::getPieceBitboard(bb::Piece piece, bb::Color color) {
    return m_pieces_BB[color][piece];
}

bb::U64 Board::getOccupancyBitboard(bb::Color color) {
    return m_occupancy_BB[color];
}

bb::U64 Board::getCombinedOccupancyBitboard() {
    return m_occupancy_combined_BB;
}
    
bb::U64 Board::getCurrentEnPassantTarget() {
    return m_game_state_history.back().enPassantTarget;
}

bool Board::getCastlingRight(uint8_t castling_right) {
    return (m_game_state_history.back().castlingRights & castling_right);
}