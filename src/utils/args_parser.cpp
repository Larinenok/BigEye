#include <iostream>

#include "utils/args.hpp"
#include "runtime.hpp"

namespace po = utils::program_options;

namespace utils {

void parseArgs(const int argc, char *argv[]) {
    po::option_description desc("Usage: BigEye [ options ... ]\n");
    desc.add_options()
        ("help,h", "Help page")
        ("dry,d", "Dry run")
        ("headless,l", "No GUI mode")
        ("cuda,c", "Use NoVideo CUDA")
        ("noscan,n", "Do not scan cameras")
        ("db-backend,b", "<POSTGRES/SQLITE/NONE>", true)
        ("db-address,a", "<<IP:PORT> or <PATH_TO_SOCKFILE>>", true)
        ("db-user,u", "<USERNAME>", true)
        ("db-passwd,p", "<PASSWORD>", true)
        ("db-name,n", "<DATABASE NAME>", true);

    po::options_map args;
    po::store(po::parse_command_line(argc, argv, desc), args);

    if (args.count("error")) {
        std::cout << "Invalid argument" << std::endl;
        std::cout << desc << std::endl;
        exit(0);
    }
    if (args.count("dry")) {
        runtime::FLAG_dryRun = true;
    }
    if (args.count("headless")) {
        runtime::FLAG_headless = true;
    }
    if (args.count("cuda")) {
        runtime::FLAG_useCuda = true;
    }
    if (args.count("noscan")) {
        runtime::FLAG_noScan = true;
    }
    if (args.count("db-backend")) {
        runtime::KEY_db_backend = args.at("db-backend").get_value();
    }
    if (args.count("db-address")) {
        runtime::KEY_db_address = args.at("db-address").get_value();
    }
    if (args.count("db-user")) {
        runtime::KEY_db_user = args.at("db-user").get_value();
    }
    if (args.count("db-passwd")) {
        runtime::KEY_db_passwd = args.at("db-passwd").get_value();
    }
    if (args.count("db-name")) {
        runtime::KEY_db_name = args.at("db-name").get_value();
    }
    if (args.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }
}

}// end utils