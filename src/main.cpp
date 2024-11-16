#include "main.h"

int main(int argc, char *argv[]) {
    try {

        std::random_device rd;

        // random number engine for move selection
        std::default_random_engine eng(rd());

        // initialize attack tables for rapid lookup of attacked squares
        std::cout << "Initializing attack tables... ";
        chess::attacks::initializeAttackTables();
        std::cout << "done!" << std::endl;
        
        // initialize chess board
        std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";   // standard position
        //std::string fen = "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1";               // all castling allowed
        //std::string fen = "4k3/8/2q5/8/8/8/8/R3K2R w KQ - 0 1";                  // white castling on queen side inhibited
        //std::string fen = "4k3/8/2r5/8/8/8/8/R3K1NR w KQ - 0 1";                // white castling on king side blocked and on queen side inhibited
        //std::string fen = "k2r4/4P1P1/8/8/8/8/8/K7 w - - 0 1";                  // white can capture or promotion-capture
        //std::string fen = "rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3";  // white can en-passant capture on f6
        chess::Board board = chess::Board(fen);

        board.print();

        chess::Bitboard attackers = board.getAttackers(chess::F3, chess::WHITE);
        chess::bb::printBitboard(attackers); 

        // generate moves
        chess::MoveList movelist = chess::MoveList(board);
        std::cout << "Found " << movelist.size() << " moves:" << std::endl;
        for (chess::Move move : movelist) {
            move.printDetails();
            //board.makeMove(move);
            //board.print();
            //board.unmakeMove(move);
            //board.print();
            //std::cout << std::endl;
        }

        /*while(true) {
            // get number of move to make
            std::uniform_int_distribution<size_t> distr(0, movelist.size() - 1);
            size_t i = distr(eng);
            chess::Move move = movelist[i];
            move.printDetails();
            board.makeMove(move);
            board.print();
        }*/

        return 0;
    } catch(InvalidFENException &err) {
        std::cerr << "Error: " << err.what();
        return 1;
    } catch (MagicNotFoundException &err) {
        std::cerr << "Error: " << err.what();
        return 1;
    }
}