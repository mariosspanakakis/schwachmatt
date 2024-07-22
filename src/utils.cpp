#include "utils.h"

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

    uint32_t getRandom32(){
        uint32_t number = randomState;
        number ^= number << 13;
        number ^= number >> 17;
        number ^= number << 5;
        randomState = number;
        return number;
    }

    bb::U64 getRandom64(){
        bb::U64 a, b, c, d;
        a = (bb::U64) (getRandom32() & 0xFFFF);
        b = (bb::U64) (getRandom32() & 0xFFFF);
        c = (bb::U64) (getRandom32() & 0xFFFF);
        d = (bb::U64) (getRandom32() & 0xFFFF);
        return a | (b << 16) | (c << 32) | (d << 48);
    }

    bb::U64 getRandom64Sparse(){
        bb::U64 number = getRandom64() & getRandom64() & getRandom64();
        return number;
    }
}