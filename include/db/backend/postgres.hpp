#pragma once

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
   public:
    impl(const std::string user, const std::string passwd, const std::string dbname, addr addr);
    void test() override;
};

}  // namespace postgres
}  // namespace db