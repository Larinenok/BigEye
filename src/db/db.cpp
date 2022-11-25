#include "db/db.hpp"

#include <memory>

#include "db/backend/postgres.hpp"
#include "db/backend/sqlite.hpp"
#include "excepts.hpp"

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
    this->currentBackend = backend;
    switch (backend) {
        case backends::sqlite:
            throw excepts::error("SQLite backend is not implemented");
            break;

        case backends::postgres: {
            if (!addr.port.length()) addr.port = postgres::default_port;

            this->backend = std::make_shared<postgres::impl>(
                user.size() ? user : postgres::default_user,
                passwd.size() ? passwd : postgres::default_passwd, dbname, addr);
            break;
        }

        default:
            break;
    };
}

void db::setup() { this->backend->setup(); }
size_t db::getRowsCount(std::string table) { return this->backend->getRowsCount(table); }

void db::journalWrite(dateRows::journal::row arg) { this->backend->journalWrite(arg); }
std::vector<dateRows::journal::row> db::journalRead(size_t count) { return this->backend->journalRead(count); }

void db::serviceWrite(dateRows::service::row arg) { this->backend->serviceWrite(arg); }
std::vector<dateRows::service::row> db::serviceRead(size_t count) { return this->backend->serviceRead(count); }

}  // namespace db