#ifndef UCI_H
#define UCI_H

#include <numeric>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utils.h"

class Command {
    public:
    std::string type;
    std::vector<std::string> args;

    Command(std::string in);
    std::string serialize();
};

class UniversalChessInterface {
    private:
    void process(Command command);
    
    public:
    void operate();
};

#endif
