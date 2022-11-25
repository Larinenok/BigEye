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
    std::unique_ptr<pqxx::connection> C;

   public:
    ~impl();
    impl(const std::string user, const std::string passwd, const std::string dbname, addr addr);
    void setup() override;

    void journalWrite(dateLines::journal::line) override;
    std::vector<dateLines::journal::line> journalRead() override;

    void serviceWrite(dateLines::service::line) override;
    std::vector<dateLines::service::line> serviceRead() override;
};

}  // namespace postgres
}  // namespace db