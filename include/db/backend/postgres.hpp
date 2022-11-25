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
    size_t getRowsCount(std::string table) override;

    void journalWrite(dateRows::journal::row) override;
    std::vector<dateRows::journal::row> journalRead(size_t count) override;

    void serviceWrite(dateRows::service::row) override;
    std::vector<dateRows::service::row> serviceRead(size_t count) override;
};

}  // namespace postgres
}  // namespace db