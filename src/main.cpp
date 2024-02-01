#include "main.hpp"

int main(int argc, char *argv[]) {

    attacks::initialize_attack_tables();

    std::string fen = board::INITIAL_FEN;
    try{
        Board board(fen);
    }
    catch(std::runtime_error &err){
        std::cerr << "Exception caught: " << err.what();
        return 0;
    }
    return 0;
}