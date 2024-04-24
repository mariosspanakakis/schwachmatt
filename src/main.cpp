#include "main.h"

int main(int argc, char *argv[]) {

    attacks::InitializeAttackTables();

    //std::string fen = board::INITIAL_FEN;
    try{
        Board board;
    }
    catch(std::runtime_error &err){
        std::cerr << "Exception caught: " << err.what();
        return 0;
    }

    Move move = Move(12, 46, 0);
    move.printMoveDetails();
    std::cout << std::bitset<16>(move.getFrom()) << std::endl;
    std::cout << std::bitset<16>(move.getTo()) << std::endl;

    return 0;
}