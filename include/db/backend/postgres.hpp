#include <string>
#include "db/db.hpp"

namespace db {
namespace postgres {

const std::string default_port = "5432";

class impl : ::db::impl {
   private:
   public:
    impl();
    void test() override {}
};

}  // namespace postgres
}  // namespace db