#include <iostream>
#include "ui/feedback.hpp"

namespace ui {

    void error(std::string msg) {
        std::cerr << "[" <<RED<<BOLD<<"ERROR"<<CLR<< "] "
            << BOLD<<msg<<CLR 
            << std::endl;
        // GUI popup here
    }

    void warn(std::string msg) {
        std::cerr << "[" <<YEL<<BOLD<<"WARN!"<<CLR<< "] "
            << BOLD<<msg<<CLR
            << std::endl;
        // GUI popup here
    }

    void info(std::string msg) {
        std::cout << "[INFO_] " << msg << std::endl;
        // GUI popup here (?)
    }

    void msg(std::string msg) {
        std::cout << msg << std::endl;
        // I think, popup is not necessary there, delete this after reading ^_^
    }

}
