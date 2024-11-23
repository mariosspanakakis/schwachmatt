#include "utils.h"

namespace utils {

/*std::vector<std::string> splitFen(const std::string& fen) {
    // initialize vector to store the subgroups
    std::vector<std::string> groups;
    // convert FEN to string stream to conveniently iterate through it
    std::istringstream iss(fen);
    // split FEN into substrings that are separated by whitespaces
    std::string token;
    while (std::getline(iss, token, ' ')) {
        groups.push_back(token);
    }
    // return substrings
    return groups;
}*/

std::vector<std::string> tokenize(const std::string &s, char d) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, d)) {
        result.push_back(item);
    }

    return result;
}

}   // namespace utils