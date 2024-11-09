#ifndef UCI_H
#define UCI_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/*
GUI     -> uci ->           ENGINE
GUI     <- uciok <-         ENGINE
GUI     -> isready ->       ENGINE
GUI     <- isreadyok <-     ENGINE
*/

class UniversalChessInterface {
    private:
    void process(std::string command);
    
    public:
    void operate();
};

// split a string into tokens separated by whitespaces
inline std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> groups;
    std::string token;
    std::istringstream iss(s);

    while (iss >> token) {
        groups.push_back(token);
    }

    return groups;
}

#endif