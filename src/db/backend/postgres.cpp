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
    ret.addr = {"", default_port};
    return ret;
}

impl::impl(const std::string user, const std::string passwd, const std::string dbname, addr addr) {
    try {
        pqxx::connection C("dbname = " + dbname + " user = " + user + " password = " + passwd +
                           " host = " + addr.ip + " port = " + addr.port);

        if (!C.is_open()) throw excepts::error("Can't open database");
        //ui::trace("Opened database successfully: "+C.dbname());
        pqxx::work W{C};

        // pqxx::row r = W.exec1(); r[0]
        // constexpr inline std::string_view = "...";
        
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
            //ui::trace("New table created");
        } else if (ret != 't')  // Not true of false
            throw excepts::error("Database returns unrecognised response!");

        C.close();

    } catch (const std::exception &e) {
        throw excepts::error(e.what());
    }
}

void impl::test() { std::cout << "Test called!\n"; }

}  // namespace postgres

}  // namespace db