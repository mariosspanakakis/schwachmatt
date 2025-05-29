#ifndef MAIN_HPP
#define MAIN_HPP

#include <atomic>
#include <csignal>
#include <iostream>
#include "engine.hpp"

int main(int argc, char* argv[]);
void shutdown(int signum);

#endif // MAIN_HPP