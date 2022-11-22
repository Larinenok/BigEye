#pragma once

#include <pqxx/pqxx>
#include <string>

#include "db/db.hpp"

namespace db {
namespace postgres {

const std::string default_user = "postgres";
const std::string default_passwd = "postgres";
const std::string default_port = "5432";

credetials getDefaults();

class impl : public ::db::impl {
   private:
    pqxx::connection C;

   public:
    ~impl();
    impl(const std::string user, const std::string passwd, const std::string dbname, addr addr);
    void setup() override;
};

}  // namespace postgres
}  // namespace db