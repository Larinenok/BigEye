#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace db {

struct addr {
    std::string ip = "127.0.0.1";
    std::string port = "";
};

struct credetials {
    std::string user = "";
    std::string passwd = "";
    std::string dbname = "bigeye";
    addr address = {};
};

namespace backends {
enum available { none, postgres, sqlite };
}

credetials getDefaults(backends::available backend);

namespace dateRows {

namespace service {
struct row {
    std::string id;
    std::string datetime;
    std::string metadata;
};
const std::string postgresString =
    "test varchar (25) ";

const std::string sqliteString = "";
}  // namespace service

namespace journal {
struct row {
    std::string id;
    std::string datetime;
    std::string metadata;
};
const std::string postgresString =
    "aboba varchar (25),"
    "bebra varchar (25) ";

const std::string sqliteString = "";
}  // namespace journal

/*constexpr std::string_view types(std::string_view& dbString) {}
    return dbString;
}*/

}  // namespace dateRows

// Base class of backend-specific implementation
class impl {
   public:
    virtual ~impl() = default;
    virtual void setup() = 0;

    // Service table
    virtual void serviceWrite(dateRows::service::row) = 0;
    virtual std::vector<dateRows::service::row> serviceRead(size_t count) = 0;

    // Journal table
    virtual void journalWrite(dateRows::journal::row) = 0;
    virtual std::vector<dateRows::journal::row> journalRead(size_t count) = 0;

    virtual size_t getRowsCount(std::string table) = 0;
};

// Main interface
class db {
   private:
    std::shared_ptr<impl> backend;
    backends::available currentBackend;

   public:
    db();

    db(const backends::available backend = backends::postgres,
       credetials credetials = getDefaults(backends::postgres));

    db(const backends::available backend = backends::postgres, const std::string user = "",
       const std::string passwd = "", const std::string dbname = "bigeye", addr addr = {});

    void setup();

    size_t getRowsCount(std::string table);

    void journalWrite(dateRows::journal::row);
    std::vector<dateRows::journal::row> journalRead(size_t count);

    void serviceWrite(dateRows::service::row);
    std::vector<dateRows::service::row> serviceRead(size_t count);
};

}  // namespace db