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

void impl::close() { this->C->close(); }

void impl::setup() {
    std::vector<std::pair<std::string, std::string>> iter = {
        {"service", dataRows::service::postgresString},
        {"journal", dataRows::journal::postgresString}};

    for (auto& i : iter) {
        pqxx::work W{*C};
        char ret = W.query_value<std::string>(
                        "SELECT EXISTS ("
                        "SELECT FROM pg_tables "
                        "WHERE schemaname = 'public' AND tablename = '" +
                        i.first + "')")
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
    return static_cast<size_t>(W.query_value<size_t>("SELECT COUNT(*) FROM " + table));
}


using namespace dataRows;

// Service table:
void impl::serviceWrite(service::row dataRow) {
    pqxx::work W{*C};
    W.exec("INSERT INTO service " + genNamesVec(service::postgresString) + " VALUES (" +
           std::to_string(static_cast<int>(dataRow.type)) + ", '" + dataRow.data + "');");
    W.commit();
}
std::vector<service::row> impl::serviceRead(size_t count) {
    std::vector<service::row> ret;
    pqxx::work W{*C};
    auto response = W.exec_n(count, "SELECT * FROM service LIMIT " + std::to_string(count) + ";");

    for (auto i : response) {
        service::row row;
        row.id = std::stoul(i.at(0).c_str());
        row.type = static_cast<service::types>(i.at(1).get<int>().value());
        row.data = i.at(2).c_str();
        ret.push_back(row);
    }
    return ret;
}

// Journal table:
void impl::journalWrite(journal::row dataRow) {
    std::cout << "enter\n";
    pqxx::work W{*C};
    W.exec("INSERT INTO journal " + genNamesVec(journal::postgresString) + " VALUES ('" + dataRow.datetime + "', '" +
           dataRow.metadata + "', NULL);");
    W.commit();
    std::cout << "out\n";
}
std::vector<journal::row> impl::journalRead(size_t count) {
    std::vector<journal::row> ret;
    pqxx::work W{*C};
    auto response = W.exec_n(count, "SELECT * FROM journal LIMIT " + std::to_string(count) + ";");

    for (auto i : response) {
        journal::row row;
        row.id = std::stoul(i.at(0).c_str());
        row.datetime = i.at(1).c_str();
        row.metadata = i.at(2).c_str();
        row.image = nullptr;
        ret.push_back(row);
    }

    return ret;
}

}  // namespace postgres

}  // namespace db