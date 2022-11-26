#pragma once

#include <string>

#include "db/db.hpp"

namespace db {
namespace sqlite {

const std::string default_user = "";
const std::string default_passwd = "";

credetials getDefaults();

class impl : public ::db::impl {
   public:
    ~impl();
    impl(const std::string user, const std::string passwd, const std::string dbname, addr addr);
};

}  // namespace postgres
}  // namespace db