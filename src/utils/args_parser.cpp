#include <iostream>

#include "utils/args.hpp"
#include "runtime.hpp"

namespace po = utils::program_options;

namespace utils {

void parseArgs(const int argc, char *argv[]) {
    po::option_description desc("Usage: FastBleed [ options ... ]\n");
    desc.add_options()
        ("help,h", "Help page")
        ("dry,d", "asd")
        ("headless,l", "asd")
    ;

    po::options_map args;
    po::store(po::parse_command_line(argc, argv, desc), args);

    if (args.count("error")) {
        std::cout << "Invalid argument\n" << desc << std::endl;
        exit(0);
    }
    if (args.count("dry")) {
        runtime::FLAG_dryRun = true;
    }
    if (args.count("headless")) {
        runtime::FLAG_headless = true;
    }
    if (args.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }
}

}// end utils