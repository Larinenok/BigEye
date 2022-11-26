#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace db {

namespace backends {
enum available { none, postgres, sqlite };
}

struct addr {
    std::string ip = "127.0.0.1";
    std::string port = "";

    std::string path = "";

    bool pathPrefered = false;
};

struct credetials {
    backends::available backend = backends::none;
    std::string user = "";
    std::string passwd = "";
    std::string dbname = "bigeye";
    addr address = {};
};

credetials getDefaults(backends::available backend);

namespace dateRows {

namespace service {
struct row {
    std::string id;
    std::string datetime;
    std::string metadata;
};
const std::string postgresString = "test varchar (25) ";

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
    virtual void setup(){};

    // Service table
    virtual void serviceWrite(dateRows::service::row){};
    virtual std::vector<dateRows::service::row> serviceRead(size_t count) { return {}; };

    // Journal table
    virtual void journalWrite(dateRows::journal::row){};
    virtual std::vector<dateRows::journal::row> journalRead(size_t count) { return {}; };

    virtual size_t getRowsCount(std::string table) { return {}; };
};

// Main interface
class db {
   private:
    std::shared_ptr<impl> backend = std::make_shared<impl>();
    credetials login;

   public:
    db(const backends::available backend = backends::none, const std::string user = "",
       const std::string passwd = "", const std::string dbname = "bigeye", addr addr = {});

    void connect();
    void setup();

    size_t getRowsCount(std::string table);

    void journalWrite(dateRows::journal::row);
    std::vector<dateRows::journal::row> journalRead(size_t count);

    void serviceWrite(dateRows::service::row);
    std::vector<dateRows::service::row> serviceRead(size_t count);
};

}  // namespace db