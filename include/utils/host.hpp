#include <string>

namespace utils {

std::string getHostname();

enum datetimeFormat { utcFormat = 0, localFormat = 1, unixtimeFormat = 2 };
std::string getDatetime(datetimeFormat = utcFormat);

}  // namespace utils