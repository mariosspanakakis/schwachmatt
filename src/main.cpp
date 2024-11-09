#include <iostream>

#include "uci.h"
#include "board.h"

int main() {
    // NOTE: it might make sense to add an Engine() object as an additional
    //       layer of abstraction, that would take care of initialization, board
    //       management, search etc.
    // ChessEngine engine;
    // engine.init();

    // initialize attack tables for rapid lookup of attacked squares
    //chess::attacks::initializeAttackTables();

    // set up the UCI communication
    UniversalChessInterface uci;
    uci.operate();

    return 0;
}