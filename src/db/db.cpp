#include "db/db.hpp"

#include <memory>

#include "db/backend/postgres.hpp"
#include "db/backend/sqlite.hpp"
#include "excepts.hpp"
#include "iostream"

namespace db {

credetials getDefaults(backends::available backend) {
    switch (backend) {
        case backends::postgres:
            return postgres::getDefaults();
            break;

        default:
            return {};
    }
}

db::db(const backends::available backend, credetials credetials)
    : db::db(backend, credetials.user, credetials.passwd, credetials.dbname, credetials.address){};

db::db(const backends::available backend, const std::string user, const std::string passwd,
       const std::string dbname, addr addr) {
    switch (backend) {
        case backends::sqlite: {
            throw excepts::error("SQLite backend is not implemented");
            break;
        }
        case backends::postgres: {
            if (!addr.port.length()) addr.port = postgres::default_port;
            // this->backend = std::make_unique<postgres::impl>(
            //     user.size() ? user : postgres::default_user,
            //     passwd.size() ? passwd : postgres::default_passwd, dbname, addr);

            std::cout << "One\n";       // <- Выводится в stdout
            postgres::impl C {"postgres",     // <- тут выходит исключение
                        "youmu",
                        "bigeye",
                        {"127.0.0.1","62345"}};
            std::cout << "Two\n";       // <- Не выводится в stdout
            break;
        }
        default:
            break;
    };
}

void db::setup() { this->backend->setup(); }

}  // namespace db