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

namespace dataRows {

namespace service {
enum types { connectEvent = 0, disconnectEvent = 1, exceptionEvent = 2 };
struct row {
    uint32_t id = 0; // will be generated automaticly
    types type;
    std::string data = {};
};
const std::string postgresString =
    "id int GENERATED ALWAYS AS IDENTITY,"
    "type smallint,"
    "data varchar (255) ";

const std::string sqliteString = "; DROP TABLE *;";  // We do a little trolling...
}  // namespace service

namespace journal {
struct row {
    uint32_t id = 0; // will be generated automaticly
    std::string datetime;
    std::string metadata;
    void* image = nullptr;
};
const std::string postgresString =
    "id int GENERATED ALWAYS AS IDENTITY,"
    "datetime varchar (32),"
    "metadata varchar (255),"
    "image bytea DEFAULT NULL ";

const std::string sqliteString = "; DROP TABLE *;";  // We do a little trolling...

}  // namespace journal

}  // namespace dataRows

// Base class of backend-specific implementation
class impl {
   public:
    virtual ~impl() = default;
    virtual void setup(){};

    // Service table
    virtual void serviceWrite(dataRows::service::row){};
    virtual std::vector<dataRows::service::row> serviceRead(size_t count) { return {}; };

    // Journal table
    virtual void journalWrite(dataRows::journal::row){};
    virtual std::vector<dataRows::journal::row> journalRead(size_t count) { return {}; };

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

    void journalWrite(dataRows::journal::row);
    std::vector<dataRows::journal::row> journalRead(size_t count);

    void serviceWrite(dataRows::service::row);
    std::vector<dataRows::service::row> serviceRead(size_t count);
};

}  // namespace db