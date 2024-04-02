#include "board.h"

Board::Board(const std::string& fen){
    // initialize empty bitboards for all pieces
    for (int i = 0; i < bb::N_PIECES; i++){
        m_pieces_BB[i] = 0ULL;
    }
    for (int i = 0; i < bb::N_COLORS; i++){
        m_occupancy_BB[i] = 0ULL;
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
            int index_offset = 0;
            is_white ? index_offset = 0 : index_offset = 6;

            // update the occupancy bitboard in accordance to the pieces
            switch (figure){
                case 'p':
                    bb::SetBit(m_pieces_BB[bb::PAWN_IDX + index_offset], square);
                    break;
                case 'n':
                    bb::SetBit(m_pieces_BB[bb::KNIGHT_IDX + index_offset], square);
                    break;
                case 'b':
                    bb::SetBit(m_pieces_BB[bb::BISHOP_IDX + index_offset], square);
                    break;
                case 'r':
                    bb::SetBit(m_pieces_BB[bb::ROOK_IDX + index_offset], square);
                    break;
                case 'q':
                    bb::SetBit(m_pieces_BB[bb::QUEEN_IDX + index_offset], square);
                    break;
                case 'k':
                    bb::SetBit(m_pieces_BB[bb::KING_IDX + index_offset], square);
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
    for (int i = 0; i < 6; i++){
        bb::U64 bb = m_pieces_BB[i];
        m_occupancy_BB[0] |= bb;
    }
    for (int i = 6; i < 12; i++){
        bb::U64 bb = m_pieces_BB[i];
        m_occupancy_BB[1] |= bb;
    }
    m_occupancy_combined_BB = m_occupancy_BB[0] | m_occupancy_BB[1];
};