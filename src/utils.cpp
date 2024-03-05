#include "utils.hpp"

namespace utils {

    // initialize global random state for random number generation
    bb::U64 random_state_64 = 1853193856239832ULL;
    
    std::string* split_fen(std::string fen){
        // a FEN consists of 6 groups that are seperated by whitespaces
        static std::string groups[6];
        char delimiter = ' ';
        int current_group = 0;

        // loop through FEN and extract groups
        int start = 0;
        int end = 0;
        for (uint64_t i = 0; i <= fen.length(); i++){
            if (fen[i] == delimiter || i == fen.length()){
                // extract substring from last to current index
                end = i;
                std::string substring = fen.substr(start, end - start);
                start = end + 1;
                // store substring in group array
                groups[current_group] = substring;
                current_group += 1;
            }
        }

        return groups;
    }

    // get pseudo-random 64-bit number through xorshift64 algorithm
    bb::U64 get_random_64_bit(){
        random_state_64 ^= random_state_64 << 13;
        random_state_64 ^= random_state_64 >> 7;
        random_state_64 ^= random_state_64 << 17;
        return random_state_64;
    }

    // get pseudo-random 64-bit number with few populated bits
    bb::U64 get_random_sparse_64_bit(){
        bb::U64 number = get_random_64_bit() & get_random_64_bit();
        return number;
    }
}