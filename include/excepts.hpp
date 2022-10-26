#pragma once
#include "ui/feedback.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

using std::string;

namespace excepts {

// Usage: throw excepts::error(...)
class error : public std::exception {
private:
    const string msg;
    const string file;
    const string func;
    const string info;
    string what_string;
    char* what_msg;

public:
    error(  const string msg = "Runtime error!",
            const string file = "",
            const string func = "",
            const string info = "");

    const char* what() const throw();
};

}
