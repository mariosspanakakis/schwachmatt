#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>

namespace utils {

// split a string into tokens separated by the given delimiter
std::vector<std::string> tokenize(const std::string& s, char d);

}   // namespace utils

#endif