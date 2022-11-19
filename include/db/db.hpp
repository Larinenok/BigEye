#include <string>

#include "db/backend/sqlite.hpp"
#include "db/backend/postgres.hpp"

namespace db {

enum available { sqlite, postgres };

struct address {
   std::string addr;

   address(){}
   address(std::string addr){}

};

class db {
   private:
    void* impl;

   public:
    db(const enum available targetBackend, const std::string login = "",
       const std::string passwd = "" , const address addr = {});
};

}  // namespace db