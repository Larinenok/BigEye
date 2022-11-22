#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace db {

namespace backends {
enum available { sqlite, postgres };
}

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

credetials getDefaults(backends::available backend);

// Base class of backend-specific implementation
class impl {
   private:
   public:
    virtual ~impl() = default;
    virtual void test() = 0;
};

// Main interface
class db {
   private:
    std::unique_ptr<impl> backend;

   public:
    db(const backends::available backend = backends::postgres,
       credetials credetials = getDefaults(backends::postgres));

    db(const backends::available backend = backends::postgres, const std::string user = "",
       const std::string passwd = "", const std::string dbname = "bigeye", addr addr = {});

    // TEST
    void stats();
};

}  // namespace db