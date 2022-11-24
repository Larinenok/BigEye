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
struct serviceLine {
    std::string id;
    std::string datetime;
    std::string metadata;

    std::string postgresString = "";
};

struct journalLine {
    std::string id;
    std::string datetime;
    std::string metadata;

    std::string postgresString = "";
};
}  // namespace dateLines

// Base class of backend-specific implementation
class impl {
   public:
    virtual ~impl() = default;
    virtual void setup() = 0;

    virtual void journalWrite(dateLines::journalLine) = 0;
    virtual std::vector<dateLines::journalLine> journalRead() = 0;

    virtual void serviceWrite(dateLines::serviceLine) = 0;
    virtual std::vector<dateLines::serviceLine> serviceRead() = 0;
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

    void journalWrite(dateLines::journalLine);
    std::vector<dateLines::journalLine> journalRead();

    void serviceWrite(dateLines::serviceLine);
    std::vector<dateLines::serviceLine> serviceRead();
};

}  // namespace db