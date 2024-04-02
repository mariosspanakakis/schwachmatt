#include "utils.h"

namespace utils {

    uint32_t random_state = 856839613;
    
    std::vector<std::string> SplitFen(std::string fen) {
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

    uint32_t GetRandom32(){
        uint32_t number = random_state;
        number ^= number << 13;
        number ^= number >> 17;
        number ^= number << 5;
        random_state = number;
        return number;
    }

    bb::U64 GetRandom64(){
        bb::U64 a, b, c, d;
        a = (bb::U64) (GetRandom32() & 0xFFFF);
        b = (bb::U64) (GetRandom32() & 0xFFFF);
        c = (bb::U64) (GetRandom32() & 0xFFFF);
        d = (bb::U64) (GetRandom32() & 0xFFFF);
        return a | (b << 16) | (c << 32) | (d << 48);
    }

    bb::U64 GetRandom64Sparse(){
        bb::U64 number = GetRandom64() & GetRandom64() & GetRandom64();
        return number;
    }
}