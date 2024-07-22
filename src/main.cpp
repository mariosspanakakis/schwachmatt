#include "main.h"

int main(int argc, char *argv[]) {

    // initialize attack tables for rapid lookup of attacked squares
    std::cout << "Initializing attack tables...";
    attacks::initializeAttackTables();
    std::cout << " done!" << std::endl;

    // initialize and setup chess board
    Board board;
    try {
        board = Board("3q2nr/1P1NkpP1/1rp3n1/p2p3p/2pP1P2/P3Pb2/2QB3P/R3KBNR b KQ - 4 23");
    } catch(std::runtime_error &err){
        std::cerr << "Exception caught: " << err.what();
        return 1;
    }

    // generate moves
    bb::Color color = bb::BLACK;
    bb::Piece piece = bb::KING;
    MoveList movelist;
    movegen::generatePieceMoves(board, piece, color, movelist);
    std::cout << "Found " << movelist.getSize() << " moves:" << std::endl;
    for (int i = 0; i < movelist.getSize(); i++) {
        mv::printMoveDetails(movelist.getMove(i));
    }

    return 0;
}