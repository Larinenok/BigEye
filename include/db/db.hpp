#include <string>

#include "db/backend/sqlite.hpp"

namespace db {

enum available { sqlite };

class db {
   private:
    void* impl;

   public:
    db(const enum available targetBackend, const std::string login = "",
       const std::string passwd = "" /*, const std::string addr*/);
};

}  // namespace db