#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

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
