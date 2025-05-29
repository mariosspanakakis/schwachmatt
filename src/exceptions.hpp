#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

class InvalidFENException : public std::runtime_error {
public:
    InvalidFENException(const std::string& message)
        : std::runtime_error(message) {}
};

class MagicNotFoundException : public std::runtime_error {
public:
    MagicNotFoundException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif
