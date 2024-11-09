#include "uci.h"

void UniversalChessInterface::operate() {
    std::string line;
    while(std::getline(std::cin, line)) {
        process(line);
    }
}

void UniversalChessInterface::process(std::string input) {
    // remove additional white spaces and separate the string into tokens
    std::vector<std::string> commands = split(input);

    // echo the received commands
    /*for (std::string tkn : commands) {
        std::cout << tkn << std::endl;
    }*/

    // TODO: add proper command handling
    if (commands.at(0) == "uci") {
        std::cout << "uciok" << std::endl;
    }
}