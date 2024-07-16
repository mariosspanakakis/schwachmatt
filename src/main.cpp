#include "main.h"

int main(int argc, char *argv[]) {

    // initialize attack tables for rapid lookup
    attacks::InitializeAttackTables();

    // initialize and setup chess board
    Board board;
    try {
        board = Board();
    } catch(std::runtime_error &err){
        std::cerr << "Exception caught: " << err.what();
        return 1;
    }

    movegen::GenerateKnightMoves(board, bb::WHITE);

    return 0;
}