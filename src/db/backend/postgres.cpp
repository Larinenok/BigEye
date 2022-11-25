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
        {"service", dateLines::service::postgresString},
        {"journal", dateLines::journal::postgresString}};

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

// Journal table:
void impl::journalWrite(dateLines::journal::line dateLine) {
    pqxx::work W{*C};
    W.exec("INSERT INTO journal (aboba, bebra) VALUES ('swing', 'yellow');");
    W.commit();
}
std::vector<dateLines::journal::line> impl::journalRead() {
    pqxx::work W{*C};
    // std::string ret = W.query_value<std::string>("SELECT * FROM journal;");
    auto ret = W.exec_n(20, "SELECT * FROM journal;");
    std::cout << ret.columns() << " " << ret.at(0).at(0) << '\n';
    return {};
}

// Service table:
void impl::serviceWrite(dateLines::service::line dateLine) {
    pqxx::work W{*C};
    W.exec("INSERT INTO journal (aboba, bebra) VALUES ('swing', 'yellow');");
    W.commit();
}
std::vector<dateLines::service::line> impl::serviceRead() {
    pqxx::work W{*C};
    // std::string ret = W.query_value<std::string>("SELECT * FROM journal;");
    auto ret = W.exec_n(20, "SELECT * FROM journal;");
    std::cout << ret.columns() << " " << ret.at(0).at(0) << '\n';
    return {};
}

}  // namespace postgres

}  // namespace db