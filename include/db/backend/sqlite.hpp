#include "db/db.hpp"

namespace db {

namespace sqlite {

const std::string default_user = "sqlite";
const std::string default_passwd = "sqlite";
const std::string default_port = "";

credetials getDefaults();

class impl : public ::db::impl {
   private:
   public:
    impl(const std::string user, const std::string passwd, const std::string dbname, addr addr);
    void setup() override;
};

}  // namespace sqlite

}  // namespace db