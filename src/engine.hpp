#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include "attacks.hpp"
#include "board.hpp"
#include "move.hpp"
#include "uci.hpp"

class Engine {
public:
    Engine();
    ~Engine() = default;
    void start();
    void stop();
    void operate();
    void reset();
private:
    std::thread m_engine_thread;
    
    UniversalChessInterface m_uci;
    Board m_board;

    std::atomic<bool> m_running = true;

    bool m_is_initialized = false;

    void processCommand(Command command);

    void setPosition(const std::string& fen, std::vector<Move> moves);
};
