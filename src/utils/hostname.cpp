#include "utils/hostname.hpp"

#include <unistd.h>

#include <cstdlib>

namespace utils {

std::string getHostname() {
    char buff[32];

#ifdef _WIN32
    GetComputerName(buff, 32);
#else
    gethostname(buff, 32);
#endif

    return std::string(buff);
}

}  // namespace utils