#include "utils.h"

namespace chess {
namespace utils {

uint32_t randomState = 856839613;

std::vector<std::string> splitFen(const std::string& fen) {
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
}

uint32_t getRandom32() {
    uint32_t number = randomState;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    randomState = number;
    return number;
}

uint64_t getRandom64() {
    uint64_t a, b, c, d;
    a = (uint64_t) (getRandom32() & 0xFFFF);
    b = (uint64_t) (getRandom32() & 0xFFFF);
    c = (uint64_t) (getRandom32() & 0xFFFF);
    d = (uint64_t) (getRandom32() & 0xFFFF);
    return a | (b << 16) | (c << 32) | (d << 48);
}

uint64_t getRandom64Sparse() {
    uint64_t number = getRandom64() & getRandom64() & getRandom64();
    return number;
}

void printMove(Move move) {
    // print origin and target square
    std::cout << SQUARE_NAMES[move.getFrom()];
    std::cout << " -> ";
    std::cout << SQUARE_NAMES[move.getTo()];
    std::cout << " ";

    // print move bitsets
    std::cout << "[" << std::bitset<4>(move.getFlag()) << " ";
    std::cout << std::bitset<6>(move.getTo()) << " ";
    std::cout << std::bitset<6>(move.getFrom()) << "]";
    std::cout << " ";

    // print move flag
    std::cout << FLAG_NAMES[move.getFlag()];
    std::cout << std::endl;
}

}   // namespace utils
}   // namespace chess