#include "main.h"

int main(int argc, char *argv[]) {

    attacks::InitializeAttackTables();

    try{
        Board board;
    }
    catch(std::runtime_error &err){
        std::cerr << "Exception caught: " << err.what();
        return 0;
    }

    Move move = Move(bb::E4, bb::G4, CAPTURE);
    move.PrintMoveDetails();

    return 0;
}