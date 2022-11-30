#include "db/db.hpp"

#include <memory>

#include "db/backend/postgres.hpp"
#include "db/backend/sqlite.hpp"
#include "excepts.hpp"
#include "runtime.hpp"

namespace db {

void addr::fromRemoteHost(std::string string) {
    size_t colonPos = string.rfind(':');
    this->ip = string.substr(0, colonPos);
    this->port = string.substr(colonPos+1);
}

credetials getDefaults(backends::available backend) {
    switch (backend) {
        case backends::postgres:
            return postgres::getDefaults();
            break;

        default:
            return {};
    }
}

db::db(backends::available backend, const std::string user, const std::string passwd,
       const std::string dbname, addr addr) {
    credetials defaults;
    switch (backend) {
        case backends::postgres:
            defaults = postgres::getDefaults();
            break;

        case backends::sqlite:
            sqlite::getDefaults();
            break;

        default:
            break;
    }

    this->login.backend = backend;
    this->login.user = (user.size()) ? user : defaults.user;
    this->login.passwd = (passwd.size()) ? passwd : defaults.passwd;
    this->login.dbname = (dbname.size()) ? dbname : defaults.dbname;
    this->login.address = addr;  // Later
}

void db::connect() {
    backends::available _backend = this->login.backend;
    if (runtime::FLAG_dryRun) _backend = backends::none;

    switch (_backend) {
        case backends::sqlite:
            throw excepts::error("SQLite backend is not implemented");
            this->backend = std::make_shared<impl>();  // REPLACE
            break;

        case backends::postgres: {
            this->backend = std::make_shared<postgres::impl>(
                this->login.user.size() ? this->login.user : postgres::default_user,
                this->login.passwd.size() ? this->login.passwd : postgres::default_passwd,
                this->login.dbname, this->login.address);
            break;
        }

        default:
            this->backend = std::make_shared<impl>();
    };
}

void db::disconnect() { this->backend->close(); }

void db::setup() { this->backend->setup(); }
size_t db::getRowsCount(std::string table) { return this->backend->getRowsCount(table); }

void db::journalWrite(dataRows::journal::row arg) { this->backend->journalWrite(arg); }
std::vector<dataRows::journal::row> db::journalRead(size_t count) {
    return this->backend->journalRead(count);
}

void db::serviceWrite(dataRows::service::row arg) { this->backend->serviceWrite(arg); }
std::vector<dataRows::service::row> db::serviceRead(size_t count) {
    return this->backend->serviceRead(count);
}

}  // namespace db