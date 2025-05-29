#ifndef UCI_HPP
#define UCI_HPP

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include "utils.hpp"

enum CommandType {
    UCI_INIT,
    UCI_NEWGAME,
    UCI_ISREADY,
    UCI_POSITION,
    UCI_GO,
    UCI_STOP,
    UCI_QUIT,
    UCI_DEBUG,
    UCI_INVALID_COMMAND
};

class Command {
public:
    Command(std::string in);
    std::string raw() const { return m_raw; }
    CommandType type() const { return m_type; }
    std::vector<std::string> args() const { return m_args; }
private:
    std::string m_raw;
    CommandType m_type;
    std::vector<std::string> m_args;
};

class UniversalChessInterface {
public:
    void start();
    void stop();
    Command popCommand();
    bool hasCommand() {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        return !m_command_queue.empty();
    }
private:
    std::queue<Command> m_command_queue;
    std::mutex m_queue_mutex;

    std::atomic<bool> m_running{true};
    std::thread m_thread;
    void listen();
};

#endif
