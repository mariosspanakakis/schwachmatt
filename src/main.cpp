#include "main.h"

int main(int argc, char *argv[]) {
    try {
        // initialize attack tables for rapid lookup of attacked squares
        std::cout << "Initializing attack tables... ";
        chess::attacks::initializeAttackTables();
        std::cout << "done!" << std::endl;
        
        // initialize chess board
        chess::Board board;
        //board = chess::Board("r3kb1r/p1N2ppp/2pp1q1n/1p2p1B1/Q2PP3/3B1b2/PP3PPP/2R1K2R b Kkq - 1 12");
        board = chess::Board();

        board.print();

        // generate moves
        chess::MoveList movelist = chess::MoveList<chess::BLACK>(board);
        std::cout << "Found " << movelist.size() << " moves:" << std::endl;
        for (const chess::Move& move : movelist) {
            chess::utils::printMove(move);
        }

        /*chess::Square kingSquare = chess::bb::getLeastSignificantBitIndex(board.getPieceBitboard(chess::KING, color));
        bool inCheck = board.isAttackedBy(kingSquare, !color);
        if (inCheck) {
            std::cout << "King is in check." << std::endl;
        } else {
            std::cout << "King is not in check." << std::endl;
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