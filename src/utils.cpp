#include "utils.h"

namespace utils {

    // initialize global random state for random number generation
    bb::U64 random_state_64 = 1853193856239832ULL;
    
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

    // get pseudo-random 64-bit number through xorshift64 algorithm
    bb::U64 GetRandom64(){
        random_state_64 ^= random_state_64 << 13;
        random_state_64 ^= random_state_64 >> 7;
        random_state_64 ^= random_state_64 << 17;
        return random_state_64;
    }

    // get pseudo-random 64-bit number with few populated bits
    bb::U64 GetRandom64Sparse(){
        bb::U64 number = GetRandom64() & GetRandom64();
        return number;
    }
}