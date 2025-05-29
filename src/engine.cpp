#include "engine.hpp"

Engine::Engine() : m_uci(), m_board(), m_running(true) {}

void Engine::start() {
    m_uci.start();
    m_engine_thread = std::thread(&Engine::operate, this);
}

void Engine::stop() {
    m_running = false;
    m_uci.stop();
    if (m_engine_thread.joinable()) {
        m_engine_thread.join();
    }
}

void Engine::operate() {
    while(m_running) {
        if (m_uci.hasCommand() == true) {
            Command command = m_uci.popCommand();
            processCommand(command);
            std::cout << "Processed command: " << command.raw() << std::endl;
        }
    }
}

void Engine::reset() {
    m_board = Board();
    if (!m_is_initialized) {
        std::cout << "Initializing engine, please wait..." << std::endl;
        // TODO: initialize all precalculated data structures
        attacks::precalculate(true);
        m_is_initialized = true;
    }
}

void Engine::setPosition(const std::string& fen, std::vector<Move> moves) {
    // TODO: set up the board and play the given move sequence,
    //       throw an error if the move sequence is invalid
}

void Engine::processCommand(Command command) {
    switch(command.type()) {
        case UCI_INIT: {  // initial command
            std::cout << "id name Schwachmatt" << std::endl;
            std::cout << "id author Marios Spanakakis" << std::endl;
            std::cout << "uciok" << std::endl;
            break;
        }
        case UCI_NEWGAME: {  // start new game
            reset();
            break;
        }
        case UCI_ISREADY: {  // respond when ready
            // TODO: start a thread that waits for the engine to be ready
            //       and then send the "readyok" message
            break;
        }
        case UCI_POSITION: {  // set up position
            // TODO: set up a position according to the given FEN and play the
            //       given moves
            break;
        }
        case UCI_GO: {  // start searching
            break;
        }
        case UCI_STOP: {  // stop searching
            break;
        }
        case UCI_QUIT: {  // quit operation
            break;
        }
        case UCI_DEBUG: {  // switch debug mode
            break;
        }
    }
}
