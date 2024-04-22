#pragma once
#include <exception>
#include <string>

class Exception: public std::exception {
    const std::string message;
public:
    virtual const char* what() const throw() override;
    Exception(std::string message);
};