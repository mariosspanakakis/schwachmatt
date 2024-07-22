#include "main.h"

int main(int argc, char *argv[]) {

    // initialize attack tables for rapid lookup of attacked squares
    std::cout << "Initializing attack tables...";
    attacks::initializeAttackTables();
    std::cout << " done!" << std::endl;

    // initialize and setup chess board
    Board board;
    try {
        board = Board("r3kbnr/ppp2ppp/2np1q2/4p3/2BPP1b1/2N2N2/PPP2PPP/R1BQK2R w KQkq - 4 6");
    } catch(std::runtime_error &err){
        std::cerr << "Exception caught: " << err.what();
        return 1;
    }

    // generate moves
    bb::Color color = bb::WHITE;
    bb::Piece piece = bb::KING;
    MoveList movelist;
    movegen::generatePieceMoves(board, piece, color, movelist);
    std::cout << "Found " << movelist.getSize() << " moves:" << std::endl;
    for (int i = 0; i < movelist.getSize(); i++) {
        mv::printMoveDetails(movelist.getMove(i));
    }

    return 0;
}