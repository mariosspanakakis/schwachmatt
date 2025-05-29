#include "main.hpp"

std::atomic<bool> running(true);
void shutdown(int signum) {
    running = false;
    std::cout << "Shutting down..." << std::endl;
}

int main(int argc, char* argv[]) {
    /* Connect shutdown signal to signal handler. */
    std::signal(SIGINT, shutdown);

    /* Start the engine. */
    Engine engine;
    engine.start();

    /* Keep the program running while all operation happens in threads. */
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    engine.stop();
    return 0;
}