#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace db {

namespace backends {
enum available { none, postgres, sqlite };  // NONE!
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
    virtual void write() = 0;
    virtual void read() const = 0;
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

    void setup();  // Create some tables // TODO: Autodetect dataLines and tables

    // Tables:

    // ...

    class _journal {
       private:
        std::shared_ptr<impl> backend;

       public:
        _journal(std::shared_ptr<impl> backend) : backend(backend) {}

        struct dataLine {
            std::string id;
            std::string datetime;
            std::string metadata;
        };

        std::vector<dataLine> read();
        void write(dataLine line);
    };

    std::unique_ptr<_journal> journal;
};

}  // namespace db