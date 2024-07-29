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
        //std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";   // standard initial FEN
        std::string fen = "4k3/8/8/8/8/8/8/R3K2R w KQkq - 0 1";                 // setup for white castling
        chess::Board board = chess::Board(fen);

        board.print();

        // generate moves
        chess::MoveList movelist = chess::MoveList(board);
        std::cout << "Found " << movelist.size() << " moves:" << std::endl;
        for (const chess::Move& move : movelist) {
            std::cout << move.toString() << std::endl;
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