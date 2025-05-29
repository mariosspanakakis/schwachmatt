#include "uci.hpp"

void UniversalChessInterface::start() {
    m_running = true;
    m_thread = std::thread(&UniversalChessInterface::listen, this);
}

void UniversalChessInterface::stop() {
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void UniversalChessInterface::listen() {
    while(m_running) {
        std::string line;
        while(std::getline(std::cin, line)) {
            Command command = Command(line);
            std::vector<std::string> args = command.args();

            std::cout << "Received command: " << std::endl;
            std::cout << "   " << command.raw() << std::endl;
            std::cout << "   args: [";
            for (size_t i = 0; i < args.size(); ++i) {
                std::cout << args[i];
                if (i < args.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;

            if (command.type() != UCI_INVALID_COMMAND) {
                std::lock_guard<std::mutex> lock(m_queue_mutex);
                m_command_queue.push(command);
            } else {
                std::cout << "Invalid command: " << command.raw() << std::endl;
            }
        }
    }
}

Command UniversalChessInterface::popCommand() {
    std::unique_lock<std::mutex> lock(m_queue_mutex);
    if (m_command_queue.empty()) {
        return Command("");
    }
    Command command = m_command_queue.front();
    m_command_queue.pop();
    return command;
}

Command::Command(std::string in) {
    m_raw = in;
    std::vector<std::string> tokens = utils::tokenize(in, ' ');
    m_args = std::vector<std::string>(tokens.begin() + 1, tokens.end());
    
    std::string cmd = tokens.front();
    if (cmd == "uci") {
        m_type = UCI_INIT;
    } else if (cmd == "ucinewgame") {
        m_type = UCI_NEWGAME;
    } else if (cmd == "isready") {
        m_type = UCI_ISREADY;
    } else if (cmd == "position") {
        m_type = UCI_POSITION;
    } else if (cmd == "go") {
        m_type = UCI_GO;
    } else if (cmd == "stop") {
        m_type = UCI_STOP;
    } else if (cmd == "quit") {
        m_type = UCI_QUIT;
    } else if (cmd == "debug") {
        m_type = UCI_DEBUG;
    } else {
        m_type = UCI_INVALID_COMMAND;
    }
}
