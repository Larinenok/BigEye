#include "db/backend/postgres.hpp"

#include <db/db.hpp>
#include <pqxx/pqxx>
#include <string>

#include "excepts.hpp"
#include "ui/feedback.hpp"

namespace db {

namespace postgres {

credetials getDefaults() {
    credetials ret;
    ret.backend = backends::postgres;
    ret.user = default_user;
    ret.passwd = default_passwd;
    ret.dbname = "";
    ret.address = {"", default_port};
    return ret;
}

impl::impl(const std::string user, const std::string passwd, const std::string dbname, addr addr) {
    try {
        this->C = std::make_unique<pqxx::connection>("dbname = " + dbname + " user = " + user +
                                                     " password = " + passwd +
                                                     " host = " + addr.ip + " port = " + addr.port);
        if (!this->C->is_open()) throw excepts::error("Can't open database");
        ui::msg("Opened database successfully: " + static_cast<std::string>(this->C->dbname()));

    } catch (const std::exception& e) {
        throw excepts::error(e.what());
    }
}

impl::~impl() { this->C->close(); }

void impl::setup() {
    std::vector<std::pair<std::string, std::string>> iter = {
        {"service", dateRows::service::postgresString},
        {"journal", dateRows::journal::postgresString}};

    for (auto& i : iter) {
        pqxx::work W{*C};
        char ret = W.query_value<std::string>(
                        "SELECT EXISTS ("
                            "SELECT FROM pg_tables "
                                "WHERE schemaname = 'public' AND tablename = '" + i.first + "'"
                        ")")
                       .at(0);
        if (ret == 'f') {
            // Table does ot exist; Create
            W.exec("CREATE TABLE " + i.first + " (" + i.second + ") ");
            W.commit();
        }
    }
}

size_t impl::getRowsCount(std::string table) {
    pqxx::work W{*C};
    return static_cast<size_t>(W.query_value<size_t>("SELECT COUNT(*) FROM journal"));
}

// Journal table:
void impl::journalWrite(dateRows::journal::row dateRow) {
    pqxx::work W{*C};
    W.exec("INSERT INTO journal (aboba, bebra) VALUES ('swing', 'yellow');");
    W.commit();
}
std::vector<dateRows::journal::row> impl::journalRead(size_t count) {
    pqxx::work W{*C};
    auto ret = W.exec_n(count, "SELECT * FROM journal LIMIT " + std::to_string(count) + ";");
    for(auto i : ret) {
        std::cout << "[ ";
        for(int ii = 0; ii < ret.columns(); ii++) 
            std::cout << i.at(ii) << ' ';
        std::cout << "]\n";
    }
    return {};
}

// Service table:
void impl::serviceWrite(dateRows::service::row dateRow) {
    pqxx::work W{*C};
    W.exec("INSERT INTO service (test) VALUES ('swing');");
    W.commit();
}
std::vector<dateRows::service::row> impl::serviceRead(size_t count) {
    pqxx::work W{*C};
    auto ret = W.exec_n(count, "SELECT * FROM service LIMIT " + std::to_string(count) + ";");
    for(auto i : ret) {
        std::cout << "[ ";
        for(int ii = 0; ii < ret.columns(); ii++) 
            std::cout << i.at(ii) << ' ';
        std::cout << "]\n";
    }
    return {};
}

}  // namespace postgres

}  // namespace db