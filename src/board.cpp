#include "board.hpp"

// TODO: add and update game status (castlings rights, side to move etc.)
Board::Board(const std::string& fen){
    // initialize all bitboards as empty
    for (int i = 0; i < bb::N_PIECES; i++){
        m_pieces_BB[i] = 0ULL;
    }
    for (int i = 0; i < bb::N_COLORS; i++){
        m_occupancy_BB[i] = 0ULL;
    }
    m_occupancy_combined_BB = 0ULL;
    
    // split the given FEN into groups that describe the board status           // TODO: this looks horrible
    std::string* groups = utils::split_fen(fen);
    std::string fen_pieces = groups[0];
    std::string fen_move_rights = groups[1];
    std::string fen_castling_rights = groups[2];
    std::string fen_en_passant_square = groups[3];
    std::string fen_halfmove = groups[4];
    std::string fen_fullmove = groups[5];


    // loop through FEN and set up pieces as specified, note that FEN notation
    // counts ranks downwards but files upwards!
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
            int square = bb::coord_to_square(rank, file);

            int index_offset = 0;
            is_white ? index_offset = 0 : index_offset = 6;

            switch (figure){
                case 'p':
                    bb::set_bit(m_pieces_BB[bb::PAWN_IDX + index_offset], square);
                    break;
                case 'n':
                    bb::set_bit(m_pieces_BB[bb::KNIGHT_IDX + index_offset], square);
                    break;
                case 'b':
                    bb::set_bit(m_pieces_BB[bb::BISHOP_IDX + index_offset], square);
                    break;
                case 'r':
                    bb::set_bit(m_pieces_BB[bb::ROOK_IDX + index_offset], square);
                    break;
                case 'q':
                    bb::set_bit(m_pieces_BB[bb::QUEEN_IDX + index_offset], square);
                    break;
                case 'k':
                    bb::set_bit(m_pieces_BB[bb::KING_IDX + index_offset], square);
                    break;
            }

            file += 1;
        }
        // reset file and rank after having completed a rank
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