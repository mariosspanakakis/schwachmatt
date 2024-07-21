#include "main.h"

int main(int argc, char *argv[]) {

    // initialize attack tables for rapid lookup of attacked squares
    std::cout << "Initializing attack tables...";
    attacks::InitializeAttackTables();
    std::cout << " done!" << std::endl;

    // initialize and setup chess board
    Board board;
    try {
        board = Board("3qk1nr/1P3pP1/1rp3n1/p2p3p/2pPNPb1/P3P3/2QB3P/R3KBNR b KQk - 0 21");
    } catch(std::runtime_error &err){
        std::cerr << "Exception caught: " << err.what();
        return 1;
    }

    // generate moves
    bb::Color color = bb::BLACK;
    bb::Piece piece = bb::ROOK;
    MoveList movelist;
    movegen::GeneratePieceMoves(board, piece, color, movelist);
    std::cout << "Found " << movelist.GetSize() << " moves:" << std::endl;
    for (int i = 0; i < movelist.GetSize(); i++) {
        mv::PrintMoveDetails(movelist.GetMove(i));
    }

    return 0;
}