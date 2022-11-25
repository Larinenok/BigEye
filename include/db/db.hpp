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

namespace dateLines {

namespace service {
struct line {
    std::string id;
    std::string datetime;
    std::string metadata;
};
const std::string postgresString =
    "test varchar (25) ";

const std::string sqliteString = "";
}  // namespace service

namespace journal {
struct line {
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

}  // namespace dateLines

// Base class of backend-specific implementation
class impl {
   public:
    virtual ~impl() = default;
    virtual void setup() = 0;

    virtual void journalWrite(dateLines::journal::line) = 0;
    virtual std::vector<dateLines::journal::line> journalRead() = 0;

    virtual void serviceWrite(dateLines::service::line) = 0;
    virtual std::vector<dateLines::service::line> serviceRead() = 0;
};

// Main interface
class db {
   private:
    std::shared_ptr<impl> backend;

   public:
    db();

    db(const backends::available backend = backends::postgres,
       credetials credetials = getDefaults(backends::postgres));

    db(const backends::available backend = backends::postgres, const std::string user = "",
       const std::string passwd = "", const std::string dbname = "bigeye", addr addr = {});

    void setup();

    void journalWrite(dateLines::journal::line);
    std::vector<dateLines::journal::line> journalRead();

    void serviceWrite(dateLines::service::line);
    std::vector<dateLines::service::line> serviceRead();
};

}  // namespace db