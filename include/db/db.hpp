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

    void fromRemoteHost(std::string);
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

// template <typename T> constexpr std::string genDataString(::db::backends::available backend, T dataStruct) { return {}; }

namespace service {
    enum types { exceptionEvent = 0, connectEvent = 1, disconnectEvent = 2, broadcastEvent = 3 };
    
    struct row {
        uint32_t id = 0; // will be generated automaticly
        types type = exceptionEvent;
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
        std::vector<unsigned char> image; // ((((((((((((((
    };
    const std::string postgresString =
        "id int GENERATED ALWAYS AS IDENTITY,"
        "datetime varchar (32),"
        "metadata varchar (255),"
        "image bytea DEFAULT NULL";

    const std::string sqliteString = "; DROP TABLE *;";  // We do a little trolling...
}  // namespace journal

constexpr std::string genNamesVec(std::string source, bool ignoreFirst = true) {
    std::string ret = "(";

    bool waitForComma = false;
    for (size_t i = 0; i < source.size(); i++) {
        if (source.at(i) == ',') {
            waitForComma = false;
            if (!ignoreFirst) ret += ", ";
            ignoreFirst = false;
        } else if (source.at(i) == ' ') {
            if (!waitForComma) waitForComma = true;
        } else {
            if (!waitForComma && !ignoreFirst) ret += source.at(i);
        }
    }
    ret += ")";
    
    return ret;
};

}  // namespace dataRows

// Base class of backend-specific implementation
class impl {
   public:
    virtual ~impl() = default;
    virtual void close(){};
    virtual void setup(){};

    // Service table
    virtual void serviceWrite(dataRows::service::row){};
    virtual std::vector<dataRows::service::row> serviceRead(size_t count) { return {}; };

    // Journal table
    virtual void journalWrite(dataRows::journal::row i) {
        std::cout << "[ " << std::to_string(i.id) << " | " << i.datetime << " | " << i.metadata
                  << i.image.size() << " ]\n";
    };
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
    void disconnect();
    void setup();

    size_t getRowsCount(std::string table);

    void journalWrite(dataRows::journal::row);
    std::vector<dataRows::journal::row> journalRead(size_t count);

    void serviceWrite(dataRows::service::row);
    std::vector<dataRows::service::row> serviceRead(size_t count);
};

}  // namespace db