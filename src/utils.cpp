#include "utils.hpp"

namespace utils {

std::vector<std::string> tokenize(const std::string &s, char d) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, d)) {
        result.push_back(item);
    }

    return result;
}

std::string getProgressBar(int val, int total, int width) {
    assert(total > 0 && val >= 0 && val <= total);
    float percentage = static_cast<float>(val) / total;
    std::string bar = "[" + std::string(static_cast<int>(percentage * width), '#') + std::string(width - static_cast<int>(percentage * width), ' ') + "] " + 
                      std::to_string(static_cast<int>(percentage * 100)) + "%";
    return bar;
}

}  // namespace utils