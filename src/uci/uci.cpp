#include "uci.h"

void UniversalChessInterface::operate() {
    std::string line;
    while(std::getline(std::cin, line)) {
        Command command = Command(line);

        std::cout << "Received command: " << command.serialize() << std::endl;
        std::cout << "   type: " << command.type << std::endl;
        std::cout << "   args: [";
        for (std::string arg : command.args) {
            std::cout << arg << "+";
        }
        std::cout << "]" << std::endl;
        process(command);
    }
}

void UniversalChessInterface::process(Command command) {

    // process the given command
    if (command.type == "uci") {                         // initial command
        std::cout << "id name Schwachmatt" << std::endl;
        std::cout << "id author Marios Spanakakis" << std::endl;
        std::cout << "uciok" << std::endl;
    } else if (command.type == "ucinewgame") {           // start new game
        // TODO: handle
    } else if (command.type == "isready") {              // respond when ready
        // TODO: handle
    } else if (command.type == "position") {             // set up position
        // TODO: handle
    } else if (command.type == "go") {                   // start searching
        // TODO: handle
    } else if (command.type == "stop") {                 // stop searching
        // TODO: handle
    } else if (command.type == "quit") {                 // quit operation
        // TODO: handle
    } else if (command.type == "debug") {                // switch debug mode
        // TODO: handle
    }
}

Command::Command(std::string in) {
    std::vector<std::string> tokens = utils::tokenize(in, ' ');
    type = tokens.front();
    tokens.erase(tokens.begin());
    args = tokens;
}

std::string Command::serialize() {
    std::string del = " ";
    return type + del + std::accumulate(args.begin(), args.end(), del);
}
