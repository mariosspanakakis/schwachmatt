#include "utils.hpp"

namespace utils {

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
}