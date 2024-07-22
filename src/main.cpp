#include "main.h"

int main(int argc, char *argv[]) {
    try {
        // initialize attack tables for rapid lookup of attacked squares
        std::cout << "Initializing attack tables... ";
        attacks::initializeAttackTables();
        std::cout << "done!" << std::endl;
        
        // initialize chess board
        Board board;
        board = Board("r3kb1r/p1N2ppp/2pp1q1n/1p2p1B1/Q2PP3/3B1b2/PP3PPP/2R1K2R b Kkq - 1 12");

        // generate moves
        bb::Color color = bb::BLACK;
        /*bb::Piece piece = bb::ROOK;
        MoveList movelist;
        movegen::generatePieceMoves(board, piece, color, movelist);*/
        MoveList movelist = movegen::generateMoves(board, color);
        std::cout << "Found " << movelist.getSize() << " moves:" << std::endl;
        for (int i = 0; i < movelist.getSize(); i++) {
            mv::printMoveDetails(movelist.getMove(i));
        }

        bb::Square kingSquare = bb::getLeastSignificantBitIndex(board.getPieceBitboard(bb::KING, color));
        bool inCheck = board.isAttackedBy(kingSquare, !color);
        if (inCheck) {
            std::cout << "King is in check." << std::endl;
        } else {
            std::cout << "King is not in check." << std::endl;
        }

        return 0;
    } catch(InvalidFENException &err) {
        std::cerr << "Error: " << err.what();
        return 1;
    } catch (MagicNotFoundException &err) {
        std::cerr << "Error: " << err.what();
        return 1;
    }
}