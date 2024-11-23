#include <iostream>

#include "uci.h"
#include "board.h"

int main() {
    /* Initialize engine. */
    //chess::attacks::initializeAttackTables();

    // set up the UCI communication
    UniversalChessInterface uci;
    uci.operate();

    // TODO: add producer thread that adds all received commands to a command queue
    // TODO: add consumer thread that subsequently calls and handles the commands in the queue

    // NOTE: maybe use dispatch functions that are called for each command?

    return 0;
}