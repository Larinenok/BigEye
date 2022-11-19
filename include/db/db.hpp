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

class impl {
   private:
   public:
    virtual void test() = 0;
};

class db {
   private:
    std::unique_ptr<impl> backend;

   public:
    db(const backends::available targetBackend = backends::sqlite, const std::string user = "",
       const std::string passwd = "", const std::string dbname = "BigEye", addr addr = {});

    void stats();
};

}  // namespace db