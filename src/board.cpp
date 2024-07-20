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
    std::vector<std::string> fen_groups = utils::SplitFen(fen);

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
            int square = bb::ConvertCoordToSquare(rank, file);

            // set offset for piece referencing
            bb::Color color = is_white ? bb::WHITE : bb::BLACK;

            // update the occupancy bitboard in accordance to the pieces
            switch (figure){
                case 'p':
                    bb::SetBit(m_pieces_BB[color][bb::PAWN], square);
                    break;
                case 'n':
                    bb::SetBit(m_pieces_BB[color][bb::KNIGHT], square);
                    break;
                case 'b':
                    bb::SetBit(m_pieces_BB[color][bb::BISHOP], square);
                    break;
                case 'r':
                    bb::SetBit(m_pieces_BB[color][bb::ROOK], square);
                    break;
                case 'q':
                    bb::SetBit(m_pieces_BB[color][bb::QUEEN], square);
                    break;
                case 'k':
                    bb::SetBit(m_pieces_BB[color][bb::KING], square);
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
};

bb::U64 Board::GetPieceBitboard(bb::Piece piece, bb::Color color) {
    return m_pieces_BB[color][piece];
}

bb::U64 Board::GetOccupancyBitboard(bb::Color color) {
    return m_occupancy_BB[color];
}

bb::U64 Board::GetCombinedOccupancyBitboard() {
    return m_occupancy_combined_BB;
}