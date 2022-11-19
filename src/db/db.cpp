#include "db/db.hpp"

#include <memory>
#include <pqxx/pqxx>  // REMOVE

#include "db/backend/postgres.hpp"
#include "db/backend/sqlite.hpp"
#include "excepts.hpp"
#include "iostream"

namespace db {

db::db(const backends::available targetBackend, const std::string user, const std::string passwd,
       const std::string dbname, addr addr) {
    switch (targetBackend) {
        case backends::sqlite:
            excepts::error("SQLite backend is not implemented");
            break;

        case backends::postgres:
            if (!addr.port.length()) addr.port = postgres::default_port;
            // this->backend = std::make_unique<postgres::impl>();

            // REMOVE
            try {
                pqxx::connection C("dbname = " + dbname + " user = " + user + " password = " +
                                   passwd + " hostaddr = " + addr.ip + " port = " + addr.port);
                
                if (C.is_open()) {
                    std::cout << "Opened database successfully: " << C.dbname() << std::endl;
                } else {
                    std::cout << "Can't open database" << std::endl;
                }
                C.close();
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
            // \REMOVE

            break;

        default:
            break;
    };
}

void db::stats() { this->backend->test(); }

}  // namespace db