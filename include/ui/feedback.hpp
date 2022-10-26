#include <string>

#ifdef USE_COLORS
    #define CLR "\033[0m"
    #define RED "\033[31m"
    #define YEL "\033[33m"
    #define BOLD "\033[1m"
#else
    #define CLR ""
    #define RED ""
    #define YEL ""
    #define BOLD ""
#endif

namespace ui {

void error(std::string msg);
void warn(std::string msg);
void info(std::string msg);
void msg(std::string msg);

}
