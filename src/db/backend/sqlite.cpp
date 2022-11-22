#include "db/backend/sqlite.hpp"

#include "excepts.hpp"
#include "ui/feedback.hpp"

namespace db {

namespace sqlite {

credetials getDefaults() {
    credetials ret;
    ret.user = default_user;
    ret.passwd = default_passwd;
    ret.dbname = "";
    ret.address = {"", default_port};
    return ret;
}

impl::impl(const std::string user, const std::string passwd, const std::string dbname, addr addr) {
    throw excepts::error("SQLite is not implemented!");
}

void impl::setup() { throw excepts::error("SQLite is not implemented!"); }

}  // namespace sqlite

}  // namespace db