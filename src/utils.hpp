#ifndef UTILS_HPP
#define UTILS_HPP

#include <cassert>
#include <iostream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>

namespace utils {

// split a string into tokens separated by the given delimiter
std::vector<std::string> tokenize(const std::string& s, char d);

std::string getProgressBar(int val, int total, int width = 50);

}  // namespace utils

#endif