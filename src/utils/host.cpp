#include "utils/host.hpp"

#include <windows.h>


#include <cstdlib>
#include <ctime>
#include <iostream>

namespace utils {

std::string getHostname() {
    char buff[32];

#ifdef _WIN32
    gethostname(buff, 32);
#else
    gethostname(buff, 32);
#endif

    return static_cast<std::string>(buff);
}

std::string getDatetime(datetimeFormat format) {
    std::time_t now = std::time(nullptr);
    std::string datetime;

    switch(format) {
        case utcFormat: {
            datetime = std::asctime(std::gmtime(&now));
            break;
        }
        case localFormat: {
            datetime = std::ctime(&now);
            break;
        }
        case unixtimeFormat: {
            datetime = std::to_string(now);
            break;
        }
        default:
            break;
    }

    if (datetime.back() == '\n')
        datetime = datetime.substr(0,datetime.size()-1);

    return datetime;
}

}  // namespace utils
