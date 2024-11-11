#include "uci.h"

void UniversalChessInterface::operate() {
    std::string line;
    while(std::getline(std::cin, line)) {
        process(line);
    }
}

void UniversalChessInterface::process(std::string input) {
    // remove additional white spaces and separate the string into tokens
    std::vector<std::string> tokens = split(input);

    // process the given commands
    std::string command = tokens.at(0);
    if (command == "uci") {                         // initial command
        std::cout << "id name Schwachmatt" << std::endl;
        std::cout << "id author Marios Spanakakis" << std::endl;
        std::cout << "uciok" << std::endl;
    } else if (command == "ucinewgame") {           // start new game
        // TODO: handle
    } else if (command == "isready") {              // respond when ready
        // TODO: handle
    } else if (command == "position") {             // set up position
        // TODO: handle
    } else if (command == "go") {                   // start searching
        // TODO: handle
    } else if (command == "stop") {                 // stop searching
        // TODO: handle
    } else if (command == "quit") {                 // quit operation
        // TODO: handle
    } else if (command == "debug") {                // switch debug mode
        // TODO: handle
    }
}