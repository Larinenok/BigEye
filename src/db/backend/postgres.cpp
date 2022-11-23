#include "db/backend/postgres.hpp"

#include <pqxx/pqxx>

#include "excepts.hpp"
#include "ui/feedback.hpp"

namespace db {

namespace postgres {

credetials getDefaults() {
    credetials ret;
    ret.user = default_user;
    ret.passwd = default_passwd;
    ret.dbname = "";
    ret.address = {"", default_port};
    return ret;
}

impl::impl(const std::string user, const std::string passwd, const std::string dbname, addr addr) {
    std::cout << "Three\n";  // <- Не выводится
    try {
        std::cout << "Four\n";
        this->C = std::make_unique<pqxx::connection>("dbname = " + dbname + " user = " + user +
                                                     " password = " + passwd +
                                                     " host = " + addr.ip + " port = " + addr.port);
        if (!this->C->is_open()) throw excepts::error("Can't open database");
        ui::msg("Opened database successfully: " + static_cast<std::string>(this->C->dbname()));

    } catch (const std::exception &e) {
        throw excepts::error(e.what());
    }
}

impl::~impl() { this->C->close(); }

void impl::setup() {
    // ui::trace("Setting up tables...");
    pqxx::work W{*C};
    char ret;
    ret = W.query_value<std::string>(
               "SELECT EXISTS ("
               "SELECT FROM pg_tables "
               "WHERE schemaname = 'public' AND tablename  = 'journal'"
               ")")
              .at(0);

    if (ret == 'f') {  // Table does ot exist; Create
        W.exec(
            "CREATE TABLE journal ("
            "aboba varchar (25),"
            "bebra varchar (25)"
            ") ");
        W.commit();
        // ui::trace("New table created");
    } else if (ret != 't')  // Not true of false
        throw excepts::error("Database returns unrecognised response!");
}

}  // namespace postgres

}  // namespace db