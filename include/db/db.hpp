#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace db {

namespace backends {
enum available { postgres, sqlite };
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
   public:
    virtual ~impl() = default;
    virtual void setup() = 0;
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

    void setup();  // Create some tables

    // Table classes:
    //class registered {};
    class journal {
        public:
        struct dataLine {
            std::string id;
            std::string datetime;
            std::string metadata;
        };

        std::vector<dataLine> get();
        void put(dataLine line);
        
    };
};

}  // namespace db