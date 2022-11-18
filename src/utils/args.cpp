#include "utils/args.hpp"

#include <iostream>
#include <string>

namespace utils {

void parseArgs(int argc, char* argv[]) {
    // Simple example
    // Full parser is not finished yet
    for (int i = 0; i < argc; i++)
        if (auto arg = static_cast<std::string>(argv[i]); arg == "--headless" || arg == "-c")
            runtime::FLAG_headless = true;
}

}  // namespace utils